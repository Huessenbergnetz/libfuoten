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

#ifndef FUOTEN
#define FUOTEN

#include "fuoten_global.h"
#include <QMetaType>
#include <QVersionNumber>

namespace Fuoten {

FUOTENSHARED_EXPORT QVersionNumber version();

/*!
 * \brief Provides global enumerations.
 *
 * \headerfile "" <Fuoten/FuotenEnums>
 */
class FUOTENSHARED_EXPORT FuotenEnums {
    Q_GADGET
public:

    /*!
     * \brief Object types.
     */
    enum Type : quint8 {
        Feed    = 0,    /**< Feed */
        Folder  = 1,    /**< Folder */
        Starred = 2,    /**< Starred items/articles */
        All     = 3,    /**< All items/articles */
        Item    = 4     /**< Item/Article */
    };
    Q_ENUM(Type)

    /*!
     * \brief Sorting role.
     */
    enum SortingRole : quint8 {
        ID          = 0,    /**< Sort by database ID */
        Name        = 1,    /**< Sort by name/title */
        Time        = 2,    /**< Sort by time */
        UnreadCount = 3,    /**< Sort by unread item count */
        FeedCount   = 4,    /**< Sort by feed count (only applicable to folders) */
        FolderName  = 5     /**< Sort by folder name (only applicable to feeds) */
    };
    Q_ENUM(SortingRole)

    /*!
     * \brief Actions for the local queue.
     */
    enum QueueAction : quint8 {
        NoQueueAction   = 0x0,    /**< No action set. */
        MarkAsRead      = 0x1,    /**< The item has been marked as read. */
        MarkAsUnread    = 0x2,    /**< The item has been marked as unread. */
        Star            = 0x4,    /**< The item has been starred. */
        Unstar          = 0x8     /**< The item has been unstarred. */
    };
    Q_DECLARE_FLAGS(QueueActions, QueueAction)
    Q_FLAG(QueueActions)

    /*!
     * \brief Item per feed deletion strategy.
     */
    enum ItemDeletionStrategy : quint8 {
        NoItemDeletion      = 0,    /**< Items will never be deleted. */
        DeleteItemsByTime   = 1,    /**< Items will be deleted if they are older than a specific time. */
        DeleteItemsByCount  = 2     /**< Items will deleted if the feed contains more than a specific count. */
    };
    Q_ENUM(ItemDeletionStrategy)

private:
    FuotenEnums();
    ~FuotenEnums();
};
}

Q_DECLARE_OPERATORS_FOR_FLAGS(Fuoten::FuotenEnums::QueueActions)

#endif // FUOTEN

