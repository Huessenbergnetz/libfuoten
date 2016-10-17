/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016 Buschtrommel / Matthias Fehring
 * https://www.buschmann23.de/entwicklung/bibliotheken/libfuoten/
 * https://github.com/Buschtrommel/libfuoten
 *
 * generic/status.cpp
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

#include "status_p.h"
#include "../error.h"
#include <QJsonValue>


using namespace Fuoten;
using namespace Generic;

Status::Status(QObject *parent) :
    Component(* new StatusPrivate, parent)
{
    setApiRoute(QStringLiteral("/status"));
    setExpectedJSONType(Component::Object);
}

Status::Status(StatusPrivate &dd, QObject *parent) :
    Component(dd, parent)
{
    setApiRoute(QStringLiteral("/status"));
    setExpectedJSONType(Component::Object);
}


void Status::get()
{
    if (inOperation()) {
        return;
    }

    setInOperation(true);

    sendRequest();
}


void Status::successCallback()
{
    Q_D(const Status);
    if (configuration()) {
        configuration()->setServerVersion(d->resultObject.value(QStringLiteral("version")).toString());
        const QJsonObject w = d->resultObject.value(QStringLiteral("warnings")).toObject();
        if (!w.isEmpty()) {
            configuration()->setImproperlyConfiguredCron(w.value(QStringLiteral("improperlyConfiguredCron")).toBool());
        }
    }
    setInOperation(false);
    Q_EMIT succeeded(jsonResult());
}


void Status::extractError(QNetworkReply *reply)
{
    setError(new Error(reply, this));
    setInOperation(false);
    Q_EMIT failed(error());
}



bool Status::checkOutput()
{
    if (Component::checkOutput()) {

        Q_D(Status);

        d->resultObject = jsonResult().object();

        if (!d->resultObject.contains(QStringLiteral("version"))) {
            //% "Can not find the version information in the server reply."
            setError(new Error(Error::OutputError, Error::Critical, qtTrId("err-version-not-found"), QString(), this));
            Q_EMIT failed(error());
            return false;
        }

        if (!d->resultObject.contains(QStringLiteral("warnings"))) {
            //% "Can not find the warnings information in the server reply."
            setError(new Error(Error::OutputError, Error::Critical, qtTrId("err-warnings-not-found"), QString(), this));
            Q_EMIT failed(error());
            return false;
        }

    } else {
        return false;
    }

    return true;
}
