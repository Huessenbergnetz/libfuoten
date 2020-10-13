/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016-2020 Hüssenbergnetz/Matthias Fehring
 * https://github.com/Huessenbergnetz/libfuoten
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

#include "loginflowv2_p.h"
#include "../error.h"
#include <QJsonObject>

using namespace Fuoten;

LoginFlowV2Private::LoginFlowV2Private(LoginFlowV2 *q) :
    ComponentPrivate(), q_ptr(q)
{
    baseRoute = QString();
    apiRoute = QStringLiteral("/index.php/login/v2");
    requiresAuth = false;
    useStorage = false;
    checkForWipe = false;
    expectedJSONType = Component::Object;
    namOperation = QNetworkAccessManager::PostOperation;
}

LoginFlowV2Private::~LoginFlowV2Private()
{

}

LoginFlowV2::LoginFlowV2(QObject *parent) :
    Component(* new LoginFlowV2Private(this), parent)
{

}

LoginFlowV2::LoginFlowV2(LoginFlowV2Private &dd, QObject *parent) :
    Component(dd, parent)
{

}

LoginFlowV2::~LoginFlowV2()
{

}

void LoginFlowV2::execute()
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("%s", "Still in operation. Returning.");
        return;
    }

    qDebug("%s", "Start requesting Login Flow v2 token and endpoint.");

    if (!configuration()) {
        setConfiguration(Component::defaultConfiguration());
    }
    Q_ASSERT_X(configuration(), "send request", "no configuration available");

    setInOperation(true);

    addRequestHeader(QByteArrayLiteral("User-Agent"), configuration()->getLoginFlowUserAgent().toUtf8());

    sendRequest();
}

bool LoginFlowV2::checkInput()
{
    return true;
}

void LoginFlowV2::successCallback()
{
    const QJsonObject o = jsonResult().object();
    const QUrl login = QUrl(o.value(QStringLiteral("login")).toString());
    if (!login.isValid()) {
        qCritical("Invalid login flow v2 login url: %s", qUtf8Printable(o.value(QStringLiteral("login")).toString()));
        //% "The login URL returned by the Nextcloud API is invalid."
        setError(new Error(Error::OutputError, Error::Critical, qtTrId("libfuoten-err-loginflowv2-invalid-login-url"), o.value(QStringLiteral("login")).toString(), this));
        setInOperation(false);
        Q_EMIT failed(error());
        return;
    }

    Q_D(LoginFlowV2);

    const QJsonObject pollObject = o.value(QStringLiteral("poll")).toObject();
    const QString pollingToken = pollObject.value(QStringLiteral("token")).toString();
    if (pollingToken.isEmpty()) {
        qCritical("%s", "Empty login flow v2 polling token.");
        //% "The Nextcloud API returned an empty polling token."
        setError(new Error(Error::OutputError, Error::Critical, qtTrId("liburoten-err-loginflowv2-empty-token"), QString(), this));
        setInOperation(false);
        Q_EMIT failed(error());
        return;
    }
    const QUrl pollingEndpoint = QUrl(pollObject.value(QStringLiteral("endpoint")).toString());
    if (!pollingEndpoint.isValid()) {
        qCritical("Invalid login flow v2 polling endpoint: %s", qUtf8Printable(pollObject.value(QStringLiteral("endpoint")).toString()));
        //% "The Nextcloud API returned an invalid polling URL."
        setError(new Error(Error::OutputError, Error::Critical, qtTrId("libfuoten-err-loginflowv2-invalid-polling-url"), pollObject.value(QStringLiteral("endpoint")).toString(), this));
        setInOperation(false);
        Q_EMIT failed(error());
        return;
    }

    Q_EMIT gotLoginUrl(login);

    qDebug("Got valid login flow v2 data. Login URL: %s, Polling URL: %s, Polling Token: %s", qUtf8Printable(login.toString()), qUtf8Printable(pollingEndpoint.toString()), qUtf8Printable(pollingToken));
    qDebug("%s", "Staring polling.");

    d->startPolling(pollingEndpoint, pollingToken);
}

