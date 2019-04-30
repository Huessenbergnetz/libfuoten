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

#ifndef FUOTENFOLDER_P_H
#define FUOTENFOLDER_P_H

#include "folder.h"
#include "baseitem_p.h"

namespace Fuoten {

class FolderPrivate : public BaseItemPrivate
{
public:
    FolderPrivate() :
        BaseItemPrivate(),
        feedCount(0),
        unreadCount(0)
    {}

    FolderPrivate(qint64 _id, const QString &_name, uint _feedCount, uint _unreadCount) :
        BaseItemPrivate(_id),
        name(_name),
        feedCount(_feedCount),
        unreadCount(_unreadCount)
    {}

    QString name;
    uint feedCount;
    uint unreadCount;

private:
    Q_DISABLE_COPY(FolderPrivate)
};

}

#endif // FUOTENFOLDER_P_H
