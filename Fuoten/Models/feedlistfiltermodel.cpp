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

#include "feedlistfiltermodel_p.h"
#include "../Storage/abstractstorage.h"
#include "../feed.h"
#include <QLocale>
#ifdef QT_DEBUG
#include <QDebug>
#endif

using namespace Fuoten;

FeedListFilterModel::FeedListFilterModel(QObject *parent) :
    BaseFilterModel(* new FeedListFilterModelPrivate, parent)
{
    Q_D(const FeedListFilterModel);
    connect(d->flm.data(), &FeedListModel::inOperationChanged, this, &FeedListFilterModel::inOperationChanged);
    connect(d->flm.data(), &FeedListModel::storageChanged, this, &FeedListFilterModel::storageChanged);
    connect(d->flm.data(), &FeedListModel::parentIdChanged, this, &FeedListFilterModel::parentIdChanged);
    setSourceModel(d->flm.data());
}


FeedListFilterModel::FeedListFilterModel(FeedListFilterModelPrivate &dd, QObject *parent) :
    BaseFilterModel(dd, parent)
{
    Q_D(const FeedListFilterModel);
    connect(d->flm.data(), &FeedListModel::inOperationChanged, this, &FeedListFilterModel::inOperationChanged);
    connect(d->flm.data(), &FeedListModel::storageChanged, this, &FeedListFilterModel::storageChanged);
    connect(d->flm.data(), &FeedListModel::parentIdChanged, this, &FeedListFilterModel::parentIdChanged);
    setSourceModel(d->flm.data());
}


void FeedListFilterModel::load(const QString &locale)
{
    if (!locale.isEmpty()) {
        QLocale::setDefault(QLocale(locale));
    }

    Q_D(FeedListFilterModel);
    if (d->flm) {
        d->flm->load();
        sort(0);
    }
}



bool FeedListFilterModel::inOperation() const
{
    Q_D(const FeedListFilterModel);
    return d->flm ? d->flm->inOperation() : false;
}



AbstractStorage *FeedListFilterModel::storage() const
{
    Q_D(const FeedListFilterModel);
    return d->flm ? d->flm->storage() : nullptr;
}



void FeedListFilterModel::setStorage(AbstractStorage *nStorage)
{
    Q_D(FeedListFilterModel);
    if (d->flm) {
        d->flm->setStorage(nStorage);
    }
}



qint64 FeedListFilterModel::parentId() const
{
    Q_D(const FeedListFilterModel);
    return d->flm ? d->flm->parentId() : -1;
}

void FeedListFilterModel::setParentId(qint64 nParentId)
{
    Q_D(FeedListFilterModel);
    if (d->flm) {
        d->flm->setParentId(nParentId);
    }
}



bool FeedListFilterModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    Feed *l = sourceModel()->data(left).value<Feed*>();
    Feed *r = sourceModel()->data(right).value<Feed*>();
    Feed *lR = l;
    Feed *rR = r;

    if (sortOrder() != Qt::AscendingOrder) {
        lR = r;
        rR = l;
    }

    int c;

    switch(sortingRole()) {
    case FuotenEnums::Name:

        if (sortByFolder()) {
            c = lR->folderName().localeAwareCompare(rR->folderName());
            if (c < 0) {
                return true;
            } else if (c > 0) {
                return false;
            }
        }

        if (respectPinned()) {
            if (l->pinned() && !r->pinned()) {
                return true;
            } else if (!l->pinned() && r->pinned()) {
                return false;
            }
        }

        return lR->title().localeAwareCompare(rR->title()) <= 0;

    case FuotenEnums::FolderName:

        c = l->folderName().localeAwareCompare(r->folderName());
        if (c < 0) {
            return true;
        } else if (c > 0) {
            return false;
        }

        if (respectPinned()) {
            if (l->pinned() && !r->pinned()) {
                return true;
            } else if (!l->pinned() && r->pinned()) {
                return false;
            }
        }

        return lR->title().localeAwareCompare(rR->title()) <= 0;

    case FuotenEnums::UnreadCount:

        if (respectPinned()) {
            if (l->pinned() && !r->pinned()) {
                return true;
            } else if (!l->pinned() && r->pinned()) {
                return false;
            }
        }

        return lR->unreadCount() <= rR->unreadCount();

    default:

        if (respectPinned()) {
            if (l->pinned() && !r->pinned()) {
                return true;
            } else if (!l->pinned() && r->pinned()) {
                return false;
            }
        }

        return lR->id() <= rR->id();
    }

    return true;
}



bool FeedListFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (search().isEmpty() && !hideRead()) {
        return true;
    } else if (search().isEmpty() && hideRead()) {
        return (sourceModel()->data(sourceModel()->index(source_row, 0, source_parent)).value<Feed*>()->unreadCount() > 0);
    } else if (!search().isEmpty() && !hideRead()) {
        return sourceModel()->data(sourceModel()->index(source_row, 0, source_parent)).value<Feed*>()->title().contains(search(), Qt::CaseInsensitive);
    } else {
        Feed *f = sourceModel()->data(sourceModel()->index(source_row, 0, source_parent)).value<Feed*>();
        return (f->title().contains(search(), Qt::CaseInsensitive) && (f->unreadCount() > 0));
    }
}



bool FeedListFilterModel::respectPinned() const { Q_D(const FeedListFilterModel); return d->respectPinned; }

void FeedListFilterModel::setRespectPinned(bool nRespectPinned)
{
    Q_D(FeedListFilterModel);
    if (nRespectPinned != d->respectPinned) {
        d->respectPinned = nRespectPinned;
#ifdef QT_DEBUG
        qDebug() << "Changed respectPinned to" << d->respectPinned;
#endif
        Q_EMIT respectPinnedChanged(respectPinned());
        invalidate();
    }
}

bool FeedListFilterModel::sortByFolder() const { Q_D(const FeedListFilterModel); return d->sortByFolder; }

void FeedListFilterModel::setSortByFolder(bool nSortByFolder)
{
    Q_D(FeedListFilterModel);
    if (nSortByFolder != d->sortByFolder) {
        d->sortByFolder = nSortByFolder;
#ifdef QT_DEBUG
        qDebug() << "Changed sortByFolder to" << d->sortByFolder;
#endif
        Q_EMIT sortByFolderChanged(sortByFolder());
        invalidate();
    }
}




