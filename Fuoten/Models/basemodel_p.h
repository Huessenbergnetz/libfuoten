/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016 Buschtrommel / Matthias Fehring
 * https://www.buschmann23.de/entwicklung/bibliotheken/libfuoten/
 * https://github.com/Buschtrommel/libfuoten
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

#ifndef FUOTENBASEMODEL_P_H
#define FUOTENBASEMODEL_P_H

#include "basemodel.h"

namespace Fuoten {

class BaseModelPrivate
{
public:
    BaseModelPrivate() {}

    virtual ~BaseModelPrivate() {}

    bool inOperation = false;
    AbstractStorage *storage = nullptr;
    bool loaded = false;
    qint64 parentId = -1;
    FuotenEnums::SortingRole sortingRole = FuotenEnums::ID;
    Qt::SortOrder sortOrder = Qt::AscendingOrder;
    bool unreadOnly = false;
    int limit = 0;

private:
    Q_DISABLE_COPY(BaseModelPrivate)
};

}

#endif // FUOTENBASEMODEL_P_H
