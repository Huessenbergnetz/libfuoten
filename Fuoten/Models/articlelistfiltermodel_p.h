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

#ifndef FUOTENARTICLELISTFILTERMODEL_P_H
#define FUOTENARTICLELISTFILTERMODEL_P_H

#include "articlelistfiltermodel.h"
#include "basefiltermodel_p.h"
#include "articlelistmodel.h"

namespace Fuoten {

class ArticleListFilterModelPrivate : public BaseFilterModelPrivate
{
public:
    ArticleListFilterModelPrivate() :
        BaseFilterModelPrivate(),
        alm(new ArticleListModel)
    {
        sortOrder = Qt::DescendingOrder;
    }

    const QScopedPointer<ArticleListModel> alm;
};

}

#endif // FUOTENARTICLELISTFILTERMODEL_P_H
