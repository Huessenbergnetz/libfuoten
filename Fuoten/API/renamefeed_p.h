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

#ifndef FUOTENRENAMEFEED_P_H
#define FUOTENRENAMEFEED_P_H

#include "renamefeed.h"
#include "component_p.h"

namespace Fuoten {

class RenameFeedPrivate : public ComponentPrivate
{
public:
    RenameFeedPrivate() :
        ComponentPrivate(),
        feedId(0)
    {
        expectedJSONType = Component::Empty;
        namOperation = QNetworkAccessManager::PutOperation;
    }

    ~RenameFeedPrivate() {}

    qint64 feedId;
    QString newName;
};

}

#endif // FUOTENRENAMEFEED_P_H
