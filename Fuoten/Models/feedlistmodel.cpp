/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "feedlistmodel_p.h"
#include "../feed.h"

using namespace Fuoten;

FeedListModelPrivate::FeedListModelPrivate() :
    AbstractFeedModelPrivate()
{

}


FeedListModelPrivate::~FeedListModelPrivate()
{

}


FeedListModel::FeedListModel(QObject *parent) :
    AbstractFeedModel(* new FeedListModelPrivate, parent)
{

}


FeedListModel::FeedListModel(FeedListModelPrivate &dd, QObject *parent) :
    AbstractFeedModel(dd, parent)
{

}


FeedListModel::~FeedListModel()
{

}


QModelIndex FeedListModel::index(int row, int column, const QModelIndex &parent) const
{
    return hasIndex(row, column, parent) ? createIndex(row, column) : QModelIndex();
}


QModelIndex FeedListModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return QModelIndex();
}


QModelIndex FeedListModel::sibling(int row, int column, const QModelIndex &idx) const
{
    Q_UNUSED(idx)
    return index(row, column);
}


Qt::ItemFlags FeedListModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags f = QAbstractItemModel::flags(index);
    if (index.isValid()) {
        f |= Qt::ItemNeverHasChildren;
    }

    return f;
}


int FeedListModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : 1;
}


bool FeedListModel::hasChildren(const QModelIndex &parent) const
{
    return parent.isValid() ? false : (rowCount() > 0);
}


int FeedListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    Q_D(const FeedListModel);
    return d->feeds.count();
}


QVariant FeedListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || (index.row() >= rowCount())) {
        return QVariant();
    }

    Q_D(const FeedListModel);

    if (role == Qt::DisplayRole) {
        return QVariant::fromValue<Feed*>(d->feeds.at(index.row()));
    } else {
        return QVariant();
    }
}

#include "moc_feedlistmodel.cpp"
