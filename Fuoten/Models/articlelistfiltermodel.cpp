/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "articlelistfiltermodel_p.h"
#include "../Storage/abstractstorage.h"
#include "../article.h"
#include <QLocale>

using namespace Fuoten;

ArticleListFilterModelPrivate::ArticleListFilterModelPrivate() :
    BaseFilterModelPrivate(),
    alm(new ArticleListModel)
{
    sortOrder = Qt::DescendingOrder;
}


ArticleListFilterModelPrivate::~ArticleListFilterModelPrivate()
{

}


ArticleListFilterModel::ArticleListFilterModel(QObject *parent) :
    BaseFilterModel(* new ArticleListFilterModelPrivate, parent)
{
    Q_D(ArticleListFilterModel);
    connect(d->alm.data(), &ArticleListModel::inOperationChanged, this, &BaseFilterModel::inOperationChanged);
    connect(d->alm.data(), &ArticleListModel::storageChanged, this, &BaseFilterModel::storageChanged);
    connect(d->alm.data(), &ArticleListModel::parentIdChanged, this, &BaseFilterModel::parentIdChanged);
    connect(d->alm.data(), &ArticleListModel::doubleParentIdChanged, this, &BaseFilterModel::doubleParentIdChanged);
    connect(d->alm.data(), &ArticleListModel::parentIdTypeChanged, this, &ArticleListFilterModel::parentIdChanged);
    connect(d->alm.data(), &ArticleListModel::bodyLimitChanged, this, &ArticleListFilterModel::bodyLimitChanged);
    connect(d->alm.data(), &ArticleListModel::loadedChanged, this, &BaseFilterModel::loadedChanged);
    setSourceModel(d->alm.data());
}


ArticleListFilterModel::ArticleListFilterModel(ArticleListFilterModelPrivate &&dd, QObject *parent) :
    BaseFilterModel(dd, parent)
{
    Q_D(ArticleListFilterModel);
    connect(d->alm.data(), &ArticleListModel::inOperationChanged, this, &BaseFilterModel::inOperationChanged);
    connect(d->alm.data(), &ArticleListModel::storageChanged, this, &BaseFilterModel::storageChanged);
    connect(d->alm.data(), &ArticleListModel::parentIdChanged, this, &BaseFilterModel::parentIdChanged);
    connect(d->alm.data(), &ArticleListModel::doubleParentIdChanged, this, &BaseFilterModel::doubleParentIdChanged);
    connect(d->alm.data(), &ArticleListModel::parentIdTypeChanged, this, &ArticleListFilterModel::parentIdChanged);
    connect(d->alm.data(), &ArticleListModel::bodyLimitChanged, this, &ArticleListFilterModel::bodyLimitChanged);
    connect(d->alm.data(), &ArticleListModel::loadedChanged, this, &BaseFilterModel::loadedChanged);
    setSourceModel(d->alm.data());
}


ArticleListFilterModel::~ArticleListFilterModel()
{

}


bool ArticleListFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (search().isEmpty() && !hideRead()) {
        return true;
    } else if (search().isEmpty() && hideRead()) {
        return (sourceModel()->data(sourceModel()->index(source_row, 0, source_parent)).value<Article*>()->unread());
    } else if (!search().isEmpty() && !hideRead()) {
        return find(sourceModel()->data(sourceModel()->index(source_row, 0, source_parent)).value<Article*>()->title());
    } else {
        Article *a = sourceModel()->data(sourceModel()->index(source_row, 0, source_parent)).value<Article*>();
        return (find(a->title()) && a->unread());
    }
}


bool ArticleListFilterModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    Article *l;
    Article *r;

    if (sortOrder() == Qt::AscendingOrder) {
        l = sourceModel()->data(left).value<Article*>();
        r = sourceModel()->data(right).value<Article*>();
    } else {
        r = sourceModel()->data(left).value<Article*>();
        l = sourceModel()->data(right).value<Article*>();
    }

    if (l->pubDate() < r->pubDate()) {
        return true;
    } else if (l->pubDate() > r->pubDate()) {
        return false;
    }

    return l->id() < r->id();
}


bool ArticleListFilterModel::inOperation() const
{
    Q_D(const ArticleListFilterModel);
    return d->alm ? d->alm->inOperation() : false;
}


AbstractStorage *ArticleListFilterModel::storage() const
{
    Q_D(const ArticleListFilterModel);
    return d->alm ? d->alm->storage() : nullptr;
}


void ArticleListFilterModel::setStorage(AbstractStorage *nStorage)
{
    Q_D(ArticleListFilterModel);
    if (d->alm) {
        d->alm->setStorage(nStorage);
    }
}


qint64 ArticleListFilterModel::parentId() const
{
    Q_D(const ArticleListFilterModel);
    return d->alm ? d->alm->parentId() : -1;
}


void ArticleListFilterModel::setParentId(qint64 nParentId)
{
    Q_D(ArticleListFilterModel);
    if (d->alm) {
        d->alm->setParentId(nParentId);
    }
}


void ArticleListFilterModel::load(const QString &locale)
{
    if (!locale.isEmpty()) {
        QLocale::setDefault(QLocale(locale));
    }

    Q_D(ArticleListFilterModel);
    if (d->alm) {
        d->alm->load();
        sort(0);
    }
}


void ArticleListFilterModel::reload(const QString &locale)
{
    if (!locale.isEmpty()) {
        QLocale::setDefault(QLocale(locale));
    }

    Q_D(ArticleListFilterModel);
    if (d->alm) {
        d->alm->reload();
        sort(0);
    }
}


FuotenEnums::Type ArticleListFilterModel::parentIdType() const
{
    Q_D(const ArticleListFilterModel);
    if (d->alm) {
        return d->alm->parentIdType();
    } else {
        return FuotenEnums::All;
    }
}

void ArticleListFilterModel::setParentIdType(FuotenEnums::Type nParentIdType)
{
    Q_D(ArticleListFilterModel);
    if (d->alm) {
        d->alm->setParentIdType(nParentIdType);
    }
}


int ArticleListFilterModel::bodyLimit() const
{
    Q_D(const ArticleListFilterModel);
    if (d->alm) {
        return d->alm->bodyLimit();
    } else {
        return -1;
    }
}

void ArticleListFilterModel::setBodyLimit(int nBodyLimit)
{
    Q_D(ArticleListFilterModel);
    if (d->alm) {
        d->alm->setBodyLimit(nBodyLimit);
    }
}


bool ArticleListFilterModel::loaded() const
{
    Q_D(const ArticleListFilterModel);
    if (d->alm) {
        return d->alm->loaded();
    } else {
        return false;
    }
}

#include "moc_articlelistfiltermodel.cpp"
