/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "folderlistmodel_p.h"

using namespace Fuoten;

FolderListModelPrivate::FolderListModelPrivate() :
    AbstractFolderModelPrivate()
{

}


FolderListModelPrivate::~FolderListModelPrivate()
{

}


FolderListModel::FolderListModel(QObject *parent) :
    AbstractFolderModel(* new FolderListModelPrivate, parent)
{

}


FolderListModel::FolderListModel(FolderListModelPrivate &dd, QObject *parent) :
    AbstractFolderModel(dd, parent)
{

}


FolderListModel::~FolderListModel()
{

}


QModelIndex FolderListModel::index(int row, int column, const QModelIndex &parent) const
{
    return hasIndex(row, column, parent) ? createIndex(row, column) : QModelIndex();
}


QModelIndex FolderListModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return QModelIndex();
}


QModelIndex FolderListModel::sibling(int row, int column, const QModelIndex &idx) const
{
    Q_UNUSED(idx)
    return index(row, column);
}


Qt::ItemFlags FolderListModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags f = QAbstractItemModel::flags(index);
    if (index.isValid()) {
        f |= Qt::ItemNeverHasChildren;
    }

    return f;
}


int FolderListModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : 1;
}


bool FolderListModel::hasChildren(const QModelIndex &parent) const
{
    return parent.isValid() ? false : (rowCount() > 0);
}


int FolderListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    Q_D(const FolderListModel);
    return d->folders.count();
}


QVariant FolderListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || (index.row() >= rowCount())) {
        return QVariant();
    }

    Q_D(const FolderListModel);

    if (role == Qt::DisplayRole) {
        return QVariant::fromValue<Folder*>(d->folders.at(index.row()));
    } else {
        return QVariant();
    }
}

#include "moc_folderlistmodel.cpp"
