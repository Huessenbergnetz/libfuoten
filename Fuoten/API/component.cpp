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
#include <QReadWriteLock>
#include <QGlobalStatic>

using namespace Fuoten;


class DefaultValues
{
public:
    mutable QReadWriteLock lock;

    AbstractConfiguration *configuration() const
    {
        return m_defaultConfig;
    }

    void setConfiguration(AbstractConfiguration *config)
    {
        m_defaultConfig = config;
    }

    AbstractStorage *storage() const
    {
        return m_defaultStorage;
    }

    void setStorage(AbstractStorage *storage)
    {
        m_defaultStorage = storage;
    }

    AbstractNamFactory *networkAccessManagerFactory() const
    {
        return m_namFactory;
    }

    void setNetworkAccessManagerFactory(AbstractNamFactory *factory)
    {
        m_namFactory = factory;
    }

    AbstractNotificator *notificator() const
    {
        return m_defaultNotificator;
    }

    void setNotificator(AbstractNotificator *notificator) {
        m_defaultNotificator = notificator;
    }

private:
    AbstractConfiguration *m_defaultConfig = nullptr;
    AbstractStorage *m_defaultStorage = nullptr;
    AbstractNamFactory *m_namFactory = nullptr;
    AbstractNotificator *m_defaultNotificator = nullptr;
};
Q_GLOBAL_STATIC(DefaultValues, defVals)


AbstractConfiguration *ComponentPrivate::defaultConfiguration()
{
    const DefaultValues *defs = defVals();
    Q_ASSERT(defs);

    defs->lock.lockForRead();
    AbstractConfiguration *config = defs->configuration();
    defs->lock.unlock();

    return config;
}


void ComponentPrivate::setDefaultConfiguration(AbstractConfiguration *config)
{
    qDebug("Setting default configuration to %p.", config);
    DefaultValues *defs = defVals();
    Q_ASSERT(defs);
    QWriteLocker locker(&defs->lock);

    defs->setConfiguration(config);
}


AbstractStorage *ComponentPrivate::defaultStorage()
{
    const DefaultValues *defs = defVals();
    Q_ASSERT(defs);

    defs->lock.lockForRead();
    AbstractStorage *storage = defs->storage();
    defs->lock.unlock();

    return storage;
}


void ComponentPrivate::setDefaultStorage(AbstractStorage *storage)
{
    qDebug("Setting default storage to %p.", storage);
    DefaultValues *defs = defVals();
    Q_ASSERT(defs);
    QWriteLocker locker(&defs->lock);

    defs->setStorage(storage);
}


AbstractNamFactory *ComponentPrivate::networkAccessManagerFactory()
{
    const DefaultValues *defs = defVals();
    Q_ASSERT(defs);

    defs->lock.lockForRead();
    AbstractNamFactory *nam = defs->networkAccessManagerFactory();
    defs->lock.unlock();

    return nam;
}


void ComponentPrivate::setNetworkAccessManagerFactory(AbstractNamFactory *factory)
{
    qDebug("Setting network access manager factory to %p.", factory);
    DefaultValues *defs = defVals();
    Q_ASSERT(defs);
    QWriteLocker locker(&defs->lock);

    defs->setNetworkAccessManagerFactory(factory);
}


AbstractNotificator *ComponentPrivate::defaultNotificator()
{
    const DefaultValues *defs = defVals();
    Q_ASSERT(defs);

    defs->lock.lockForRead();
    AbstractNotificator *noti = defs->notificator();
    defs->lock.unlock();

    return noti;
}


void ComponentPrivate::setDefaultNotificator(AbstractNotificator *notificator)
{
    qDebug("Settings default notificator to %p.", notificator);
    DefaultValues *defs = defVals();
    Q_ASSERT(defs);
    QWriteLocker locker(&defs->lock);

    defs->setNotificator(notificator);
}


Component::Component(QObject *parent) :
    QObject(parent), d_ptr(new ComponentPrivate)
{
    connect(this, &Component::failed, [this](Error *e){notify(e);});
}


Component::Component(ComponentPrivate &dd, QObject *parent) :
    QObject(parent), d_ptr(&dd)
{
    connect(this, &Component::failed, [this](Error *e){notify(e);});
}


Component::~Component()
{
}


