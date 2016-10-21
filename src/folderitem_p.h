/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016 Buschtrommel / Matthias Fehring
 * https://www.buschmann23.de/entwicklung/bibliotheken/libfuoten/
 * https://github.com/Buschtrommel/libfuoten
 *
 * folderitem_p.h
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

#ifndef FOLDERITEM_P_H
#define FOLDERITEM_P_H

#include "folderitem.h"
#include "baseitem_p.h"

namespace Fuoten {


class FolderItemPrivate : public BaseItemPrivate
{
public:
    FolderItemPrivate() :
        BaseItemPrivate(),
        feedCount(0),
        unreadCount(0),
        itemCount(0)
    {}

    FolderItemPrivate(quint64 _id, const QString &_name, uint _feedCount, uint _unreadCount, uint _itemCount) :
        BaseItemPrivate(_id),
        name(_name),
        feedCount(_feedCount),
        unreadCount(_unreadCount),
        itemCount(_itemCount)
    {}

    QString name;
    uint feedCount;
    uint unreadCount;
    uint itemCount;
    QString newName;

};

}

#endif // FOLDERITEM_P_H
