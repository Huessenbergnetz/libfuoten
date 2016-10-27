/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016 Buschtrommel / Matthias Fehring
 * https://www.buschmann23.de/entwicklung/bibliotheken/libfuoten/
 * https://github.com/Buschtrommel/libfuoten
 *
 * This library is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "component_p.h"
#include "../error.h"
#include <QJsonParseError>
#include <QUrl>
#ifdef QT_DEBUG
#include <QtDebug>
#endif

using namespace Fuoten;


Component::Component(QObject *parent) :
    QObject(parent), d_ptr(new ComponentPrivate)
{
}


Component::Component(ComponentPrivate &dd, QObject *parent) :
    QObject(parent), d_ptr(&dd)
{

}



Component::~Component()
{
}





void Component::sendRequest()
{
    Q_D(Component);

    setError(nullptr);

    d->result.clear();
    d->jsonResult = QJsonDocument();

    if (!checkInput()) {
        setInOperation(false);
        return;
    }

    QUrl url;

    if (d->configuration->getUseSSL()) {
        url.setScheme(QStringLiteral("https"));
    } else {
        url.setScheme(QStringLiteral("http"));
    }

    if (d->configuration->getServerPort() != 0) {
        url.setPort(d->configuration->getServerPort());
    }

    url.setHost(d->configuration->getHost());

    QString urlPath = d->configuration->getInstallPath();
    urlPath.append(QStringLiteral("/index.php/apps/news/api/v1-2"));
    urlPath.append(d->apiRoute);

    url.setPath(urlPath);

    if (!d->urlQuery.isEmpty()) {
        url.setQuery(d->urlQuery);
    }

    if (!url.isValid()) {
        //% "Invalid API URL"
        setError(new Error(Error::InputError, Error::Critical, qtTrId("invalid-api-url"), url.toString(), this));
        Q_EMIT failed(error());
        setInOperation(false);
        return;
    }

    if (!d->networkAccessManager) {
        setNetworkAccessManager(new QNetworkAccessManager(this));
        if (d->configuration->getIgnoreSSLErrors()) {
            connect(d->networkAccessManager, &QNetworkAccessManager::sslErrors, this, &Component::_ignoreSSLErrors);
        }
    }

    QNetworkRequest nr;

    nr.setUrl(url);

    if (!d->requestHeaders.isEmpty()) {
        QHash<QByteArray, QByteArray>::const_iterator i = d->requestHeaders.constBegin();
        while (i != d->requestHeaders.constEnd()) {
            nr.setRawHeader(i.key(), i.value());
            ++i;
        }
    }

    nr.setRawHeader(QByteArrayLiteral("User-Agent"), d->configuration->getUserAgent().toUtf8());

    if (!d->payload.isEmpty()) {
        nr.setRawHeader(QByteArrayLiteral("Content-Length"), QByteArray::number(d->payload.length()));
        nr.setRawHeader(QByteArrayLiteral("Content-Type"), QByteArrayLiteral("application/json"));
    }

    if (d->expectedJSONType != Empty) {
        nr.setRawHeader(QByteArrayLiteral("Accept"), QByteArrayLiteral("application/json"));
    }

    if (d->requiresAuth) {
        QByteArray authHeader = QByteArrayLiteral("Basic ");
        QString auth(d->configuration->getUsername());
        auth.append(QLatin1String(":")).append(d->configuration->getPassword());
        authHeader.append(auth.toUtf8().toBase64());
        nr.setRawHeader(QByteArrayLiteral("Authorization"), authHeader);
    }

#ifdef QT_DEBUG
    qDebug() << "Start performing network operation.";
    qDebug() << "API URL:" << url;
    if (!nr.rawHeaderList().isEmpty()) {
        const QList<QByteArray> hl = nr.rawHeaderList();
        for (const QByteArray &h : hl) {
            if (h != QByteArrayLiteral("Authorization")) {
                qDebug() << h << nr.rawHeader(h);
            }
        }
    }
    if (!d->payload.isEmpty()) {
        qDebug() << "Payload:" << d->payload;
    }
#endif

    if (d->requestTimeout > 0) {
        if (!d->timeoutTimer) {
            d->timeoutTimer = new QTimer(this);
            d->timeoutTimer->setSingleShot(true);
            d->timeoutTimer->setTimerType(Qt::VeryCoarseTimer);
            connect(d->timeoutTimer, &QTimer::timeout, this, &Component::_requestTimedOut);
        }
        d->timeoutTimer->start(d->requestTimeout * 1000);
    }

    d->performNetworkOperation(nr);
    connect(d->reply, &QNetworkReply::finished, this, &Component::_requestFinished);
}




