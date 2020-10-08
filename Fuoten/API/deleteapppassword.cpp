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
