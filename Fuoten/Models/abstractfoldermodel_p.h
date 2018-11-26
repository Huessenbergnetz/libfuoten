/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016-2018 Hüssenbergnetz/Matthias Fehring
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

#ifndef FUOTENABSTRACTFOLDERMODEL_P_H
#define FUOTENABSTRACTFOLDERMODEL_P_H

#include "abstractfoldermodel.h"
#include "basemodel_p.h"
#include "../folder.h"

namespace Fuoten {
class AbstractFolderModelPrivate : public BaseModelPrivate
{
public:
    AbstractFolderModelPrivate() :
        BaseModelPrivate()
    {
        sortingRole = FuotenEnums::Name;
        sortOrder = Qt::AscendingOrder;
    }

    ~AbstractFolderModelPrivate() {
        while (!folders.isEmpty()) {
            Folder *f = folders.takeFirst();
            if (f->inOperation()) {
                f->deleteLater();
            } else {
                delete f;
            }
        }
    }

    int rowByID(qint64 id) {
        if (folders.isEmpty()) {
            return -1;
        }

        int idx = -1;

        for (int i = 0; i < folders.count(); ++i) {
            if (folders.at(i)->id() == id) {
                idx = i;
                break;
            }
        }

        return idx;
    }

    QList<Folder*> folders;

private:
    Q_DISABLE_COPY(AbstractFolderModelPrivate)
};
}

#endif // FUOTENABSTRACTFOLDERMODEL_P_H

