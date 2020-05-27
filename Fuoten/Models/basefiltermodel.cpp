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

#include "basefiltermodel_p.h"
#include <QMetaEnum>

using namespace Fuoten;


BaseFilterModelPrivate::BaseFilterModelPrivate()
{
    search.setCaseSensitivity(Qt::CaseInsensitive);
}

BaseFilterModelPrivate::~BaseFilterModelPrivate()
{

}


BaseFilterModel::BaseFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent), d_ptr(new BaseFilterModelPrivate)
{

}


BaseFilterModel::BaseFilterModel(BaseFilterModelPrivate &dd, QObject *parent) :
    QSortFilterProxyModel(parent), d_ptr(&dd)
{

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
        qDebug("Changed sortingRole to %s.", FuotenEnums::staticMetaObject.enumerator(FuotenEnums::staticMetaObject.indexOfEnumerator("Type")).valueToKey(d->sortingRole));
        Q_EMIT sortingRoleChanged(sortingRole());
        invalidate();
    }
}


Qt::SortOrder BaseFilterModel::sortOrder() const { Q_D(const BaseFilterModel); return d->sortOrder; }

void BaseFilterModel::setSortOrder(Qt::SortOrder nSortOrder)
{
    Q_D(BaseFilterModel); 
    if (nSortOrder != d->sortOrder) {
        d->sortOrder = nSortOrder;
        qDebug("Changed sortOrder to %i.", d->sortOrder);
        Q_EMIT sortOrderChanged(sortOrder());
        invalidate();
    }
}


QString BaseFilterModel::search() const { Q_D(const BaseFilterModel); return d->search.pattern(); }

void BaseFilterModel::setSearch(const QString &nSearch)
{
    Q_D(BaseFilterModel);
    if (nSearch != d->search.pattern()) {
        d->search.setPattern(nSearch);
        qDebug("Changed search to %s.", qUtf8Printable(d->search.pattern()));
        Q_EMIT searchChanged(search());
        invalidateFilter();
    }
}


bool BaseFilterModel::find(const QString &str) const
{
    Q_D(const BaseFilterModel);
    return (d->search.indexIn(str) > -1);
}


bool BaseFilterModel::hideRead() const { Q_D(const BaseFilterModel); return d->hideRead; }

void BaseFilterModel::setHideRead(bool nHideRead)
{
    Q_D(BaseFilterModel);
    if (nHideRead != d->hideRead) {
        d->hideRead = nHideRead;
        qDebug("Changed hideRead to %s.", d->hideRead ? "true" : "false");
        Q_EMIT hideReadChanged(hideRead());
        invalidateFilter();
    }
}


double BaseFilterModel::doubleParentId() const { return static_cast<double>(parentId()); }

void BaseFilterModel::setDoubleParentId(double nDoubleParentId)
{
    setParentId(static_cast<qint64>(nDoubleParentId));
}

#include "moc_basefiltermodel.cpp"
