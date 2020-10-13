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

#include "getserverstatus_p.h"
#include <QJsonObject>
#include <QDebug>

using namespace Fuoten;

GetServerStatusPrivate::GetServerStatusPrivate(GetServerStatus *q) :
    ComponentPrivate(), q_ptr(q)
{
    baseRoute = QString();
    apiRoute = QStringLiteral("/status.php");
    requiresAuth = false;
    useStorage = false;
    checkForWipe = false;
    expectedJSONType = Component::Object;
    namOperation = QNetworkAccessManager::GetOperation;
}

GetServerStatusPrivate::~GetServerStatusPrivate()
{

}

GetServerStatus::GetServerStatus(QObject *parent) :
    Component(* new GetServerStatusPrivate(this), parent)
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

    qDebug("%s", "Clearing old status data.");
    Q_D(GetServerStatus);
    d->setIsInstalled(false);
    d->setIsInMaintenance(false);
    d->setNeedsDbUpgrade(false);
    d->setVersion(QVersionNumber());
    d->setEdition(QString());
    d->setProductname(QString());
    d->setHasExtendedSupport(false);
    d->setSetupPossible(NotPossible);

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

    d->setIsInstalled(o.value(QStringLiteral("installed")).toBool(false));
    d->setIsInMaintenance(o.value(QStringLiteral("maintenance")).toBool(false));
    d->setNeedsDbUpgrade(o.value(QStringLiteral("needsDbUpgrade")).toBool(false));
    d->setVersion(QVersionNumber::fromString(o.value(QStringLiteral("version")).toString()));
    d->setEdition(o.value(QStringLiteral("edition")).toString());
    d->setProductname(o.value(QStringLiteral("productname")).toString());
    d->setHasExtendedSupport(o.value(QStringLiteral("extendedSupport")).toBool(false));

    SetupPossible _setupPossible = NotPossible;
    if (d->isInstalled && !d->isInMaintenance) {
        _setupPossible = Manual;
        if (d->version.normalized() >= QVersionNumber(13, 0, 0)) {
            _setupPossible = LoginFlow;
            if (d->version.normalized() >= QVersionNumber(17, 0, 0)) {
                _setupPossible = LoginFlowV2;
            }
        }
    }
    d->setSetupPossible(_setupPossible);

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

void GetServerStatusPrivate::setEdition(const QString &_edition)
{
    if (edition != _edition) {
        qDebug("Changing edition from %s to %s", qUtf8Printable(edition), qUtf8Printable(_edition));
        edition = _edition;
        Q_Q(GetServerStatus);
        Q_EMIT q->editionChanged(edition);
    }
}

void GetServerStatusPrivate::setProductname(const QString &_productname)
{
    if (productname != _productname) {
        qDebug("Changing productname from %s to %s", qUtf8Printable(productname), qUtf8Printable(_productname));
        productname = _productname;
        Q_Q(GetServerStatus);
        Q_EMIT q->productnameChanged(productname);
    }
}

void GetServerStatusPrivate::setVersion(const QVersionNumber &_version)
{
    if (version != _version) {
        qDebug("Changing version from %s to %s", qUtf8Printable(version.toString()), qUtf8Printable(_version.toString()));
        version = _version;
        Q_Q(GetServerStatus);
        Q_EMIT q->versionChanged(version);
        Q_EMIT q->versionStringChanged(version.toString());
    }
}

void GetServerStatusPrivate::setSetupPossible(GetServerStatus::SetupPossible _setupPossible)
{
    if (setupPossible != _setupPossible) {
        qDebug() << "Changing setupPossible from" << setupPossible << "to" << _setupPossible;
        setupPossible = _setupPossible;
        Q_Q(GetServerStatus);
        Q_EMIT q->setupPossibleChanged(setupPossible);
    }
}

void GetServerStatusPrivate::setIsInstalled(bool _isInstalled)
{
    if (isInstalled != _isInstalled) {
        qDebug() << "Changing isInstalled from" << isInstalled << "to" << _isInstalled;
        isInstalled = _isInstalled;
        Q_Q(GetServerStatus);
        Q_EMIT q->isInstalledChanged(isInstalled);
    }
}

void GetServerStatusPrivate::setIsInMaintenance(bool _isInMaintenance)
{
    if (isInMaintenance != _isInMaintenance) {
        qDebug() << "Changing isInMaintenance from" << isInMaintenance << "to" << _isInMaintenance;
        isInMaintenance = _isInMaintenance;
        Q_Q(GetServerStatus);
        Q_EMIT q->isInMaintenanceChanged(isInMaintenance);
    }
}

void GetServerStatusPrivate::setHasExtendedSupport(bool _hasExtendedSupport)
{
    if (hasExtendedSupport != _hasExtendedSupport) {
        qDebug() << "Changing hasExtendedSupport from" << hasExtendedSupport << "to" << _hasExtendedSupport;
        hasExtendedSupport = _hasExtendedSupport;
        Q_Q(GetServerStatus);
        Q_EMIT q->hasExtendedSupportChanged(hasExtendedSupport);
    }
}

void GetServerStatusPrivate::setNeedsDbUpgrade(bool _needsDbUpgrade)
{
    if (needsDbUpgrade != _needsDbUpgrade) {
        qDebug() << "Changing needsDbUpgrade from" << needsDbUpgrade << "to" << _needsDbUpgrade;
        needsDbUpgrade = _needsDbUpgrade;
        Q_Q(GetServerStatus);
        Q_EMIT q->needsDbUpgradeChanged(needsDbUpgrade);
    }
}

#include "moc_getserverstatus.cpp"
