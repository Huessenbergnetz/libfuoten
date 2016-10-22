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

#ifndef FUOTEN
#define FUOTEN

#include "fuoten_global.h"
#include <QMetaType>

namespace Fuoten {

/*!
 * \brief Provides global enumerations.
 *
 * \headerfile "" <Fuoten/Fuoten>
 */
class FUOTENSHARED_EXPORT Fuoten {
    Q_GADGET
public:

    /*!
     * \brief Object types.
     */
    enum Type {
        Feed    = 0,    /**< Feed */
        Folder  = 1,    /**< Folder */
        Starred = 2,    /**< Starred items */
        All     = 3     /**< All items */
    };
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
    Q_ENUM(Type)
#else
    Q_ENUMS(Type)
#endif

    /*!
     * \brief Sorting role.
     */
    enum SortingRole {
        ID          = 0,    /**< Sort by database ID */
        Name        = 1,    /**< Sort by name/title */
        Time        = 2,    /**< Sort by time */
        UnreadCount = 3,    /**< Sort by unread item count */
        ItemCount   = 4,    /**< Sort by total item count */
        FeedCount   = 5     /**< Sort by feed count */
    };
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
    Q_ENUM(SortingRole)
#else
    Q_ENUMS(SortingRole)
#endif

private:
    Fuoten();
    ~Fuoten();
};
}

#if QT_VERSION < QT_VERSION_CHECK(5, 5, 0)
Q_DECLARE_METATYPE(Fuoten::Fuoten::Type)
Q_DECLARE_METATYPE(Fuoten::Fuoten::SortingRole)
#endif

#endif // FUOTEN

