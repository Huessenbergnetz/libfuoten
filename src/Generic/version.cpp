/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016 Buschtrommel / Matthias Fehring
 * https://www.buschmann23.de/entwicklung/bibliotheken/libfuoten/
 * https://github.com/Buschtrommel/libfuoten
 *
 * generic/version.cpp
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

#include "version_p.h"
#include "../error.h"
#include <QJsonValue>

using namespace Fuoten;
using namespace Generic;

Version::Version(QObject *parent) :
    Component(*new VersionPrivate, parent)
{
    setApiRoute(QStringLiteral("/version"));
    setExpectedJSONType(Component::Object);
}


Version::Version(VersionPrivate &dd, QObject *parent) :
    Component(dd, parent)
{
    setApiRoute(QStringLiteral("/version"));
    setExpectedJSONType(Component::Object);
}


void Version::get()
{
    if (inOperation()) {
        return;
    }

    setInOperation(true);

    sendRequest();
}


void Version::successCallback()
{
    Q_D(const Version);
    if (configuration()) {
        configuration()->setServerVersion(d->resultObject.value(QStringLiteral("version")).toString());
    }
    setInOperation(false);
    Q_EMIT succeeded(jsonResult());
}



void Version::extractError(QNetworkReply *reply)
{
    setError(new Error(reply, this));
    setInOperation(false);
    Q_EMIT failed(error());
}



bool Version::checkOutput()
{
    if (Component::checkOutput()) {

        Q_D(Version);

        d->resultObject = jsonResult().object();

        if (!d->resultObject.contains(QStringLiteral("version"))) {
            setError(new Error(Error::OutputError, Error::Critical, tr("Can not find the version information in the server reply."), QString(), this));
            Q_EMIT failed(error());
            if (configuration()) { configuration()->setServerVersion("0.0.0"); }
            return false;
        } else {
            return true;
        }

    } else {
        if (configuration()) { configuration()->setServerVersion("0.0.0"); }
        return false;
    }
}
