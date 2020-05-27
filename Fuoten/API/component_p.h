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

#ifndef FUOTENCOMPONENT_P_H
#define FUOTENCOMPONENT_P_H

#include "component.h"
#include "../Helpers/abstractconfiguration.h"
#include "../Storage/abstractstorage.h"
#include "../Helpers/abstractnamfactory.h"
#include <QTimer>
#include <QNetworkReply>
#include <QNetworkRequest>

namespace Fuoten {

class ComponentPrivate
{
public:
    ComponentPrivate();
    virtual ~ComponentPrivate();

    QHash<QByteArray, QByteArray> requestHeaders;
    QString baseRoute = QStringLiteral("/index.php/apps/news/api/v1-2");
    QString apiRoute;
    QByteArray result;
    QByteArray payload;
    QJsonDocument jsonResult;
    QUrlQuery urlQuery;
    QNetworkAccessManager *networkAccessManager = nullptr;
    Error *error = nullptr;
    AbstractConfiguration *configuration = nullptr;
    AbstractStorage *storage = nullptr;
    AbstractNotificator *notificator = nullptr;
    QTimer *timeoutTimer = nullptr;
    QNetworkReply *reply = nullptr;
    QNetworkAccessManager::Operation namOperation = QNetworkAccessManager::GetOperation;
    quint16 requestTimeout = 300;
    quint8 retryCount;
    Component::ExpectedJSONType expectedJSONType = Component::Empty;
    bool requiresAuth = true;
    bool inOperation = false;
    bool useStorage = true;

    void performNetworkOperation(const QNetworkRequest &request);
    static AbstractConfiguration *defaultConfiguration();
    static void setDefaultConfiguration(AbstractConfiguration *config);
    static AbstractStorage *defaultStorage();
    static void setDefaultStorage(AbstractStorage *storage);
    static AbstractNamFactory *networkAccessManagerFactory();
    static void setNetworkAccessManagerFactory(AbstractNamFactory *factory);
    static AbstractNotificator *defaultNotificator();
    static void setDefaultNotificator(AbstractNotificator *notificator);

private:
    Q_DISABLE_COPY(ComponentPrivate)
};

}

#endif // FUOTENCOMPONENT_P_H
