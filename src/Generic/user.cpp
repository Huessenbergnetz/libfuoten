/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016 Buschtrommel / Matthias Fehring
 * https://www.buschmann23.de/entwicklung/bibliotheken/libfuoten/
 * https://github.com/Buschtrommel/libfuoten
 *
 * generic/user.cpp
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

#include "user_p.h"
#include "../error.h"
#include <QJsonValue>

using namespace Fuoten;
using namespace Generic;

User::User(QObject *parent) :
    Component(*new UserPrivate, parent)
{
    setApiRoute(QStringLiteral("/user"));
    setExpectedJSONType(Component::Object);
}


User::User(UserPrivate &dd, QObject *parent) :
    Component(dd, parent)
{
    setApiRoute(QStringLiteral("/user"));
    setExpectedJSONType(Component::Object);
}



void User::get()
{
    if (inOperation()) {
        return;
    }

    setInOperation(true);

    sendRequest();
}



void User::successCallback()
{
    Q_D(const User);
    if (configuration()) {
        configuration()->setDisplayName(d->resultObject.value(QStringLiteral("displayName")).toString());
        const QJsonObject a = d->resultObject.value(QStringLiteral("avatar")).toObject();
        if (!a.isEmpty()) {
            configuration()->setAvatar(a.value(QStringLiteral("data")).toString(), a.value(QStringLiteral("mime")).toString());
        } else {
            configuration()->setAvatar(QString(), QString());
        }
    }

    setInOperation(false);
    Q_EMIT succeeded(jsonResult());
}



void User::extractError(QNetworkReply *reply)
{
    setError(new Error(reply, this));
    setInOperation(false);
    Q_EMIT failed(error());
}


bool User::checkOutput()
{
    if (Component::checkOutput()) {

        Q_D(User);

        d->resultObject = jsonResult().object();

        if (!d->resultObject.contains(QStringLiteral("displayName"))) {
            setError(new Error(Error::OutputError, Error::Critical, tr("Can not find the user's display name in the server reply."), QString(), this));
            Q_EMIT failed(error());
            return false;
        }

    } else {
        return false;
    }

    return true;
}