void Component::sendRequest()
{
    Q_D(Component);

    if (!d->configuration) {
        setConfiguration(ComponentPrivate::defaultConfiguration());
    }

    Q_ASSERT_X(d->configuration, "send request", "no configuration available");
    Q_ASSERT_X(!d->configuration->getHost().isEmpty(), "send request", "empty host name");
    Q_ASSERT_X(!d->apiRoute.isEmpty(), "send request", "empty API route");

    setError(nullptr);

    d->result.clear();
    d->jsonResult = QJsonDocument();

    if (Q_UNLIKELY(!checkInput())) {
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
    urlPath.append(QLatin1String("/index.php/apps/news/api/v1-2"));
    urlPath.append(d->apiRoute);

    url.setPath(urlPath);

    if (!d->urlQuery.isEmpty()) {
        url.setQuery(d->urlQuery);
    }

    Q_ASSERT_X(url.isValid(), "send request", "invalid API URL");

    if (!d->networkAccessManager) {
        AbstractNamFactory *namf = Component::networkAccessManagerFactory();
        if (namf) {
            d->networkAccessManager = namf->create(this);
        } else {
            d->networkAccessManager = new QNetworkAccessManager(this);
        }
        if (d->configuration->getIgnoreSSLErrors()) {
            connect(d->networkAccessManager, &QNetworkAccessManager::sslErrors, this, &Component::_ignoreSSLErrors);
        }
    }

    QNetworkRequest nr(url);

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
    qDebug("Start performing network operation.");
    qDebug("API URL: %s", qUtf8Printable(url.toString()));
    if (!nr.rawHeaderList().empty()) {
        const QList<QByteArray> hl = nr.rawHeaderList();
        for (const QByteArray &h : hl) {
            if (h != QByteArrayLiteral("Authorization")) {
                qDebug("%s: %s", h.constData(), nr.rawHeader(h).constData());
            }
        }
    }
    if (!d->payload.isEmpty()) {
        qDebug("Paylod: %s", d->payload.constData());
    }
#endif

    if (Q_LIKELY(d->requestTimeout > 0)) {
        if (!d->timeoutTimer) {
            d->timeoutTimer = new QTimer(this);
            qDebug("Created new timeout timer at %p.", d->timeoutTimer);
            d->timeoutTimer->setSingleShot(true);
            d->timeoutTimer->setTimerType(Qt::VeryCoarseTimer);
            connect(d->timeoutTimer, &QTimer::timeout, this, &Component::_requestTimedOut);
        }
        d->timeoutTimer->start(d->requestTimeout * 1000);
        qDebug("Started timeout timer with %u seconds.", d->requestTimeout);
    }

    d->performNetworkOperation(nr);
    Q_CHECK_PTR(d->reply);
    if (!connect(d->reply, &QNetworkReply::finished, this, &Component::_requestFinished)) {
        qFatal("Failed to connect QNetworkReply to Component::_requestFinished slot.");
    }
}


void Component::_requestFinished()
{
    Q_D(Component);

    if (Q_LIKELY(d->timeoutTimer && d->timeoutTimer->isActive())) {
        qDebug("Stopping timeout timer with %i seconds left.", d->timeoutTimer->remainingTime()/1000);
        d->timeoutTimer->stop();
    }

    qDebug("%s", "Reading network reply data.");
    d->result = d->reply->readAll();

    if (Q_LIKELY(d->reply->error() == QNetworkReply::NoError)) {

        if (checkOutput()) {
            qDebug("%s", "Calling successCallback().");
            successCallback();
        } else {
            setInOperation(false);
        }

    } else {
        qDebug("%s", "Extracting error data from network reply.");
        extractError(d->reply);
        setInOperation(false);
    }

    d->reply->deleteLater();
    d->reply = nullptr;
}


void Component::extractError(QNetworkReply *reply)
{
    Q_ASSERT_X(reply, "extract error", "invalid QNetworkReply");

    setError(new Error(reply, this));

    setInOperation(false);
    Q_EMIT failed(error());
}


void Component::_requestTimedOut()
{
    Q_D(Component);

    //% "The connection to the server timed out after %n second(s)."
    setError(new Error(Error::RequestError, Error::Critical, qtTrId("err-conn-timeout", requestTimeout()), d->reply->request().url().toString(), this));

    setInOperation(false);

    QNetworkReply *nr = d->reply;
    d->reply = nullptr;
    delete nr;
    Q_EMIT failed(error());
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

    if (Q_UNLIKELY(d->requiresAuth && (d->configuration->getUsername().isEmpty() || d->configuration->getPassword().isEmpty()))) {
        //% "You have to specify a username and a password."
        setError(new Error(Error::AuthorizationError, Error::Critical, qtTrId("err-username-pass-missing"), QString(), this));
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

    if (Q_UNLIKELY((d->expectedJSONType != Empty) && (d->jsonResult.isNull() || d->jsonResult.isEmpty()))) {
        //% "The request replied an empty answer, but there was content expected."
        setError(new Error(Error::OutputError, Error::Critical, qtTrId("err-empty-answer"), QString(), this));
        Q_EMIT failed(error());
        return false;
    }

    if (Q_UNLIKELY((d->expectedJSONType == Array) && !d->jsonResult.isArray())) {
        //% "It was expected that the request returns a JSON array, but it returned something else."
        setError(new Error(Error::OutputError, Error::Critical, qtTrId("err-no-json-array"), QString(), this));
        Q_EMIT failed(error());
        return false;
    }

    if (Q_UNLIKELY((d->expectedJSONType == Object && !d->jsonResult.isObject()))) {
        //% "It was expected that the request returns a JSON object, but it returned something else."
        setError(new Error(Error::OutputError, Error::Critical, qtTrId("err-no-json-object"), QString(), this));
        Q_EMIT failed(error());
        return false;
    }

    return true;
}


bool Component::inOperation() const { Q_D(const Component); return d->inOperation; }

void Component::setInOperation(bool nInOperation)
{
    Q_D(Component); 
    if (nInOperation != d->inOperation) {
        d->inOperation = nInOperation;
        qDebug("Changed inOperation to %s.", d->inOperation ? "true" : "false");
        Q_EMIT inOperationChanged(inOperation());
    }
}


quint8 Component::requestTimeout() const { Q_D(const Component); return d->requestTimeout; }

void Component::setRequestTimeout(quint8 seconds)
{
    Q_D(Component); 
    if (seconds != d->requestTimeout) {
        d->requestTimeout = seconds;
        qDebug("Changed requestTimeout to %u seconds.", d->requestTimeout);
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
        qDebug("Changed error to %p.", d->error);
        Q_EMIT errorChanged(error());
    }
}


AbstractConfiguration *Component::configuration() const
{
    Q_D(const Component);
    AbstractConfiguration *_config = d->configuration;
    if (!_config) {
        _config = ComponentPrivate::defaultConfiguration();
    }
    Q_ASSERT(_config);
    return _config;
}


void Component::setConfiguration(AbstractConfiguration *nAbstractConfiguration)
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Can not change property %s, still in operation.", "configuration");
        return;
    }

    Q_D(Component);
    if (nAbstractConfiguration != d->configuration) {
        d->configuration = nAbstractConfiguration;
        qDebug("Changed configuration to %p.", d->configuration);
        Q_EMIT configurationChanged(configuration());
    }
}


