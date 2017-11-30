/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016-2017 Matthias Fehring
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

#ifndef FUOTENGETITEMS_P_H
#define FUOTENGETITEMS_P_H

#include "getitems.h"
#include "component_p.h"

namespace Fuoten {

class GetItemsPrivate : public ComponentPrivate
{
public:
    GetItemsPrivate() : ComponentPrivate()
    {
        apiRoute = QStringLiteral("/items");
        expectedJSONType = Component::Object;
    }

    GetItemsPrivate(int nBatchSize, qint64 nOffset, FuotenEnums::Type nType, qint64 nParentId, bool nGetRead, bool nOldestFirst) :
        ComponentPrivate(),
        offset(nOffset),
        parentId(nParentId),
        batchSize(nBatchSize),
        type(nType),
        getRead(nGetRead),
        oldestFirst(nOldestFirst)
    {
        apiRoute = QStringLiteral("/items");
        expectedJSONType = Component::Object;
    }

    qint64 offset = 0;
    qint64 parentId = 0;
    int batchSize = -1;
    FuotenEnums::Type type = FuotenEnums::All;
    bool getRead = false;
    bool oldestFirst = false;
};

}

#endif // FUOTENGETITEMS_P_H
