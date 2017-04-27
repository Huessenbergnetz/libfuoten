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

#include "getstatus_p.h"
#include "../error.h"
#include <QJsonValue>
#ifdef QT_DEBUG
#include <QtDebug>
#endif


using namespace Fuoten;

GetStatus::GetStatus(QObject *parent) :
    Component(* new GetStatusPrivate, parent)
{
    setApiRoute(QStringLiteral("/status"));
    setExpectedJSONType(Component::Object);
}

GetStatus::GetStatus(GetStatusPrivate &dd, QObject *parent) :
    Component(dd, parent)
{
    setApiRoute(QStringLiteral("/status"));
    setExpectedJSONType(Component::Object);
}


void GetStatus::execute()
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Still in operation. Returning.");
        return;
    }

#ifdef QT_DEBUG
    qDebug() << "Start requesting the status from the server.";
#endif

    setInOperation(true);

    sendRequest();
}


void GetStatus::successCallback()
{
    Q_D(const GetStatus);
    configuration()->setServerVersion(d->resultObject.value(QStringLiteral("version")).toString());
    const QJsonObject w = d->resultObject.value(QStringLiteral("warnings")).toObject();
    if (Q_UNLIKELY(!w.isEmpty())) {
        configuration()->setImproperlyConfiguredCron(w.value(QStringLiteral("improperlyConfiguredCron")).toBool());
    }
    setInOperation(false);

#ifdef QT_DEBUG
    qDebug() << "Successfully requested the status from the server.";
#endif

    Q_EMIT succeeded(jsonResult());
}



bool GetStatus::checkOutput()
{
    if (Q_LIKELY(Component::checkOutput())) {

        Q_D(GetStatus);

        d->resultObject = jsonResult().object();

        if (Q_UNLIKELY(!d->resultObject.contains(QStringLiteral("version")))) {
            //% "Can not find the version information in the server reply."
            setError(new Error(Error::OutputError, Error::Critical, qtTrId("err-version-not-found"), QString(), this));
            Q_EMIT failed(error());
            return false;
        }

        if (Q_UNLIKELY(!d->resultObject.contains(QStringLiteral("warnings")))) {
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