void Component::_requestFinished()
{
    Q_D(Component);

    if (d->timeoutTimer && d->timeoutTimer->isActive()) {
        d->timeoutTimer->stop();
    }

    d->result = d->reply->readAll();

#ifdef QT_DEBUG
//    qDebug() << "Request result:" << d->result;
    fprintf(stderr, "Request result: \n%s\n", d->result.constData());
#endif

    if (d->reply->error() == QNetworkReply::NoError) {

        if (checkOutput()) {
            successCallback();
        } else {
            setInOperation(false);
        }

    } else {
        extractError(d->reply);
        setInOperation(false);
    }

    d->reply->deleteLater();
    d->reply = nullptr;
}



void Component::_requestTimedOut()
{
    Q_D(Component);

    //% "The connection to the server timed out after %n second(s)."
    setError(new Error(Error::RequestError, Error::Critical, qtTrId("err-conn-timeout", requestTimeout()), d->reply->request().url().toString(), this));

    QNetworkReply *nr = d->reply;
    d->reply = nullptr;
    delete nr;

}


void Component::_ignoreSSLErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    Q_UNUSED(errors);
    if (reply) {
        reply->ignoreSslErrors();
    }
}


bool Component::checkInput()
{
    Q_D(Component);

    if (!d->configuration) {
        //% "No configuration available."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("id-err-no-config"), QString(), this));
        Q_EMIT failed(error());
        return false;
    }

    if (d->requiresAuth && (d->configuration->getUsername().isEmpty() || d->configuration->getPassword().isEmpty())) {
        //% "You have to specify a username and a password."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("err-username-pass-missing"), QString(), this));
        Q_EMIT failed(error());
        return false;
    }

    if (d->configuration->getHost().isEmpty()) {
        //% "No host specified"
        setError(new Error(Error::InputError, Error::Critical, qtTrId("err-no-host"), QString(), this));
        Q_EMIT failed(error());
        return false;
    }

    if (d->apiRoute.isEmpty()) {
        //% "No API route specified."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("err-no-route"), QString(), this));
        Q_EMIT failed(error());
        return false;
    }

    if ((d->namOperation == QNetworkAccessManager::PostOperation || d->namOperation == QNetworkAccessManager::PutOperation) && d->payload.isEmpty()) {
        //% "Empty payload when trying to perform a PUT or POST network operation."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("err-no-payloud"), QString(), this));
        Q_EMIT failed(error());
        return false;
    }

    return true;
}



bool Component::checkOutput()
{
    Q_D(Component);

    if (!(d->expectedJSONType == Empty)) {
        QJsonParseError jsonError;
        d->jsonResult = QJsonDocument::fromJson(d->result, &jsonError);
        if (jsonError.error != QJsonParseError::NoError) {
            setError(new Error(jsonError, this));
            Q_EMIT failed(error());
            return false;
        }
    }

    if ((d->jsonResult.isNull() || d->jsonResult.isEmpty()) && !(d->expectedJSONType == Empty)) {
        //% "The request replied an empty answer, but there was content expected."
        setError(new Error(Error::OutputError, Error::Critical, qtTrId("err-empty-answer"), QString(), this));
        Q_EMIT failed(error());
        return false;
    }

    if ((d->expectedJSONType == Array) && !d->jsonResult.isArray()) {
        //% "It was expected that the request returns a JSON array, but it returned something else."
        setError(new Error(Error::OutputError, Error::Critical, qtTrId("err-no-json-array"), QString(), this));
        Q_EMIT failed(error());
        return false;
    }

    if ((d->expectedJSONType == Object && !d->jsonResult.isObject())) {
        //% "It was expected that the request returns a JSON object, but it returned something else."
        setError(new Error(Error::OutputError, Error::Critical, qtTrId("err-no-json-object"), QString(), this));
        Q_EMIT failed(error());
        return false;
    }

    return true;
}




QNetworkAccessManager *Component::networkAccessManager() const { Q_D(const Component); return d->networkAccessManager; }

void Component::setNetworkAccessManager(QNetworkAccessManager *nNetworkAccessManager)
{
    Q_D(Component); 
    if (nNetworkAccessManager != d->networkAccessManager) {
        d->networkAccessManager = nNetworkAccessManager;
#ifdef QT_DEBUG
        qDebug() << "Changed networkAccessManager to" << d->networkAccessManager;
#endif
        Q_EMIT networkAccessManagerChanged(networkAccessManager());
    }
}




