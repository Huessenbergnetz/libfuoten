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

#include "getuser_p.h"
#include "../error.h"
#include <QJsonObject>
#include <QJsonValue>

using namespace Fuoten;

GetUserPrivate::GetUserPrivate() :
    ComponentPrivate()
{

}


GetUserPrivate::~GetUserPrivate()
{

}


GetUser::GetUser(QObject *parent) :
    Component(*new GetUserPrivate, parent)
{
    setApiRoute(QStringLiteral("/user"));
    setExpectedJSONType(Component::Object);
}


GetUser::GetUser(GetUserPrivate &dd, QObject *parent) :
    Component(dd, parent)
{
    setApiRoute(QStringLiteral("/user"));
    setExpectedJSONType(Component::Object);
}


GetUser::~GetUser()
{

}


void GetUser::execute()
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Still in operation. Returning.");
        return;
    }

    qDebug("%s", "Start requesting user information from the server.");

    setInOperation(true);

    sendRequest();
}


void GetUser::successCallback()
{
    setInOperation(false);

    qDebug("%s", "Successfully requested user information from the server.");
    Q_EMIT succeeded(jsonResult());
}


bool GetUser::checkOutput()
{
    if (Q_LIKELY(Component::checkOutput())) {

        const QJsonObject o = jsonResult().object();

        if (Q_UNLIKELY(!o.contains(QStringLiteral("displayName")))) {
            //% "Can not find the user's display name in the server reply."
            setError(new Error(Error::OutputError, Error::Critical, qtTrId("err-displayname-not-found"), QString(), this));
            Q_EMIT failed(error());
            return false;
        }

    } else {
        return false;
    }

    return true;
}

#include "moc_getuser.cpp"
