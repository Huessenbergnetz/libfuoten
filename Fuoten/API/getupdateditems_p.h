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
