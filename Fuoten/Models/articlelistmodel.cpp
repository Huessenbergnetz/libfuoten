/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016-2020 Hüssenbergnetz/Matthias Fehring
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

#include "articlelistmodel_p.h"
#include "../article.h"

using namespace Fuoten;


ArticleListModelPrivate::ArticleListModelPrivate() :
    AbstractArticleModelPrivate()
{
}

ArticleListModelPrivate::~ArticleListModelPrivate()
{
}


ArticleListModel::ArticleListModel(QObject *parent) :
    AbstractArticleModel(* new ArticleListModelPrivate, parent)
{
}


ArticleListModel::ArticleListModel(ArticleListModelPrivate &dd, QObject *parent) :
    AbstractArticleModel(dd, parent)
{
}


ArticleListModel::~ArticleListModel()
{
}


QModelIndex ArticleListModel::index(int row, int column, const QModelIndex &parent) const
{
    return hasIndex(row, column, parent) ? createIndex(row, column) : QModelIndex();
}


QModelIndex ArticleListModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return QModelIndex();
}


QModelIndex ArticleListModel::sibling(int row, int column, const QModelIndex &idx) const
{
    Q_UNUSED(idx)
    return index(row, column);
}


Qt::ItemFlags ArticleListModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags f = QAbstractItemModel::flags(index);
    if (index.isValid()) {
        f |= Qt::ItemNeverHasChildren;
    }

    return f;
}


int ArticleListModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : 1;
}


bool ArticleListModel::hasChildren(const QModelIndex &parent) const
{
    return parent.isValid() ? false : (rowCount() > 0);
}


int ArticleListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    Q_D(const ArticleListModel);
    return d->articles.count();
}


QVariant ArticleListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || (index.row() >= rowCount())) {
        return QVariant();
    }

    Q_D(const ArticleListModel);

    if (role == Qt::DisplayRole) {
        return QVariant::fromValue<Article*>(d->articles.at(index.row()));
    } else {
        return QVariant();
    }
}

#include "moc_articlelistmodel.cpp"
