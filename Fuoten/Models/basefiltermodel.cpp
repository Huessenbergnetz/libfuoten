/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
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