bool Component::inOperation() const { Q_D(const Component); return d->inOperation; }

void Component::setInOperation(bool nInOperation)
{
    Q_D(Component); 
    if (nInOperation != d->inOperation) {
        d->inOperation = nInOperation;
#ifdef QT_DEBUG
        qDebug() << "Changed inOperation to" << d->inOperation;
#endif
        Q_EMIT inOperationChanged(inOperation());
    }
}




quint8 Component::requestTimeout() const { Q_D(const Component); return d->requestTimeout; }

void Component::setRequestTimeout(quint8 seconds)
{
    Q_D(Component); 
    if (seconds != d->requestTimeout) {
        d->requestTimeout = seconds;
#ifdef QT_DEBUG
        qDebug() << "Changed requestTimeout to" << d->requestTimeout;
#endif
        Q_EMIT requestTimeoutChanged(requestTimeout());
    }
}




Error *Component::error() const { Q_D(const Component); return d->error; }

void Component::setError(Error *nError)
{
    Q_D(Component); 
    if (nError != d->error) {
        Error *oldError = d->error;
        d->error = nError;
        if (oldError) {
            delete oldError;
        }
#ifdef QT_DEBUG
        qDebug() << "Changed error to" << d->error;
#endif
        Q_EMIT errorChanged(error());
    }
}




AbstractConfiguration *Component::configuration() const { Q_D(const Component); return d->configuration; }

void Component::setConfiguration(AbstractConfiguration *nAbstractConfiguration)
{
    Q_D(Component);
    if (nAbstractConfiguration != d->configuration) {
        d->configuration = nAbstractConfiguration;
#ifdef QT_DEBUG
        qDebug() << "Changed configuration to" << d->configuration;
#endif
        Q_EMIT configurationChanged(configuration());
    }
}



AbstractStorage *Component::storage() const { Q_D(const Component); return d->storage; }

void Component::setStorage(AbstractStorage *localStorage)
{
    Q_D(Component);
    if (localStorage != d->storage) {
        d->storage = localStorage;
#ifdef QT_DEBUG
        qDebug() << "Changed storage to" << d->storage;
#endif
        Q_EMIT storageChanged(storage());
    }
}





void Component::setExpectedJSONType(ExpectedJSONType type)
{
    Q_D(Component);
    d->expectedJSONType = type;
}



void Component::setApiRoute(const QString &route)
{
    Q_D(Component);
    d->apiRoute = route;
}



void Component::setApiRoute(const QStringList &routeParts)
{
    Q_D(Component);
    d->apiRoute = QStringLiteral("/").append(routeParts.join(QChar('/')));
}




QJsonDocument Component::jsonResult() const
{
    Q_D(const Component);
    return d->jsonResult;
}


void Component::setNetworkOperation(QNetworkAccessManager::Operation operation)
{
    Q_D(Component);
    d->namOperation = operation;
}



QHash<QByteArray, QByteArray> Component::requestHeaders() const
{
    Q_D(const Component);
    return d->requestHeaders;
}


void Component::setRequestHeaders(const QHash<QByteArray, QByteArray> &headers)
{
    Q_D(Component);
    d->requestHeaders = headers;
}


void Component::addRequestHeader(const QByteArray &headerName, const QByteArray &headerValue)
{
    if (headerName.isEmpty() || headerValue.isEmpty()) {
        return;
    }

    Q_D(Component);
    d->requestHeaders.insert(headerName, headerValue);
}



void Component::addRequestHeaders(const QHash<QByteArray, QByteArray> &headers)
{
    if (headers.isEmpty()) {
        return;
    }

    Q_D(Component);

    QHash<QByteArray, QByteArray>::const_iterator i = headers.constBegin();
    while (i != headers.constEnd()) {
        d->requestHeaders.insert(i.key(), i.value());
        ++i;
    }
}



void Component::setPayload(const QByteArray &payload)
{
    Q_D(Component);
    d->payload = payload;
}


void Component::setPayload(const QJsonObject &payload)
{
    Q_D(Component);
    d->payload = QJsonDocument(payload).toJson();
}



void Component::setUrlQuery(const QUrlQuery &query)
{
    Q_D(Component);
    d->urlQuery = query;
}



void Component::setRequiresAuth(bool reqAuth)
{
    Q_D(Component);
    d->requiresAuth = reqAuth;
}
