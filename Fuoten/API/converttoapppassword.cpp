/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "converttoapppassword_p.h"
#include "../error.h"
#include <QJsonObject>
#include <QUrlQuery>

using namespace Fuoten;

ConvertToAppPasswordPrivate::ConvertToAppPasswordPrivate() :
    ComponentPrivate()
{
    baseRoute = QString();
    apiRoute = QStringLiteral("/ocs/v2.php/core/getapppassword");
    requiresAuth = true;
    useStorage = false;
    checkForWipe = false;
    expectedJSONType = Component::Object;
    namOperation = QNetworkAccessManager::GetOperation;
}

ConvertToAppPasswordPrivate::~ConvertToAppPasswordPrivate()
{

}

ConvertToAppPassword::ConvertToAppPassword(QObject *parent) :
    Component(* new ConvertToAppPasswordPrivate(), parent)
{

}

ConvertToAppPassword::ConvertToAppPassword(ConvertToAppPasswordPrivate &dd, QObject *parent) :
    Component(dd, parent)
{

}

ConvertToAppPassword::~ConvertToAppPassword()
{

}

void ConvertToAppPassword::execute()
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("%s", "Still in operation. Returning.");
        return;
    }

    qDebug("%s", "Start request to upgrade to application password.");

    if (!configuration()) {
        setConfiguration(Component::defaultConfiguration());
    }
    Q_ASSERT_X(configuration(), "send request", "no configuration available");

    setInOperation(true);

    addRequestHeader(QByteArrayLiteral("User-Agent"), configuration()->getLoginFlowUserAgent().toLatin1());
    addRequestHeader(QByteArrayLiteral("OCS-APIRequest"), QByteArrayLiteral("true"));

    QUrlQuery uq;
    uq.addQueryItem(QStringLiteral("format"), QStringLiteral("json"));
    setUrlQuery(uq);

    sendRequest();
}

void ConvertToAppPassword::successCallback()
{
    const QJsonObject ocs = jsonResult().object().value(QStringLiteral("ocs")).toObject();
    const QJsonObject meta = ocs.value(QStringLiteral("meta")).toObject();

    if (meta.value(QStringLiteral("status")).toString() == QLatin1String("ok") && meta.value(QStringLiteral("statuscode")).toInt() == 200) {
        const QString appPass = ocs.value(QStringLiteral("data")).toObject().value(QStringLiteral("apppassword")).toString();
        if (Q_LIKELY(!appPass.isEmpty())) {
            configuration()->setPassword(appPass);
            qDebug("%s", "Successfully upgraded to application password.");
            QVariantMap notificationData{
                {QStringLiteral("username"), configuration()->getUsername()},
                {QStringLiteral("useragent"), configuration()->getLoginFlowUserAgent()}
            };
            QString host = configuration()->getHost();
            if (!configuration()->getInstallPath().isEmpty()) {
                host += QLatin1Char('/') + configuration()->getInstallPath();
            }
            notificationData.insert(QStringLiteral("host"), host);
            notify(AbstractNotificator::AppPasswordRequested, QtInfoMsg, notificationData);
            Q_EMIT succeeded(jsonResult());
        } else {
            //: error message after automatically upgrading a password to an app password
            //% "The new application password returned by Nextcloud’s OCS API is empty."
            setError(new Error(Error::OutputError, Error::Critical, qtTrId("fuoten-convertpassword-empty-apppassword"), QString(), this));
            Q_EMIT failed(error());
        }
    } else {
        const QString message = meta.value(QStringLiteral("message")).toString();
        if (Q_LIKELY(!message.isEmpty())) {
            setError(new Error(Error::OutputError, Error::Critical, message, QString(), this));
        } else {
            //: error message after automatically upgrading a password to an app password
            //% "An unknown error occured while trying to upgrade the password to an application password. Status: %1"
            setError(new Error(Error::OutputError, Error::Critical, qtTrId("fuoten-convertpassword-unknown-error").arg(meta.value(QStringLiteral("status")).toString()), QString(), this));
        }
        Q_EMIT failed(error());
    }

    setInOperation(false);
}

void ConvertToAppPassword::extractError(QNetworkReply *reply)
{
    if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() != 403) {
        Component::extractError(reply);
    } else {
        qDebug("%s", "Client is already authenticated with an app password. All fine.");
        Q_EMIT succeeded(QJsonDocument());
    }
}

#include "moc_converttoapppassword.cpp"
