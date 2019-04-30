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

#ifndef FUOTENGETUPDATEDITEMS_P_H
#define FUOTENGETUPDATEDITEMS_P_H

#include "getupdateditems.h"
#include "component_p.h"

namespace Fuoten {

class GetUpdatedItemsPrivate : public ComponentPrivate
{
public:
    GetUpdatedItemsPrivate() : ComponentPrivate()
    {
        apiRoute = QStringLiteral("/items/updated");
        expectedJSONType = Component::Object;
    }

    GetUpdatedItemsPrivate(const QDateTime &nLastModified, FuotenEnums::Type nType, qint64 nParentId) :
        ComponentPrivate(),
        parentId(nParentId),
        lastModified(nLastModified),
        type(nType)
    {
        apiRoute = QStringLiteral("/item/updated");
        expectedJSONType = Component::Object;
    }

    qint64 parentId = 0;
    QDateTime lastModified;
    FuotenEnums::Type type = FuotenEnums::All;
};

}

#endif // FUOTENGETUPDATEDITEMS_P_H
