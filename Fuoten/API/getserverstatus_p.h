/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
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
