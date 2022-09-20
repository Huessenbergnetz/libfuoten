/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENGETITEMS_P_H
#define FUOTENGETITEMS_P_H

#include "getitems.h"
#include "component_p.h"

namespace Fuoten {

class GetItemsPrivate : public ComponentPrivate
{
public:
    GetItemsPrivate();
    GetItemsPrivate(int nBatchSize, qint64 nOffset, FuotenEnums::Type nType, qint64 nParentId, bool nGetRead, bool nOldestFirst);
    ~GetItemsPrivate() override;

    qint64 offset = 0;
    qint64 parentId = 0;
    int batchSize = -1;
    FuotenEnums::Type type = FuotenEnums::All;
    bool getRead = false;
    bool oldestFirst = false;
};

}

#endif // FUOTENGETITEMS_P_H
