/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "folderlistfiltermodel_p.h"
#include "../Storage/abstractstorage.h"
#include "../folder.h"
#include <QLocale>

using namespace Fuoten;

FolderListFilterModelPrivate::FolderListFilterModelPrivate() :
    BaseFilterModelPrivate(),
    flm(new FolderListModel)
{
    sortingRole = FuotenEnums::Name;
}


FolderListFilterModelPrivate::~FolderListFilterModelPrivate()
{

}


FolderListFilterModel::FolderListFilterModel(QObject *parent) :
    BaseFilterModel(* new FolderListFilterModelPrivate, parent)
{
    Q_D(const FolderListFilterModel);
    connect(d->flm.data(), &FolderListModel::inOperationChanged, this, &FolderListFilterModel::inOperationChanged);
    connect(d->flm.data(), &FolderListModel::storageChanged, this, &FolderListFilterModel::storageChanged);
    connect(d->flm.data(), &FolderListModel::parentIdChanged, this, &FolderListFilterModel::parentIdChanged);
    connect(d->flm.data(), &FolderListModel::doubleParentIdChanged, this, &FolderListFilterModel::doubleParentIdChanged);
    connect(d->flm.data(), &FolderListModel::loadedChanged, this, &FolderListFilterModel::loadedChanged);
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
    connect(d->flm.data(), &FolderListModel::loadedChanged, this, &FolderListFilterModel::loadedChanged);
    setSourceModel(d->flm.data());
}


FolderListFilterModel::~FolderListFilterModel()
{

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


bool FolderListFilterModel::loaded() const
{
    Q_D(const FolderListFilterModel);
    if (d->flm) {
        return d->flm->loaded();
    } else {
        return false;
    }
}

#include "moc_folderlistfiltermodel.cpp"
