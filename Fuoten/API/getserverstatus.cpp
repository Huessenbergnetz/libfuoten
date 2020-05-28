/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016-2019 Hüssenbergnetz/Matthias Fehring
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

#include "getserverstatus_p.h"
#include <QJsonObject>
#include <QDebug>

using namespace Fuoten;

GetServerStatusPrivate::GetServerStatusPrivate() :
    ComponentPrivate()
{
    baseRoute = QString();
    apiRoute = QStringLiteral("/status.php");
    requiresAuth = false;
    useStorage = false;
    expectedJSONType = Component::Object;
    namOperation = QNetworkAccessManager::GetOperation;
}

GetServerStatusPrivate::~GetServerStatusPrivate()
{

}

GetServerStatus::GetServerStatus(QObject *parent) :
    Component(* new GetServerStatusPrivate, parent)
{

}

GetServerStatus::GetServerStatus(GetServerStatusPrivate &dd, QObject *parent) :
    Component(dd, parent)
{

}

GetServerStatus::~GetServerStatus()
{

}

void GetServerStatus::execute()
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("%s", "Still in operation. Returning.");
        return;
    }

    qDebug("%s", "Start requesting server status.");

    setInOperation(true);

    sendRequest();
}

bool GetServerStatus::isInstalled() const
{
    Q_D(const GetServerStatus);
    return d->isInstalled;
}

bool GetServerStatus::isInMaintenance() const
{
    Q_D(const GetServerStatus);
    return d->isInMaintenance;
}

bool GetServerStatus::needsDbUpgrade() const
{
    Q_D(const GetServerStatus);
    return d->needsDbUpgrade;
}

bool GetServerStatus::hasExtendedSupport() const
{
    Q_D(const GetServerStatus);
    return d->hasExtendedSupport;
}

QVersionNumber GetServerStatus::version() const
{
    Q_D(const GetServerStatus);
    return d->version;
}

QString GetServerStatus::versionString() const
{
    return version().toString();
}

QString GetServerStatus::edition() const
{
    Q_D(const GetServerStatus);
    return d->edition;
}

QString GetServerStatus::productname() const
{
    Q_D(const GetServerStatus);
    return d->productname;
}

Fuoten::GetServerStatus::SetupPossible GetServerStatus::setupPossible() const
{
    Q_D(const GetServerStatus);
    return d->setupPossible;
}

void GetServerStatus::successCallback()
{
    Q_D(GetServerStatus);

    setInOperation(false);

    qDebug("%s", "Successfully requested the server status.");

    const QJsonObject o = jsonResult().object();

    const bool _isInstalled = o.value(QStringLiteral("installed")).toBool(false);
    if (d->isInstalled != _isInstalled) {
        qDebug("Changing isInstalled from %s to %s", d->isInstalled ? "true" : "false", _isInstalled ? "true" : "false");
        d->isInstalled = _isInstalled;
        Q_EMIT isInstalledChanged(d->isInstalled);
    }

    const bool _isInMaintenance = o.value(QStringLiteral("maintenance")).toBool(false);
    if (d->isInMaintenance != _isInMaintenance) {
        qDebug("Changing isInMaintenance from %s to %s", d->isInMaintenance ? "true" : "false", _isInMaintenance ? "true" : "false");
        d->isInMaintenance = _isInMaintenance;
        Q_EMIT isInMaintenanceChanged(d->isInMaintenance);
    }

    const bool _needsDbUpgrade = o.value(QStringLiteral("needsDbUpgrade")).toBool(false);
    if (d->needsDbUpgrade != _needsDbUpgrade) {
        qDebug("Changing needsDbUpgrade from %s to %s", d->needsDbUpgrade ? "true" : "false", _needsDbUpgrade ? "true" : "false");
        d->needsDbUpgrade = _needsDbUpgrade;
        Q_EMIT needsDbUpgradeChanged(d->needsDbUpgrade);
    }

    const QVersionNumber _version = QVersionNumber::fromString(o.value(QStringLiteral("version")).toString());
    if (d->version != _version) {
        qDebug() << "Changing version from" << d->version << "to" << _version;
        d->version = _version;
        Q_EMIT versionStringChanged(d->version.toString());
        Q_EMIT versionChanged(d->version);
    }

    const QString _edition = o.value(QStringLiteral("edition")).toString();
    if (d->edition != _edition) {
        qDebug("Changing edition from \"%s\" to \"%s\"", qUtf8Printable(d->edition), qUtf8Printable(_edition));
        d->edition = _edition;
        Q_EMIT editionChanged(d->edition);
    }

    const QString _productname = o.value(QStringLiteral("productname")).toString();
    if (d->productname != _productname) {
        qDebug("Changing productname from \"%s\" to \"%s\"", qUtf8Printable(d->productname), qUtf8Printable(_productname));
        d->productname = _productname;
        Q_EMIT productnameChanged(d->productname);
    }

    const bool _hasExtendedSupport = o.value(QStringLiteral("extendedSupport")).toBool();
    if (d->hasExtendedSupport != _hasExtendedSupport) {
        qDebug("Changing hasExtendedSupport from %s to %s", d->hasExtendedSupport ? "true" : "false", _hasExtendedSupport ? "true" : "false");
        d->hasExtendedSupport = _hasExtendedSupport;
        Q_EMIT hasExtendedSupportChanged(d->hasExtendedSupport);
    }

    SetupPossible _setupPossible = NotPossible;
    if (d->isInstalled && !d->isInMaintenance) {
        _setupPossible = LoginFlow;
        if (d->version.normalized() >= QVersionNumber(16, 0, 0)) {
            _setupPossible = LoginFlowV2;
        }
    }
    if (d->setupPossible != _setupPossible) {
        qDebug() << "Changing setupPossible from" << d->setupPossible << "to" << _setupPossible;
        d->setupPossible = _setupPossible;
        Q_EMIT setupPossibleChanged(d->setupPossible);
    }

    Q_EMIT succeeded(jsonResult());
}

int GetServerStatus::compareVersion(const QString &versionString) const
{
    return compareVersion(QVersionNumber::fromString(versionString));
}

int GetServerStatus::compareVersion(const QVersionNumber &version) const
{
    Q_D(const GetServerStatus);
    return QVersionNumber::compare(d->version.normalized(), version.normalized());
}

#include "moc_getserverstatus.cpp"