AbstractStorage *Component::storage() const
{
    Q_D(const Component);
    AbstractStorage *_storage = d->storage;
    if (!_storage) {
        _storage = ComponentPrivate::defaultStorage();
    }
    return _storage;
}

void Component::setStorage(AbstractStorage *localStorage)
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Can not change property %s, still in operation.", "storage");
        return;
    }

    Q_D(Component);
    if (localStorage != d->storage) {
        d->storage = localStorage;
        qDebug("Changed storage to %p.", d->storage);
        Q_EMIT storageChanged(storage());
    }
}


bool Component::isUseStorageEnabled() const { Q_D(const Component); return d->useStorage; }

void Component::setUseStorage(bool useStorage)
{
    Q_D(Component);
    if (useStorage != d->useStorage) {
        d->useStorage = useStorage;
        qDebug("Changed useStorage to %s.", d->useStorage ? "true" : "false");
        Q_EMIT useStorageChanged(d->useStorage);
    }
}


AbstractNotificator *Component::notificator() const
{
    Q_D(const Component);
    AbstractNotificator *_notificator = d->notificator;
    if (!_notificator) {
        _notificator = ComponentPrivate::defaultNotificator();
    }
    return _notificator;
}

void Component::setNotificator(AbstractNotificator *notificator)
{
    Q_D(Component);
    if (notificator != d->notificator) {
        d->notificator = notificator;
        qDebug("Changed notificator to %p.", d->notificator);
        Q_EMIT notificatorChanged(d->notificator);
    }
}


void Component::setDefaultConfiguration(AbstractConfiguration *config)
{
    ComponentPrivate::setDefaultConfiguration(config);
}


AbstractConfiguration *Component::defaultConfiguration()
{
    return ComponentPrivate::defaultConfiguration();
}


void Component::setDefaultStorage(AbstractStorage *storage)
{
    ComponentPrivate::setDefaultStorage(storage);
}


AbstractStorage *Component::defaultStorage()
{
    return ComponentPrivate::defaultStorage();
}


void Component::setNetworkAccessManagerFactory(AbstractNamFactory *factory)
{
    ComponentPrivate::setNetworkAccessManagerFactory(factory);
}


AbstractNamFactory *Component::networkAccessManagerFactory()
{
    return ComponentPrivate::networkAccessManagerFactory();
}


void Component::setDefaultNotificator(AbstractNotificator *notificator)
{
    ComponentPrivate::setDefaultNotificator(notificator);
}


AbstractNotificator *Component::defaultNotificator()
{
    return ComponentPrivate::defaultNotificator();
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
    d->payload = QJsonDocument(payload).toJson(QJsonDocument::Compact);
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


void Component::notify(const Fuoten::Error *e) const
{
    Q_D(const Component);
    if (d->notificator) {
        d->notificator->notify(e);
    }
}


void Component::notify(AbstractNotificator::Type type, QtMsgType severity, const QVariant &data) const
{
    Q_D(const Component);
    if (d->notificator) {
        d->notificator->notify(type, severity, data);
    }
}

#include "moc_component.cpp"
