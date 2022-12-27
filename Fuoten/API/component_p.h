/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENCOMPONENT_P_H
#define FUOTENCOMPONENT_P_H

#include "component.h"
#include "../Helpers/abstractconfiguration.h"
#include "../Storage/abstractstorage.h"
#include "../Helpers/abstractnamfactory.h"
#include "../Helpers/wipemanager.h"
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
    QByteArray payloadContentType = QByteArrayLiteral("application/json");
    QJsonDocument jsonResult;
    QUrlQuery urlQuery;
    QNetworkAccessManager *networkAccessManager = nullptr;
    Error *error = nullptr;
    AbstractConfiguration *configuration = nullptr;
    AbstractStorage *storage = nullptr;
    AbstractNotificator *notificator = nullptr;
    WipeManager *wipeManager = nullptr;
#if (QT_VERSION < QT_VERSION_CHECK(5, 15, 0))
    QTimer *timeoutTimer = nullptr;
#endif
    QNetworkReply *reply = nullptr;
    QNetworkAccessManager::Operation namOperation = QNetworkAccessManager::GetOperation;
    quint16 requestTimeout = 300;
    quint8 retryCount;
    Component::ExpectedJSONType expectedJSONType = Component::Empty;
    bool requiresAuth = true;
    bool inOperation = false;
    bool useStorage = true;
    bool checkForWipe = true;

    void performNetworkOperation(const QNetworkRequest &request);
    static AbstractConfiguration *defaultConfiguration();
    static void setDefaultConfiguration(AbstractConfiguration *config);
    static AbstractStorage *defaultStorage();
    static void setDefaultStorage(AbstractStorage *storage);
    static AbstractNamFactory *networkAccessManagerFactory();
    static void setNetworkAccessManagerFactory(AbstractNamFactory *factory);
    static AbstractNotificator *defaultNotificator();
    static void setDefaultNotificator(AbstractNotificator *notificator);
    static WipeManager *defaultWipeManager();
    static void setDefaultWipeManager(WipeManager *wipeManager);
    static QNetworkAccessManager *defaultNam();
    static void setDefaultNam(QNetworkAccessManager *nam);

private:
    Q_DISABLE_COPY(ComponentPrivate)
};

}

#endif // FUOTENCOMPONENT_P_H
