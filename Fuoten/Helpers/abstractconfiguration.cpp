/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016-2019 Hüssenbergnetz/Matthias Fehring
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

#include "abstractconfiguration.h"
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QDebug>

using namespace Fuoten;

AbstractConfiguration::AbstractConfiguration(QObject *parent) : QSettings(parent)
{

}


AbstractConfiguration::~AbstractConfiguration()
{

}


void AbstractConfiguration::checkAccountValidity()
{
    setIsAccountValid((!getUsername().isEmpty() && !getPassword().isEmpty() && !getHost().isEmpty() && !getServerVersion().isNull()));
}


void AbstractConfiguration::setUsername(const QString &username)
{
    Q_UNUSED(username)
}


void AbstractConfiguration::setPassword(const QString &password)
{
    Q_UNUSED(password)
}


QString AbstractConfiguration::getUserAgent() const
{
    return QStringLiteral("Libfuoten %1").arg(QStringLiteral(VERSION_STRING));
}


bool AbstractConfiguration::getUseSSL() const
{
    return true;
}


void AbstractConfiguration::setUseSSL(bool useSSL)
{
    Q_UNUSED(useSSL)
}


void AbstractConfiguration::setHost(const QString &host)
{
    Q_UNUSED(host)
}


void AbstractConfiguration::setInstallPath(const QString &path)
{
    Q_UNUSED(path)
}


int AbstractConfiguration::getServerPort() const
{
    return 0;
}


void AbstractConfiguration::setServerPort(int port)
{
    Q_UNUSED(port)
}


bool AbstractConfiguration::getIgnoreSSLErrors() const
{
    return false;
}


void AbstractConfiguration::setAvatar(const QString &data, const QString &mime)
{
    Q_UNUSED(data)
    Q_UNUSED(mime)
}


void AbstractConfiguration::setDisplayName(const QString &nDisplayName)
{
    Q_UNUSED(nDisplayName)
}


void AbstractConfiguration::setImproperlyConfiguredCron(bool nImproperlyConfiguredCron)
{
    Q_UNUSED(nImproperlyConfiguredCron)
}



void AbstractConfiguration::setLastSync(const QDateTime &syncTime)
{
    Q_UNUSED(syncTime);
}


QDateTime AbstractConfiguration::getLastSync() const
{
    return QDateTime();
}


FuotenEnums::ItemDeletionStrategy AbstractConfiguration::getPerFeedDeletionStrategy(qint64 feedId) const
{
    Q_UNUSED(feedId)
    return FuotenEnums::NoItemDeletion;
}


quint16 AbstractConfiguration::getPerFeedDeletionValue(qint64 feedId) const
{
    Q_UNUSED(feedId)
    return 0;
}


QString AbstractConfiguration::getLoginFlowUserAgent() const
{
    return getUserAgent();
}


bool AbstractConfiguration::setLoginFlowCredentials(const QUrl &credentialUrl)
{
    Q_UNUSED(credentialUrl)
    return false;
}


bool AbstractConfiguration::setLoginFlowCredentials(const QByteArray &credentials)
{
    if (Q_UNLIKELY(credentials.isEmpty())) {
        qCritical("%s", "Failed to set login flow credentials from empty data.");
        return false;
    }

    QJsonParseError jsonError;
    const QJsonDocument json = QJsonDocument::fromJson(credentials, &jsonError);

    if (Q_UNLIKELY(jsonError.error != QJsonParseError::NoError)) {
        qCritical("Failed to parse the login flow credential JSON data: %s", qUtf8Printable(jsonError.errorString()));
        return false;
    }

    return setLoginFlowCredentials(json);
}


bool AbstractConfiguration::setLoginFlowCredentials(const QJsonDocument &credentials)
{
    if (Q_UNLIKELY(credentials.isEmpty() || credentials.isNull())) {
        qCritical("%s", "Failed to set login flow credentials from empty JSON document.");
        return false;
    }

    if (Q_UNLIKELY(!credentials.isObject())) {
        qCritical("%s", "Failed to set login flow credentials from a non JSON object.");
        return false;
    }

    return setLoginFlowCredentials(credentials.object());
}


bool AbstractConfiguration::setLoginFlowCredentials(const QJsonObject &credentials)
{
    if (Q_UNLIKELY(credentials.empty())) {
        qCritical("%s", "Failed to set login flow credentials from empty JSON object.");
        return false;
    }

    qDebug() << "Login Flow return value:" << credentials;

    const QUrl serverUrl = QUrl(credentials.value(QStringLiteral("server")).toString(), QUrl::StrictMode);
    if (Q_UNLIKELY(!setServerUrl(serverUrl))) {
        qCritical("Failed to set login flow credentials with invalid server URL: %s", qUtf8Printable(credentials.value(QStringLiteral("server")).toString()));
        return false;
    }

    const QString loginName = credentials.value(QStringLiteral("loginName")).toString();
    if (Q_UNLIKELY(loginName.isEmpty())) {
        qCritical("%s", "Failed to set login flow credentials with empty login name.");
        return false;
    }

    const QString appPassword = credentials.value(QStringLiteral("appPassword")).toString();
    if (Q_UNLIKELY(appPassword.isEmpty())) {
        qCritical("%s", "Failed to set login flow credentials with empty application apssword.");
        return false;
    }

    setUsername(loginName);
    setPassword(appPassword);

    setIsAccountValid(true);

    return true;
}

bool AbstractConfiguration::setServerUrl(const QUrl &url)
{
    if (Q_UNLIKELY(!url.isValid())) {
        qCritical("%s", "Invalid server URL.");
        return false;
    }

    bool ssl = true;
    if (url.scheme().isEmpty() || url.scheme() == QLatin1String("https")) {
        ssl = true;
    } else if (url.scheme() == QLatin1String("http")) {
        ssl = false;
    } else {
        qCritical("Invalid server URL scheme: %s", qUtf8Printable(url.scheme()));
        return false;
    }

    const QString host = url.host();
    if (Q_UNLIKELY(host.isEmpty())) {
        qCritical("%s", "Empty server host name.");
        return false;
    }

    int port = url.port(0);

    QString path = url.path();
    while (path.endsWith(QLatin1Char('/'))) {
        path.chop(1);
    }

    setUseSSL(ssl);
    setHost(host);
    setServerPort(port);
    setInstallPath(path);

    return true;
}

bool AbstractConfiguration::setServerUrl(const QString &url)
{
    return setServerUrl(QUrl(url, QUrl::StrictMode));
}

#include "moc_abstractconfiguration.cpp"
