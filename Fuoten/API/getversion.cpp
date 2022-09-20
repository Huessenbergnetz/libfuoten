/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "getversion_p.h"
#include "../error.h"
#include <QJsonObject>
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

    Q_D(GetVersion);

    qDebug("%s", "Start requesting version information from the server.");

    setInOperation(true);

    QVersionNumber v;
    if (d->version != v) {
        d->version = v;
        Q_EMIT versionStringChanged(versionString());
        Q_EMIT versionChanged(version());
    }

    sendRequest();
}


void GetVersion::successCallback()
{
    Q_D(GetVersion);

    const QVersionNumber v = QVersionNumber::fromString(jsonResult().object().value(QStringLiteral("version")).toString());

    if (d->version != v) {
        d->version = v;
        Q_EMIT versionStringChanged(versionString());
        Q_EMIT versionChanged(version());
    }

    setInOperation(false);

    qDebug("%s", "Successfully requested version information from the server.");

    Q_EMIT succeeded(jsonResult());
}


bool GetVersion::checkOutput()
{
    if (Q_LIKELY(Component::checkOutput())) {

        const QJsonObject o = jsonResult().object();
        if (Q_UNLIKELY(!o.contains(QStringLiteral("version")))) {
            //% "Can not find the version information in the server reply."
            setError(new Error(Error::OutputError, Error::Critical, qtTrId("err-version-not-found"), QString(), this));
            Q_EMIT failed(error());
            return false;
        } else {
            const QString s = o.value(QStringLiteral("version")).toString();
            const QVersionNumber v = QVersionNumber::fromString(s);
            if (Q_UNLIKELY(v.isNull())) {
                //% "Server reply contains invalid version number."
                setError(new Error(Error::OutputError, Error::Critical, qtTrId("err-version-invalid"), s, this));
                Q_EMIT failed(error());
                return false;
            } else {
                return true;
            }
        }

    } else {
        return false;
    }
}

QString GetVersion::versionString() const
{
    return version().toString();
}

QVersionNumber GetVersion::version() const
{
    Q_D(const GetVersion);
    return d->version;
}

#include "moc_getversion.cpp"
