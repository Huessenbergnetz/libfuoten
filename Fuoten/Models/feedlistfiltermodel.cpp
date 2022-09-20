/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "feedlistfiltermodel_p.h"
#include "../Storage/abstractstorage.h"
#include "../feed.h"
#include <QLocale>

using namespace Fuoten;

FeedListFilterModelPrivate::FeedListFilterModelPrivate() :
    BaseFilterModelPrivate(),
    flm(new FeedListModel),
    respectPinned(true),
    sortByFolder(false)
{
    sortingRole = FuotenEnums::Name;
}


FeedListFilterModelPrivate::~FeedListFilterModelPrivate()
{

}


FeedListFilterModel::FeedListFilterModel(QObject *parent) :
    BaseFilterModel(* new FeedListFilterModelPrivate, parent)
{
    Q_D(const FeedListFilterModel);
    connect(d->flm.data(), &FeedListModel::inOperationChanged, this, &FeedListFilterModel::inOperationChanged);
    connect(d->flm.data(), &FeedListModel::storageChanged, this, &FeedListFilterModel::storageChanged);
    connect(d->flm.data(), &FeedListModel::parentIdChanged, this, &FeedListFilterModel::parentIdChanged);
    connect(d->flm.data(), &FeedListModel::doubleParentIdChanged, this, &FeedListFilterModel::doubleParentIdChanged);
    connect(d->flm.data(), &FeedListModel::loadedChanged, this, &FeedListFilterModel::loadedChanged);
    setSourceModel(d->flm.data());
}


FeedListFilterModel::FeedListFilterModel(FeedListFilterModelPrivate &dd, QObject *parent) :
    BaseFilterModel(dd, parent)
{
    Q_D(const FeedListFilterModel);
    connect(d->flm.data(), &FeedListModel::inOperationChanged, this, &FeedListFilterModel::inOperationChanged);
    connect(d->flm.data(), &FeedListModel::storageChanged, this, &FeedListFilterModel::storageChanged);
    connect(d->flm.data(), &FeedListModel::parentIdChanged, this, &FeedListFilterModel::parentIdChanged);
    connect(d->flm.data(), &FeedListModel::doubleParentIdChanged, this, &FeedListFilterModel::doubleParentIdChanged);
    connect(d->flm.data(), &FeedListModel::loadedChanged, this, &FeedListFilterModel::loadedChanged);
    setSourceModel(d->flm.data());
}


FeedListFilterModel::~FeedListFilterModel()
{

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


void FeedListFilterModel::reload(const QString &locale)
{
    if (!locale.isEmpty()) {
        QLocale::setDefault(QLocale(locale));
    }

    Q_D(FeedListFilterModel);
    if (d->flm) {
        d->flm->reload();
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
        if (d->flm->parentId() != nParentId) {
            d->flm->setParentId(nParentId);
            Q_EMIT doubleParentIdChanged(doubleParentId());
        }
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
        return find(sourceModel()->data(sourceModel()->index(source_row, 0, source_parent)).value<Feed*>()->title());
    } else {
        Feed *f = sourceModel()->data(sourceModel()->index(source_row, 0, source_parent)).value<Feed*>();
        return (find(f->title()) && (f->unreadCount() > 0));
    }
}


bool FeedListFilterModel::respectPinned() const { Q_D(const FeedListFilterModel); return d->respectPinned; }

void FeedListFilterModel::setRespectPinned(bool nRespectPinned)
{
    Q_D(FeedListFilterModel);
    if (nRespectPinned != d->respectPinned) {
        d->respectPinned = nRespectPinned;
        qDebug("Changed respectPinned to %s.", d->respectPinned ? "true" : "false");
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
        qDebug("Changed sortByFolder to %s.", d->sortByFolder ? "true" : "false");
        Q_EMIT sortByFolderChanged(sortByFolder());
        invalidate();
    }
}


bool FeedListFilterModel::loaded() const
{
    Q_D(const FeedListFilterModel);
    if (d->flm) {
        return d->flm->loaded();
    } else {
        return false;
    }
}

#include "moc_feedlistfiltermodel.cpp"
