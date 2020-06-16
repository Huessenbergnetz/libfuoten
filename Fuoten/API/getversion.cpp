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

#include "getversion_p.h"
#include "../error.h"
#include <QJsonValue>

using namespace Fuoten;

GetVersionPrivate::GetVersionPrivate() :
    ComponentPrivate()
{

}


GetVersionPrivate::~GetVersionPrivate()
{

}


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


GetVersion::~GetVersion()
{

}


void GetVersion::execute()
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Still in operation. Returning.");
        return;
    }

    qDebug("%s", "Start requesting version information from the server.");

    setInOperation(true);

    sendRequest();
}


void GetVersion::successCallback()
{
    Q_D(const GetVersion);
    configuration()->setServerVersion(d->resultObject.value(QStringLiteral("version")).toString());
    setInOperation(false);

    qDebug("%s", "Successfully requested version information from the server.");

    Q_EMIT succeeded(jsonResult());
}


bool GetVersion::checkOutput()
{
    if (Q_LIKELY(Component::checkOutput())) {

        Q_D(GetVersion);

        d->resultObject = jsonResult().object();

        if (Q_UNLIKELY(!d->resultObject.contains(QStringLiteral("version")))) {
            //% "Can not find the version information in the server reply."
            setError(new Error(Error::OutputError, Error::Critical, qtTrId("err-version-not-found"), QString(), this));
            Q_EMIT failed(error());
            if (configuration()) { configuration()->setServerVersion(QStringLiteral("0.0.0")); }
            return false;
        } else {
            return true;
        }

    } else {
        configuration()->setServerVersion(QStringLiteral("0.0.0"));
        return false;
    }
}

#include "moc_getversion.cpp"
