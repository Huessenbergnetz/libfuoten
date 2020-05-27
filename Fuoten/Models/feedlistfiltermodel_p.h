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

#ifndef FUOTENFEEDLISTFILTERMODEL_P_H
#define FUOTENFEEDLISTFILTERMODEL_P_H

#include "feedlistfiltermodel.h"
#include "basefiltermodel_p.h"
#include "feedlistmodel.h"

namespace Fuoten {

class FeedListFilterModelPrivate : public BaseFilterModelPrivate
{
public:
    FeedListFilterModelPrivate();
    ~FeedListFilterModelPrivate() override;

    const QScopedPointer<FeedListModel> flm;
    bool respectPinned;
    bool sortByFolder;
};

}

#endif // FUOTENFEEDLISTFILTERMODEL_P_H
