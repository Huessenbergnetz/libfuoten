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

#include "folderlistfiltermodel_p.h"
#include "../Storage/abstractstorage.h"
#include "../folder.h"
#include <QLocale>

using namespace Fuoten;

FolderListFilterModel::FolderListFilterModel(QObject *parent) :
    BaseFilterModel(* new FolderListFilterModelPrivate, parent)
{
    Q_D(const FolderListFilterModel);
    connect(d->flm.data(), &FolderListModel::inOperationChanged, this, &FolderListFilterModel::inOperationChanged);
    connect(d->flm.data(), &FolderListModel::storageChanged, this, &FolderListFilterModel::storageChanged);
    connect(d->flm.data(), &FolderListModel::parentIdChanged, this, &FolderListFilterModel::parentIdChanged);
    connect(d->flm.data(), &FolderListModel::doubleParentIdChanged, this, &FolderListFilterModel::doubleParentIdChanged);
    setSourceModel(d->flm.data());
}


FolderListFilterModel::FolderListFilterModel(FolderListFilterModelPrivate &dd, QObject *parent) :
    BaseFilterModel(dd, parent)
{
    Q_D(const FolderListFilterModel);
    connect(d->flm.data(), &FolderListModel::inOperationChanged, this, &FolderListFilterModel::inOperationChanged);
    connect(d->flm.data(), &FolderListModel::storageChanged, this, &FolderListFilterModel::storageChanged);
    connect(d->flm.data(), &FolderListModel::parentIdChanged, this, &FolderListFilterModel::parentIdChanged);
    connect(d->flm.data(), &FolderListModel::doubleParentIdChanged, this, &FolderListFilterModel::doubleParentIdChanged);
    setSourceModel(d->flm.data());
}


void FolderListFilterModel::load(const QString &locale)
{
    if (!locale.isEmpty()) {
        QLocale::setDefault(QLocale(locale));
    }

    Q_D(FolderListFilterModel);
    if (d->flm) {
        d->flm->load();
        sort(0);
    }
}


void FolderListFilterModel::reload(const QString &locale)
{
    if (!locale.isEmpty()) {
        QLocale::setDefault(QLocale(locale));
    }

    Q_D(FolderListFilterModel);
    if (d->flm) {
        d->flm->reload();
        sort(0);
    }
}


bool FolderListFilterModel::inOperation() const
{
    Q_D(const FolderListFilterModel);
    return d->flm ? d->flm->inOperation() : false;
}



AbstractStorage *FolderListFilterModel::storage() const
{
    Q_D(const FolderListFilterModel);
    return d->flm ? d->flm->storage() : nullptr;
}


void FolderListFilterModel::setStorage(AbstractStorage *nStorage)
{
    Q_D(FolderListFilterModel);
    if (d->flm) {
        d->flm->setStorage(nStorage);
    }
}


qint64 FolderListFilterModel::parentId() const
{
    Q_D(const FolderListFilterModel);
    return d->flm ? d->flm->parentId() : 0;
}

void FolderListFilterModel::setParentId(qint64 nParentId)
{
    Q_D(FolderListFilterModel);
    if (d->flm) {
        if (d->flm->parentId() != nParentId) {
            d->flm->setParentId(nParentId);
            Q_EMIT doubleParentIdChanged(doubleParentId());
        }
    }
}



bool FolderListFilterModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    Folder *l = sourceModel()->data(left).value<Folder*>();
    Folder *r = sourceModel()->data(right).value<Folder*>();

    switch(sortingRole()) {
    case FuotenEnums::Name:
        return sortOrder() == Qt::AscendingOrder ? QString::localeAwareCompare(l->name(), r->name()) < 0 : QString::localeAwareCompare(l->name(), r->name()) > 0;
    case FuotenEnums::UnreadCount:
        return sortOrder() == Qt::AscendingOrder ? l->unreadCount()< r->unreadCount() : l->unreadCount() > r->unreadCount();
    case FuotenEnums::FeedCount:
        return sortOrder() == Qt::AscendingOrder ? l->feedCount() < r->feedCount() : l->feedCount() > r->feedCount();
    default:
        return sortOrder() == Qt::AscendingOrder ? l->id() < r->id() : l->id() > r->id();
    }
}



bool FolderListFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (search().isEmpty() && !hideRead()) {
        return true;
    } else if (search().isEmpty() && hideRead()) {
        return (sourceModel()->data(sourceModel()->index(source_row, 0, source_parent)).value<Folder*>()->unreadCount() > 0);
    } else if (!search().isEmpty() && !hideRead()) {
        return find(sourceModel()->data(sourceModel()->index(source_row, 0, source_parent)).value<Folder*>()->name());
    } else {
        Folder *f = sourceModel()->data(sourceModel()->index(source_row, 0, source_parent)).value<Folder*>();
        return (find(f->name()) && (f->unreadCount() > 0));
    }
}
