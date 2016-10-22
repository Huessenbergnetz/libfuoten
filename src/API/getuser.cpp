/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016 Buschtrommel / Matthias Fehring
 * https://www.buschmann23.de/entwicklung/bibliotheken/libfuoten/
 * https://github.com/Buschtrommel/libfuoten
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
#include <QJsonValue>
#ifdef QT_DEBUG
#include <QtDebug>
#endif

using namespace Fuoten;

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



void GetUser::execute()
{
    if (inOperation()) {
        qWarning("Still in operation. Returning.");
        return;
    }

#ifdef QT_DEBUG
    qDebug() << "Start requesting user information from the server.";
#endif

    setInOperation(true);

    sendRequest();
}



void GetUser::successCallback()
{
    Q_D(const GetUser);
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

#ifdef QT_DEBUG
    qDebug() << "Successfully requested user information from the server.";
#endif

    Q_EMIT succeeded(jsonResult());
}



void GetUser::extractError(QNetworkReply *reply)
{
    setError(new Error(reply, this));
    setInOperation(false);
    Q_EMIT failed(error());
}


bool GetUser::checkOutput()
{
    if (Component::checkOutput()) {

        Q_D(GetUser);

        d->resultObject = jsonResult().object();

        if (!d->resultObject.contains(QStringLiteral("displayName"))) {
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
