/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "deleteapppassword_p.h"

using namespace Fuoten;

DeleteAppPasswordPrivate::DeleteAppPasswordPrivate() :
    ComponentPrivate()
{
    baseRoute = QString();
    apiRoute = QStringLiteral("/ocs/v2.php/core/apppassword");
    requiresAuth = true;
    useStorage = false;
    expectedJSONType = Component::Object;
    namOperation = QNetworkAccessManager::DeleteOperation;
}

DeleteAppPasswordPrivate::~DeleteAppPasswordPrivate() = default;

DeleteAppPassword::DeleteAppPassword(QObject *parent) :
    Component(* new DeleteAppPasswordPrivate(), parent)
{
    addRequestHeader(QByteArrayLiteral("OCS-APIREQUEST"), QByteArrayLiteral("true"));
}

DeleteAppPassword::DeleteAppPassword(DeleteAppPasswordPrivate &dd, QObject *parent) :
    Component(dd, parent)
{
    addRequestHeader(QByteArrayLiteral("OCS-APIREQUEST"), QByteArrayLiteral("true"));
}

DeleteAppPassword::~DeleteAppPassword() = default;

void DeleteAppPassword::execute()
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("%s", "Still in operation. Returning.");
        return;
    }

    setInOperation(true);

    qDebug("%s", "Start deleting app password on the remote server.");

    if (!configuration()) {
        setConfiguration(Component::defaultConfiguration());
    }
    Q_ASSERT_X(configuration(), "send request", "no configuration available");

    QUrlQuery uq;
    uq.addQueryItem(QStringLiteral("format"), QStringLiteral("json"));
    setUrlQuery(uq);

    sendRequest();
}

void DeleteAppPassword::successCallback()
{
    Q_EMIT succeeded(jsonResult());
    setInOperation(false);
}

#include "moc_deleteapppassword.cpp"
