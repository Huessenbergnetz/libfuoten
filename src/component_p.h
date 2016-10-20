/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016 Buschtrommel / Matthias Fehring
 * https://www.buschmann23.de/entwicklung/bibliotheken/libfuoten/
 * https://github.com/Buschtrommel/libfuoten
 *
 * component_p.h
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

#ifndef COMPONENT_P_H
#define COMPONENT_P_H

#include "component.h"
#include "Helpers/configuration.h"
#include "Helpers/storagehandler.h"
#include <QTimer>
#include <QNetworkReply>
#include <QNetworkRequest>

namespace Fuoten {

class ComponentPrivate
{
public:
    ComponentPrivate() :
        networkAccessManager(nullptr),
        inOperation(false),
        requestTimeout(120),
        error(nullptr),
        configuration(nullptr),
        storageHandler(nullptr),
        timeoutTimer(nullptr),
        reply(nullptr),
        namOperation(QNetworkAccessManager::GetOperation),
        expectedJSONType(Component::Empty),
        requiresAuth(true)
    {}

    virtual ~ComponentPrivate() {}

    void performNetworkOperation(const QNetworkRequest &request)
    {
        switch(namOperation) {
        case QNetworkAccessManager::HeadOperation:
            reply = networkAccessManager->head(request);
            break;
        case QNetworkAccessManager::PostOperation:
            reply = networkAccessManager->post(request, payload);
            break;
        case QNetworkAccessManager::PutOperation:
            reply = networkAccessManager->put(request, payload);
            break;
        case QNetworkAccessManager::DeleteOperation:
            reply = networkAccessManager->deleteResource(request);
            break;
        default:
            reply = networkAccessManager->get(request);
            break;
        }
    }

    QNetworkAccessManager *networkAccessManager;
    bool inOperation;
    quint8 requestTimeout;
    Error *error;
    Configuration *configuration;
    StorageHandler *storageHandler;

    quint8 retryCount;
    QHash<QByteArray, QByteArray> requestHeaders;
    QByteArray payload;
    QUrlQuery urlQuery;
    QTimer *timeoutTimer;
    QNetworkReply *reply;
    QNetworkAccessManager::Operation namOperation;
    Component::ExpectedJSONType expectedJSONType;
    QString apiRoute;
    QJsonDocument jsonResult;
    QByteArray result;
    bool requiresAuth;
};

}

#endif // COMPONENT_P_H