int LoginFlowV2::secondsLeft() const
{
    Q_D(const LoginFlowV2);
    if (!d->overallTimeoutTimer) {
        return 0;
    } else {
        return d->overallTimeoutTimer->remainingTime() / 1000;
    }
}

int LoginFlowV2::pollingInterval() const
{
    Q_D(const LoginFlowV2);
    return d->pollingInterval;
}

void LoginFlowV2::setPollingInterval(int pollingInterval)
{
    Q_D(LoginFlowV2);
    if (d->pollingInterval != pollingInterval) {
        qDebug("Changing pollingInterval from %i to %i seconds.", d->pollingInterval, pollingInterval);
        d->pollingInterval = pollingInterval;
        Q_EMIT pollingIntervalChanged(d->pollingInterval);
    }
}

int LoginFlowV2::pollingTimeout() const
{
    Q_D(const LoginFlowV2);
    return d->pollingTimeout;
}

void LoginFlowV2::setPollingTimeout(int pollingTimeout)
{
    Q_D(LoginFlowV2);

    if (d->inOperation) {
        qWarning("Can not change property %s, still in operation.", "pollingTimeout");
        return;
    }

    if (d->pollingTimeout != pollingTimeout) {
        qDebug("Changing pollingTimeout from %i to %i seconds.", d->pollingTimeout, pollingTimeout);
        d->pollingTimeout = pollingTimeout;
        Q_EMIT pollingTimeoutChanged(d->pollingTimeout);
    }
}

int LoginFlowV2::pollingRequestTimeout() const
{
    Q_D(const LoginFlowV2);
    return d->pollingRequestTimeout;
}

void LoginFlowV2::setPollingRequestTimeout(int pollingRequestTimeout)
{
    Q_D(LoginFlowV2);

    if (d->pollingRequestTimeout != pollingRequestTimeout) {
        qDebug("Changing pollingRequestTimeout from %i to %i seconds.", d->pollingRequestTimeout, pollingRequestTimeout);
        d->pollingRequestTimeout = pollingRequestTimeout;
        Q_EMIT pollingRequestTimeoutChanged(d->pollingRequestTimeout);
    }
}

void LoginFlowV2Private::startPolling(const QUrl &endpoint, const QString &token)
{
    pollingRequest.setUrl(endpoint);

    const QString tokenData = QLatin1String("token=") + token;
    pollingData = tokenData.toUtf8();

    pollingRequest.setRawHeader(QByteArrayLiteral("Content-Length"), QByteArray::number(pollingData.length()));
    pollingRequest.setRawHeader(QByteArrayLiteral("Content-Type"), QByteArrayLiteral("application/x-www-form-urlencoded"));

    timeoutsInARow = 0;

    Q_Q(LoginFlowV2);

    if (!overallTimeoutTimer) {
        overallTimeoutTimer = new QTimer(q);
        overallTimeoutTimer->setSingleShot(true);
        overallTimeoutTimer->setTimerType(Qt::VeryCoarseTimer);
        QObject::connect(overallTimeoutTimer, &QTimer::timeout, q, [this](){ overallTimerTimedOut(); });
    }
    overallTimeoutTimer->start(pollingTimeout * 1000);

    if (!secondsLeftTimer) {
        secondsLeftTimer = new QTimer(q);
        secondsLeftTimer->setSingleShot(false);
        secondsLeftTimer->setTimerType(Qt::VeryCoarseTimer);
        secondsLeftTimer->setInterval(1000);
        QObject::connect(secondsLeftTimer, &QTimer::timeout, q, [this](){ emitSecondsLeft(); });
    }

    secondsLeftTimer->start();

    sendPollingRequest();
}

void LoginFlowV2Private::sendPollingRequest()
{
    if (!overallTimeoutTimer->isActive()) {
        return;
    }

    qDebug("%s", "Sending polling request.");

    Q_Q(LoginFlowV2);

    if (!pollingRequestTimeoutTimer) {
        pollingRequestTimeoutTimer = new QTimer(q);
        pollingRequestTimeoutTimer->setSingleShot(true);
        pollingRequestTimeoutTimer->setTimerType(Qt::VeryCoarseTimer);
        QObject::connect(pollingRequestTimeoutTimer, &QTimer::timeout, q, [this](){ pollingRequestTimedOut(); });
    }
    pollingRequestTimeoutTimer->start(pollingRequestTimeout * 1000);

    pollingReply = networkAccessManager->post(pollingRequest, pollingData);
    QObject::connect(pollingReply, &QNetworkReply::finished, q, [this](){ pollingRequestFinished(); });
}

