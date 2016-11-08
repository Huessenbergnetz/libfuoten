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

#ifndef FUOTENABSTRACTFEEDMODEL_P_H
#define FUOTENABSTRACTFEEDMODEL_P_H

#include "abstractfeedmodel.h"
#include "basemodel_p.h"
#include "../feed.h"

namespace Fuoten {

class AbstractFeedModelPrivate : public BaseModelPrivate
{
public:
    AbstractFeedModelPrivate() :
        BaseModelPrivate()
    {
        sortingRole = FuotenEnums::Name;
        sortOrder = Qt::AscendingOrder;
    }

    ~AbstractFeedModelPrivate() {
        while (!feeds.isEmpty()) {
            Feed *f = feeds.takeFirst();
            if (!f->inOperation()) {
                delete f;
            }
        }
    }

    int rowByID(qint64 id) {
        if (feeds.isEmpty()) {
            return -1;
        }

        int idx = -1;

        for (int i = 0; i < feeds.count(); ++i) {
            if (feeds.at(i)->id() == id) {
                idx = i;
                break;
            }
        }

        return idx;
    }

    QList<Feed*> feeds;

private:
    Q_DISABLE_COPY(AbstractFeedModelPrivate)
};

}

#endif // FUOTENABSTRACTFEEDMODEL_P_H
