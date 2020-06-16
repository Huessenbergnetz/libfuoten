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

#ifndef FUOTENGETSERVERSTATUS_P_H
#define FUOTENGETSERVERSTATUS_P_H

#include "getserverstatus.h"
#include "component_p.h"

namespace Fuoten {

class GetServerStatusPrivate : public ComponentPrivate
{
public:
    GetServerStatusPrivate(GetServerStatus *q);
    ~GetServerStatusPrivate() override;

    void setEdition(const QString &_edition);
    void setProductname(const QString &_productname);
    void setVersion(const QVersionNumber &_version);
    void setSetupPossible(GetServerStatus::SetupPossible _setupPossible);
    void setIsInstalled(bool _isInstalled);
    void setIsInMaintenance(bool _isInMaintenance);
    void setHasExtendedSupport(bool _hasExtendedSupport);
    void setNeedsDbUpgrade(bool _needsDbUpgrade);

    QString edition;
    QString productname;
    QVersionNumber version;
    GetServerStatus::SetupPossible setupPossible = GetServerStatus::NotPossible;
    bool isInstalled = false;
    bool isInMaintenance = false;
    bool hasExtendedSupport = false;
    bool needsDbUpgrade = false;

private:
    GetServerStatus *q_ptr;
    Q_DECLARE_PUBLIC(GetServerStatus)
};

}

#endif // FUOTENGETSERVERSTATUS_P_H
