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

#include "basefiltermodel_p.h"
#ifdef QT_DEBUG
#include <QtDebug>
#endif

using namespace Fuoten;


BaseFilterModel::BaseFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent), d_ptr(new BaseFilterModelPrivate)
{
    setSortLocaleAware(true);
}


BaseFilterModel::BaseFilterModel(BaseFilterModelPrivate &dd, QObject *parent) :
    QSortFilterProxyModel(parent), d_ptr(&dd)
{
    setSortLocaleAware(true);
}


BaseFilterModel::~BaseFilterModel()
{
}



FuotenEnums::SortingRole BaseFilterModel::sortingRole() const { Q_D(const BaseFilterModel); return d->sortingRole; }

void BaseFilterModel::setSortingRole(FuotenEnums::SortingRole nSortingRole)
{
    Q_D(BaseFilterModel); 
    if (nSortingRole != d->sortingRole) {
        d->sortingRole = nSortingRole;
#ifdef QT_DEBUG
        qDebug() << "Changed sortingRole to" << d->sortingRole;
#endif
        Q_EMIT sortingRoleChanged(sortingRole());
        invalidate();
    }
}




Qt::SortOrder BaseFilterModel::sortOrder() const { Q_D(const BaseFilterModel); return d->sortOrder; }

void BaseFilterModel::setSortOrder(const Qt::SortOrder &nSortOrder)
{
    Q_D(BaseFilterModel); 
    if (nSortOrder != d->sortOrder) {
        d->sortOrder = nSortOrder;
#ifdef QT_DEBUG
        qDebug() << "Changed sortOrder to" << d->sortOrder;
#endif
        Q_EMIT sortOrderChanged(sortOrder());
        invalidate();
    }
}




QString BaseFilterModel::search() const { Q_D(const BaseFilterModel); return d->search; }

void BaseFilterModel::setSearch(const QString &nSearch)
{
    Q_D(BaseFilterModel);
    if (nSearch != d->search) {
        d->search = nSearch;
#ifdef QT_DEBUG
        qDebug() << "Changed search to" << d->search;
#endif
        Q_EMIT searchChanged(search());
        invalidateFilter();
    }
}
