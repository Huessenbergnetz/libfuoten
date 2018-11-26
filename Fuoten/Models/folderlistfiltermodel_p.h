/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016-2018 Hüssenbergnetz/Matthias Fehring
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

#ifndef FUOTENFOLDERLISTFILTERMODEL_P_H
#define FUOTENFOLDERLISTFILTERMODEL_P_H

#include "folderlistfiltermodel.h"
#include "basefiltermodel_p.h"
#include "folderlistmodel.h"

namespace Fuoten {
class FolderListFilterModelPrivate : public BaseFilterModelPrivate
{
public:
    FolderListFilterModelPrivate() :
        BaseFilterModelPrivate(),
        flm(new FolderListModel)
    {
        sortingRole = FuotenEnums::Name;
    }

    const QScopedPointer<FolderListModel> flm;
//    FolderListModel *flm;
};
}

#endif // FUOTENFOLDERLISTFILTERMODEL_P_H

