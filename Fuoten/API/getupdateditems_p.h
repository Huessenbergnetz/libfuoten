/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENGETUPDATEDITEMS_P_H
#define FUOTENGETUPDATEDITEMS_P_H

#include "getupdateditems.h"
#include "component_p.h"

namespace Fuoten {

class GetUpdatedItemsPrivate : public ComponentPrivate
{
public:
    GetUpdatedItemsPrivate();
    GetUpdatedItemsPrivate(const QDateTime &nLastModified, FuotenEnums::Type nType, qint64 nParentId);
    ~GetUpdatedItemsPrivate() override;

    qint64 parentId = 0;
    QDateTime lastModified;
    FuotenEnums::Type type = FuotenEnums::All;
};

}

#endif // FUOTENGETUPDATEDITEMS_P_H
