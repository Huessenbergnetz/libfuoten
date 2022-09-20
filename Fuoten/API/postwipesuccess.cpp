/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "postwipesuccess_p.h"
#include "../error.h"

using namespace Fuoten;

PostWipeSuccessPrivate::PostWipeSuccessPrivate() :
    ComponentPrivate()
{
    baseRoute = QString();
    apiRoute = QStringLiteral("/index.php/core/wipe/success");
    requiresAuth = false;
    useStorage = false;
    checkForWipe = false;
    expectedJSONType = Component::Empty;
    namOperation = QNetworkAccessManager::PostOperation;
    payloadContentType = QByteArrayLiteral("application/x-www-form-urlencoded");
}

PostWipeSuccessPrivate::~PostWipeSuccessPrivate()
{

}

PostWipeSuccess::PostWipeSuccess(QObject *parent) :
    Component(* new PostWipeSuccessPrivate(), parent)
{

}

PostWipeSuccess::PostWipeSuccess(PostWipeSuccessPrivate &dd, QObject *parent) :
    Component(dd, parent)
{

}

PostWipeSuccess::~PostWipeSuccess()
{

}

void PostWipeSuccess::execute()
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("%s", "Still in operation. Returning.");
        return;
    }

    qDebug("%s", "Start sending wipe success message.");

    setInOperation(true);

    Q_ASSERT_X(configuration(), "send request", "no configuration available");

    const QByteArray data = QByteArrayLiteral("token=") + configuration()->getPassword().toUtf8();
    setPayload(data);

    sendRequest();
}

bool PostWipeSuccess::checkInput()
{
    if (configuration()->getPassword().isEmpty()) {
        //% "Can not post wipe success to the remote server without application token/password."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-wipe-success-no-token"), QString(), this));
        Q_EMIT failed(error());
        return false;
    }

    return Component::checkInput();
}

void PostWipeSuccess::successCallback()
{
    qDebug("%s", "Successfully send wipe success message.");
    setInOperation(false);
    Q_EMIT succeeded(QJsonDocument());
}

#include "moc_postwipesuccess.cpp"