void LoginFlowV2Private::pollingRequestFinished()
{
    qDebug("%s", "Polling request finished.");

    qDebug("Stopping polling request timeout timer with %i seconds left.", pollingRequestTimeoutTimer->remainingTime()/1000);
    pollingRequestTimeoutTimer->stop();

    timeoutsInARow = 0;

    QNetworkReply *pr = pollingReply;
    pollingReply = nullptr;

    Q_Q(LoginFlowV2);

    const int httpStatusCode = pr->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if (httpStatusCode == 404) {

        qDebug("Authentication information not yet available. Trying again in %i seconds.", pollingInterval);
        QTimer::singleShot(pollingInterval * 1000, q, [this](){ sendPollingRequest(); });

    } else if (httpStatusCode == 200) {

        qDebug("%s", "Authentication information available.");
        overallTimeoutTimer->stop();
        secondsLeftTimer->stop();
        const QByteArray data = pr->readAll();

        if (q->configuration()) {
            if (q->configuration()->setLoginFlowCredentials(data)) {
                QVariantMap notificationData{
                    {QStringLiteral("username"), q->configuration()->getUsername()}
                };
                QString host = q->configuration()->getHost();
                if (!q->configuration()->getInstallPath().isEmpty()) {
                    host += QLatin1Char('/') + q->configuration()->getInstallPath();
                }
                notificationData.insert(QStringLiteral("host"), host);
                q->notify(AbstractNotificator::AuthorizationSucceeded, QtInfoMsg, notificationData);
                Q_EMIT q->succeeded(QJsonDocument::fromJson(data));
            } else {
                //% "The returned login data is invalid."
                q->setError(new Error(Error::OutputError, Error::Critical, qtTrId("libfuoten-err-loginflowv2-invalid-auth-data"), QString(), q));
                Q_EMIT q->failed(q->error());
            }
        }

        q->setInOperation(false);

    } else {

        qDebug("%s", "Extracting error data from network reply.");
        overallTimeoutTimer->stop();
        if (secondsLeftTimer) {
            secondsLeftTimer->stop();
        }
        q->extractError(pr);
    }

    pr->deleteLater();
}

void LoginFlowV2Private::pollingRequestTimedOut()
{
    timeoutsInARow++;

    delete pollingReply;
    pollingReply = nullptr;

    Q_Q(LoginFlowV2);

    if (timeoutsInARow >= maxTimeoutsInARow) {
        overallTimeoutTimer->stop();
        q->setError(new Error(Error::RequestError, Error::Critical, qtTrId("err-conn-timeout", pollingRequestTimeout), pollingRequest.url().toString(), q));
        q->setInOperation(false);
        Q_EMIT q->failed(q->error());
    } else {
        qDebug("Polling request timed out after %i seconds. Trying again.", pollingRequestTimeout);
        sendPollingRequest();
    }
}

void LoginFlowV2Private::emitSecondsLeft()
{
    Q_CHECK_PTR(overallTimeoutTimer);

    Q_Q(LoginFlowV2);

    Q_EMIT q->secondsLeftChanged(overallTimeoutTimer->remainingTime() / 1000);
}

void LoginFlowV2Private::overallTimerTimedOut()
{
    delete pollingReply;
    pollingReply = nullptr;

    Q_Q(LoginFlowV2);
    q->setInOperation(false);
    //: error message in the login flow
    //% "The required authorisation message was not received within %n second(s)."
    q->setError(new Error(Error::RequestError, Error::Critical, qtTrId("libfuoten-err-loginflowv2-overall-timeout", pollingTimeout), QString(), q));
    Q_EMIT q->failed(q->error());
}

#include "moc_loginflowv2.cpp"
