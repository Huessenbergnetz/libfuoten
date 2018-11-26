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

#ifndef FUOTENABSTRACTARTICLEMODEL_P_H
#define FUOTENABSTRACTARTICLEMODEL_P_H

#include "abstractarticlemodel.h"
#include "basemodel_p.h"
#include "../article.h"

namespace Fuoten {

class AbstractArticleModelPrivate : public BaseModelPrivate
{
public:
    AbstractArticleModelPrivate() : BaseModelPrivate()
    {
        sortingRole = FuotenEnums::Time;
        sortOrder = Qt::DescendingOrder;
    }

    ~AbstractArticleModelPrivate() {
        while (!articles.isEmpty()) {
            Article *a = articles.takeFirst();
            if (!a->inOperation()) {
                delete a;
            }
        }
    }


    int rowByID(qint64 id) {
        if (articles.isEmpty()) {
            return -1;
        }

        int idx = -1;

        for (int i = 0; i < articles.count(); ++i) {
            if (articles.at(i)->id() == id) {
                idx = i;
                break;
            }
        }

        return idx;
    }

    int rowByGuidHash(const QString &guidHash) {
        if (articles.isEmpty()) {
            return -1;
        }

        int idx = -1;

        for (int i = 0; i < articles.count(); ++i) {
            if (articles.at(i)->guidHash() == guidHash) {
                idx = i;
                break;
            }
        }

        return idx;
    }

    QList<Article*> articles;
    int bodyLimit = -1;
    FuotenEnums::Type parentIdType = FuotenEnums::All;
    bool starredOnly = false;

private:
    Q_DISABLE_COPY(AbstractArticleModelPrivate)
};

}

#endif // FUOTENABSTRACTARTICLEMODEL_P_H
