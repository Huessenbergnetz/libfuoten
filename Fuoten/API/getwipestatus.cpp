/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "getwipestatus_p.h"
#include "../error.h"
#include <QJsonObject>

using namespace Fuoten;

GetWipeStatusPrivate::GetWipeStatusPrivate() :
    ComponentPrivate()
{
    baseRoute = QString();
    apiRoute = QStringLiteral("/index.php/core/wipe/check");
    requiresAuth = false;
    useStorage = false;
    checkForWipe = false;
    expectedJSONType = Component::Object;
    namOperation = QNetworkAccessManager::PostOperation;
    payloadContentType = QByteArrayLiteral("application/x-www-form-urlencoded");
}

GetWipeStatusPrivate::~GetWipeStatusPrivate()
{

}

GetWipeStatus::GetWipeStatus(QObject *parent) :
    Component(* new GetWipeStatusPrivate(), parent)
{

}

GetWipeStatus::GetWipeStatus(GetWipeStatusPrivate &dd, QObject *parent) :
    Component(dd, parent)
{

}

GetWipeStatus::~GetWipeStatus()
{

}

void GetWipeStatus::execute()
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("%s", "Still in operation. Returning.");
        return;
    }

    qDebug("%s", "Start checking wipe status.");

    setInOperation(true);

    if (!configuration()) {
        setConfiguration(Component::defaultConfiguration());
    }
    Q_ASSERT_X(configuration(), "send request", "no configuration available");

    const QByteArray data = QByteArrayLiteral("token=") + configuration()->getPassword().toUtf8();
    setPayload(data);

    sendRequest();
}

bool GetWipeStatus::checkInput()
{
    if (configuration()->getPassword().isEmpty()) {
        //% "Can not check for wipe status without a valid application token/password."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-wipe-no-token"), QString(), this));
        Q_EMIT failed(error());
        return false;
    }

    return Component::checkInput();
}

void GetWipeStatus::successCallback()
{
    const QJsonObject o = jsonResult().object();
    const bool doWipe = o.value(QStringLiteral("wipe")).toBool();

    qDebug("%s", "Successfully requested wipe success status.");

    if (doWipe) {
        qInfo("%s", "Remote wipe requested.");
        Q_EMIT wipeRequested();
    }

    Q_EMIT succeeded(jsonResult());

    setInOperation(false);
}

#include "moc_getwipestatus.cpp"
