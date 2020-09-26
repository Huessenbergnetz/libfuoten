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

#include "getstatus_p.h"
#include "../error.h"
#include <QJsonValue>

using namespace Fuoten;

GetStatusPrivate::GetStatusPrivate(GetStatus *q) :
    ComponentPrivate(), q_ptr(q)
{

}


GetStatusPrivate::~GetStatusPrivate()
{

}


GetStatus::GetStatus(QObject *parent) :
    Component(* new GetStatusPrivate(this), parent)
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


GetStatus::~GetStatus()
{

}


void GetStatus::execute()
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Still in operation. Returning.");
        return;
    }

    qDebug("%s", "Start requesting the status from the server.");

    setInOperation(true);

    qDebug("%s", "Clearing old status data.");
    Q_D(GetStatus);
    d->setVersion(QVersionNumber());
    d->setIncorrectDbCharset(false);
    d->setImproperlyConfiguredCrond(false);

    sendRequest();
}

QString GetStatus::versionString() const
{
    Q_D(const GetStatus);
    return d->version.toString();
}

bool GetStatus::improperlyConfiguredCron() const
{
    Q_D(const GetStatus);
    return d->improperlyConfiguredCron;
}

bool GetStatus::incorrectDbCharset() const
{
    Q_D(const GetStatus);
    return d->incorrectDbCharset;
}

void GetStatus::successCallback()
{
    Q_D(GetStatus);
    d->setVersion(d->resultObject.value(QStringLiteral("version")).toString());
    const QJsonObject w = d->resultObject.value(QStringLiteral("warnings")).toObject();
    d->setIncorrectDbCharset(w.value(QStringLiteral("improperlyConfiguredCron")).toBool());
    d->setIncorrectDbCharset(w.value(QStringLiteral("incorrectDbCharset")).toBool());
    setInOperation(false);

    qDebug("%s", "Successfully requested the status from the server.");

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

void GetStatusPrivate::setVersion(const QString &_version)
{
    setVersion(QVersionNumber::fromString(_version));
}

void GetStatusPrivate::setVersion(const QVersionNumber &_version)
{
    if (version != _version) {
        qDebug("Changing version from %s to %s.", qUtf8Printable(version.toString()), qUtf8Printable(_version.toString()));
        version = _version;
        Q_Q(GetStatus);
        Q_EMIT q->versionStringChanged(version.toString());
    }
}

void GetStatusPrivate::setIncorrectDbCharset(bool _incorrectDbCharset)
{
    if (incorrectDbCharset != _incorrectDbCharset) {
        qDebug("Changing incorrectDbCharset from %s to %s.", incorrectDbCharset ? "true" : "false", _incorrectDbCharset ? "true" : "false");
        incorrectDbCharset = _incorrectDbCharset;
        Q_Q(GetStatus);
        Q_EMIT q->incorrectDbCharsetChanged(incorrectDbCharset);
    }
}

void GetStatusPrivate::setImproperlyConfiguredCrond(bool _improperlyConfiguredCron)
{
    if (improperlyConfiguredCron != _improperlyConfiguredCron) {
        qDebug("Changing improperlyConfiguredCron from %s to %s.", improperlyConfiguredCron ? "true" : "false", _improperlyConfiguredCron ? "true" : "false");
        improperlyConfiguredCron = _improperlyConfiguredCron;
        Q_Q(GetStatus);
        Q_EMIT q->improperlyConfiguredCronChanged(improperlyConfiguredCron);
    }
}

#include "moc_getstatus.cpp"
