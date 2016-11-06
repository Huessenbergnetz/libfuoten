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

#ifndef ABSTRACTSTORAGE_P_H
#define ABSTRACTSTORAGE_P_H

#include "abstractstorage.h"
#include "../error.h"

namespace Fuoten {


class AbstractStoragePrivate
{
public:
    AbstractStoragePrivate() :
        ready(false),
        error(nullptr),
        totalUnread(0),
        starred(0)
    {}

    virtual ~AbstractStoragePrivate() {}

    bool ready;
    Error *error;
    quint16 totalUnread;
    quint16 starred;

private:
    Q_DISABLE_COPY(AbstractStoragePrivate)
};

}

#endif // ABSTRACTSTORAGE_P_H
