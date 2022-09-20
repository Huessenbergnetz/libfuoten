/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
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
    Q_DISABLE_COPY(FuotenEnums)
};
}

Q_DECLARE_OPERATORS_FOR_FLAGS(Fuoten::FuotenEnums::QueueActions)

#endif // FUOTEN

