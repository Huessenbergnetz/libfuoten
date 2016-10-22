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

#include "getversion_p.h"
#include "../error.h"
#include <QJsonValue>
#ifdef QT_DEBUG
#include <QtDebug>
#endif

using namespace Fuoten;

GetVersion::GetVersion(QObject *parent) :
    Component(*new GetVersionPrivate, parent)
{
    setApiRoute(QStringLiteral("/version"));
    setExpectedJSONType(Component::Object);
}


GetVersion::GetVersion(GetVersionPrivate &dd, QObject *parent) :
    Component(dd, parent)
{
    setApiRoute(QStringLiteral("/version"));
    setExpectedJSONType(Component::Object);
}


void GetVersion::execute()
{
    if (inOperation()) {
        qWarning("Still in operation. Returning.");
        return;
    }

#ifdef QT_DEBUG
    qDebug() << "Start requesting version information from the server.";
#endif

    setInOperation(true);

    sendRequest();
}


void GetVersion::successCallback()
{
    Q_D(const GetVersion);
    if (configuration()) {
        configuration()->setServerVersion(d->resultObject.value(QStringLiteral("version")).toString());
    }
    setInOperation(false);

#ifdef QT_DEBUG
    qDebug() << "Successfully requested version information from the server.";
#endif

    Q_EMIT succeeded(jsonResult());
}



void GetVersion::extractError(QNetworkReply *reply)
{
    setError(new Error(reply, this));
    setInOperation(false);
    Q_EMIT failed(error());
}



bool GetVersion::checkOutput()
{
    if (Component::checkOutput()) {

        Q_D(GetVersion);

        d->resultObject = jsonResult().object();

        if (!d->resultObject.contains(QStringLiteral("version"))) {
            //% "Can not find the version information in the server reply."
            setError(new Error(Error::OutputError, Error::Critical, qtTrId("err-version-not-found"), QString(), this));
            Q_EMIT failed(error());
            if (configuration()) { configuration()->setServerVersion(QStringLiteral("0.0.0")); }
            return false;
        } else {
            return true;
        }

    } else {
        if (configuration()) { configuration()->setServerVersion(QStringLiteral("0.0.0")); }
        return false;
    }
}
