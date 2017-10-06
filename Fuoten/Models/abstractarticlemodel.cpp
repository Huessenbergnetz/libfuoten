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

#include "abstractarticlemodel_p.h"
#include "../Storage/abstractstorage.h"
#include "../API/component.h"
#include <QMetaEnum>

using namespace Fuoten;

AbstractArticleModel::AbstractArticleModel(QObject *parent) :
    BaseModel(* new AbstractArticleModelPrivate, parent)
{
    setStorage(Component::defaultStorage());
}


AbstractArticleModel::AbstractArticleModel(AbstractArticleModelPrivate &dd, QObject *parent) :
    BaseModel(dd, parent)
{
    setStorage(Component::defaultStorage());
}


FuotenEnums::Type AbstractArticleModel::parentIdType() const { Q_D(const AbstractArticleModel); return d->parentIdType; }

void AbstractArticleModel::setParentIdType(FuotenEnums::Type nParentIdType)
{
    Q_D(AbstractArticleModel);
    if (nParentIdType != d->parentIdType) {
        d->parentIdType = nParentIdType;
        qDebug("Changed parentIdType to %s.", FuotenEnums::staticMetaObject.enumerator(FuotenEnums::staticMetaObject.indexOfEnumerator("Type")).valueToKey(d->parentIdType));
        Q_EMIT parentIdTypeChanged(parentIdType());
    }
}


bool AbstractArticleModel::starredOnly() const { Q_D(const AbstractArticleModel); return d->starredOnly; }

void AbstractArticleModel::setStarredOnly(bool nStarredOnly)
{
    Q_D(AbstractArticleModel);
    if (nStarredOnly != d->starredOnly) {
        d->starredOnly = nStarredOnly;
        qDebug("Changed starredOnly to %s.", d->starredOnly ? "true" : "false");
        Q_EMIT starredOnlyChanged(starredOnly());
    }
}


int AbstractArticleModel::bodyLimit() const { Q_D(const AbstractArticleModel); return d->bodyLimit; }

void AbstractArticleModel::setBodyLimit(int nBodyLimit)
{
    Q_D(AbstractArticleModel);
    if (nBodyLimit != d->bodyLimit) {
        d->bodyLimit = nBodyLimit;
        qDebug("Changed bodyLimit to %i.", d->bodyLimit);
        Q_EMIT bodyLimitChanged(bodyLimit());
    }
}


void AbstractArticleModel::handleStorageChanged(AbstractStorage *old)
{
    if (old) {
        old->disconnect(this);
    }

    AbstractStorage *s = storage();

    if (s) {
        connect(s, &AbstractStorage::gotArticlesAsync, this, &AbstractArticleModel::gotArticlesAsync);
        connect(s, &AbstractStorage::requestedItems, this, &AbstractArticleModel::itemsRequested);
        connect(s, &AbstractStorage::markedReadFolder, this, &AbstractArticleModel::folderMarkedRead);
        connect(s, &AbstractStorage::markedReadFolderInQueue, this, &AbstractArticleModel::folderMarkedReadInQueue);
        connect(s, &AbstractStorage::markedReadFeed, this, &AbstractArticleModel::feedMarkedRead);
        connect(s, &AbstractStorage::markedReadFeedInQueue, this, &AbstractArticleModel::feedMarkedReadInQueue);
        connect(s, &AbstractStorage::deletedFolder, this, &AbstractArticleModel::folderDeleted);
        connect(s, &AbstractStorage::deletedFeed, this, &AbstractArticleModel::feedDeleted);
        connect(s, &AbstractStorage::markedItem, this, &AbstractArticleModel::itemMarked);
        connect(s, &AbstractStorage::markedItems, this, &AbstractArticleModel::itemsMarked);
        connect(s, &AbstractStorage::starredItem, this, &AbstractArticleModel::itemStarred);
        connect(s, &AbstractStorage::starredItems, this, &AbstractArticleModel::itemsStarred);
        connect(s, &AbstractStorage::markedAllItemsRead, this, &AbstractArticleModel::allItemsMarkedRead);
        connect(s, &AbstractStorage::markedAllItemsReadInQueue, this, &AbstractArticleModel::allItemsMarkedReadInQueue);
        connect(s, &AbstractStorage::queueCleared, this, &AbstractArticleModel::queueCleared);
    }
}


void AbstractArticleModel::load()
{
    Q_ASSERT_X(storage(), "load articles", "no storage available");

    if (!storage()->ready() || loaded() || inOperation()) {
        return;
    }

    setInOperation(true);

    QueryArgs qa;
    qa.sortingRole = sortingRole();
    qa.sortOrder = sortOrder();
    qa.parentId = parentId();
    qa.parentIdType = parentIdType();
    qa.bodyLimit = bodyLimit();

    if ((parentId() < 0) && (parentIdType() == FuotenEnums::Starred)) {
        qa.starredOnly = true;
    }

    storage()->getArticlesAsync(qa);
}


void AbstractArticleModel::gotArticlesAsync(const ArticleList &articles)
{
    if (Q_LIKELY(!articles.isEmpty())) {

        Q_D(AbstractArticleModel);

        beginInsertRows(QModelIndex(), rowCount(), rowCount() + articles.count() -1);

        for (Article *a : articles) {
            if (a->thread() != this->thread()) {
                d->articles.append(new Article(a));
                delete a;
            } else {
                d->articles.append(a);
            }
        }

        endInsertRows();
    }

    setLoaded(true);

    setInOperation(false);
}


QModelIndex AbstractArticleModel::findByID(qint64 id) const
{
    QModelIndex idx;

    Q_D(const AbstractArticleModel);

    if (Q_UNLIKELY(d->articles.isEmpty())) {
        return idx;
    }

    int idxInt = -1;

    for (int i = 0; i < d->articles.count(); ++i) {
        if (d->articles.at(i)->id() == id) {
            idxInt = i;
            break;
        }
    }

    if (idxInt > -1) {
        idx = index(idxInt, 0);
    }

    return idx;
}


QHash<qint64, QModelIndex> AbstractArticleModel::findByIDs(const IdList &ids) const
{
    QHash<qint64, QModelIndex> idxs;

    Q_D(const AbstractArticleModel);

    if (Q_UNLIKELY(d->articles.isEmpty())) {
        return idxs;
    }

    for (int i = 0; i < d->articles.count(); ++i) {
        Article *a = d->articles.at(i);
        if (ids.contains(a->id())) {
            idxs.insert(a->id(), index(i, 0));
        }
    }

    return idxs;
}



QList<Article*> AbstractArticleModel::articles() const
{
    Q_D(const AbstractArticleModel);
    return d->articles;
}


void AbstractArticleModel::clear()
{
    Q_D(AbstractArticleModel);

    if (Q_LIKELY(!d->articles.isEmpty())) {

        beginRemoveRows(QModelIndex(), 0, rowCount() - 1);

        qDeleteAll(d->articles);
        d->articles.clear();

        endRemoveRows();
    }
}


void AbstractArticleModel::itemsRequested(const IdList &updatedItems, const IdList &newItems, const IdList &deletedItems)
{
    Q_ASSERT_X(storage(), "update articles", "no storage available");

    if (rowCount() == 0) {
        reload();
        return;
    }

    Q_D(AbstractArticleModel);

    if (!updatedItems.isEmpty()) {

        QHash<qint64, QModelIndex> idxs = findByIDs(updatedItems);

        if (!idxs.isEmpty()) {

            QueryArgs qa;
            qa.parentId = parentId();
            qa.parentIdType = parentIdType();
            qa.inIds = idxs.keys();
            qa.inIdsType = FuotenEnums::Item;
            qa.bodyLimit = bodyLimit();
            if ((parentId() < 0) && (parentIdType() == FuotenEnums::Starred)) {
                qa.starredOnly = true;
            }
            const QList<Article*> upits = storage()->getArticles(qa);

            if (!upits.isEmpty()) {
                for (Article *a : upits) {

                    QModelIndex idx = idxs.value(a->id());
                    d->articles.at(idx.row())->copy(a);
                    Q_EMIT dataChanged(idx, idx, QVector<int>(1, Qt::DisplayRole));
                }
                qDeleteAll(upits);
            }
        }
    }

    if (!newItems.isEmpty()) {

        QueryArgs qa;
        qa.parentId = parentId();
        qa.parentIdType = parentIdType();
        qa.inIds = newItems;
        qa.inIdsType = FuotenEnums::Item;
        qa.bodyLimit = bodyLimit();
        if ((parentId() < 0) && (parentIdType() == FuotenEnums::Starred)) {
            qa.starredOnly = true;
        }
        const QList<Article*> newits = storage()->getArticles(qa);

        if (!newits.isEmpty()) {

            beginInsertRows(QModelIndex(), rowCount(), rowCount() + newits.count() -1);

            d->articles.append(newits);

            endInsertRows();
        }
    }

    if (!deletedItems.isEmpty()) {

        for (qint64 id : deletedItems) {

            int row = d->rowByID(id);

            if (row > -1) {

                beginRemoveRows(QModelIndex(), row, row);

                delete d->articles.takeAt(row);

                endRemoveRows();
            }
        }
    }
}


void AbstractArticleModel::folderMarkedRead(qint64 folderId, qint64 newestItemId)
{
    if (rowCount() <= 0) {
        return;
    }

    Q_D(AbstractArticleModel);

    const ArticleList as = d->articles;

    if (as.isEmpty()) {
        return;
    }

    for (Article *a : as) {

        if (a->unread() && (a->folderId() == folderId) && (a->id() <= newestItemId)) {
            QModelIndex idx = findByID(a->id());
            if (idx.isValid()) {
                a->setUnread(false);
                Q_EMIT dataChanged(idx, idx, QVector<int>(1, Qt::DisplayRole));
            }
        }
    }
}


void AbstractArticleModel::folderMarkedReadInQueue(qint64 folderId, qint64 newestItemId)
{
    if (rowCount() <= 0) {
        return;
    }

    Q_D(AbstractArticleModel);

    const ArticleList as = d->articles;

    if (as.isEmpty()) {
        return;
    }

    for (Article *a : as) {

        if (a->unread() && (a->folderId() == folderId) && (a->id() <= newestItemId)) {
            QModelIndex idx = findByID(a->id());
            if (idx.isValid()) {
                FuotenEnums::QueueActions qa = a->queue();
                if (qa.testFlag(FuotenEnums::MarkAsUnread)) {
                    qa ^= FuotenEnums::MarkAsUnread;
                } else {
                    qa |= FuotenEnums::MarkAsRead;
                }
                a->setQueue(qa);
                a->setUnread(false);
                Q_EMIT dataChanged(idx, idx, QVector<int>(1, Qt::DisplayRole));
            }
        }
    }
}


void AbstractArticleModel::feedMarkedRead(qint64 feedId, qint64 newestItemId)
{
    if (rowCount() <= 0) {
        return;
    }

    if ((parentIdType() == FuotenEnums::Feed) && (parentId() != feedId)) {
        return;
    }

    Q_D(AbstractArticleModel);

    const ArticleList as = d->articles;

    if (as.isEmpty()) {
        return;
    }

    for (Article *a : as) {

        if (a->unread() && (a->feedId() == feedId) && (a->id() <= newestItemId)) {
            QModelIndex idx = findByID(a->id());
            if (idx.isValid()) {
                a->setUnread(false);
                Q_EMIT dataChanged(idx, idx, QVector<int>(1, Qt::DisplayRole));
            }
        }

    }
}


void AbstractArticleModel::feedMarkedReadInQueue(qint64 feedId, qint64 newestItemId)
{
    if (rowCount() <= 0) {
        return;
    }

    if ((parentIdType() == FuotenEnums::Feed) && (parentId() != feedId)) {
        return;
    }

    Q_D(AbstractArticleModel);

    const ArticleList as = d->articles;

    if (as.isEmpty()) {
        return;
    }

    for (Article *a : as) {

        if (a->unread() && (a->feedId() == feedId) && (a->id() <= newestItemId)) {

            QModelIndex idx = findByID(a->id());
            if (idx.isValid()) {

                FuotenEnums::QueueActions qa = a->queue();
                if (qa.testFlag(FuotenEnums::MarkAsUnread)) {
                    qa ^= FuotenEnums::MarkAsUnread;
                } else {
                    qa |= FuotenEnums::MarkAsRead;
                }
                a->setQueue(qa);
                a->setUnread(false);
                Q_EMIT dataChanged(idx, idx, QVector<int>(1, Qt::DisplayRole));
            }
        }
    }
}


void AbstractArticleModel::folderDeleted(qint64 folderId)
{
    Q_ASSERT_X(storage(), "remove articles", "no storage available");

    if (rowCount() <= 0) {
        return;
    }

    IdList idsToDelete;
    const ArticleList as = articles();
    for (Article *a : as) {
        if (a->folderId() == folderId) {
            idsToDelete.append(a->id());
        }
    }

    if (!idsToDelete.isEmpty()) {

        Q_D(AbstractArticleModel);

        for (int i = 0; i < idsToDelete.count(); ++i) {
            int row = d->rowByID(idsToDelete.at(i));
            if (row > -1) {
                beginRemoveRows(QModelIndex(), row, row);

                Article *a = d->articles.takeAt(row);

                endRemoveRows();

                a->deleteLater();
            }
        }
    }
}


void AbstractArticleModel::feedDeleted(qint64 feedId)
{
    Q_ASSERT_X(storage(), "remove articles", "no storage available");

    if (rowCount() <= 0) {
        return;
    }

    IdList idsToDelete;
    const ArticleList as = articles();
    for (Article *a : as) {
        if (a->feedId() == feedId) {
            idsToDelete.append(a->id());
        }
    }

    if (!idsToDelete.isEmpty()) {

        Q_D(AbstractArticleModel);

        for (int i = 0; i < idsToDelete.count(); ++i) {
            int row = d->rowByID(idsToDelete.at(i));
            if (row > -1) {
                beginRemoveRows(QModelIndex(), row, row);

                Article *a = d->articles.takeAt(row);

                endRemoveRows();

                a->deleteLater();
            }
        }
    }
}


void AbstractArticleModel::itemMarked(qint64 itemId, bool unread)
{
    if (rowCount() <= 0) {
        return;
    }

    QModelIndex idx = findByID(itemId);

    if (idx.isValid()) {
        Q_D(AbstractArticleModel);

        d->articles.at(idx.row())->setUnread(unread);

        Q_EMIT dataChanged(idx, idx, QVector<int>(1, Qt::DisplayRole));
    }
}


void AbstractArticleModel::itemsMarked(const IdList &itemIds, bool unread)
{
    Q_ASSERT_X(!itemIds.isEmpty(), "mark items as read", "empty id list");

    if (rowCount() <= 0) {
        return;
    }

    QHash<qint64, QModelIndex> idxs = findByIDs(itemIds);

    if (!idxs.isEmpty()) {

        Q_D(AbstractArticleModel);

        QHash<qint64, QModelIndex>::const_iterator i = idxs.constBegin();
        while (i != idxs.constEnd()) {
            d->articles.at(i.value().row())->setUnread(unread);
            Q_EMIT dataChanged(i.value(), i.value(), QVector<int>(Qt::DisplayRole));
        }
    }
}


void AbstractArticleModel::itemStarred(qint64 feedId, const QString &guidHash, bool starred)
{
    if ((parentId() > 0) && (parentIdType() == FuotenEnums::Feed) && (parentId() != feedId)) {
        return;
    }

    if (rowCount() <= 0) {
        return;
    }

    Q_D(AbstractArticleModel);

    int row = d->rowByGuidHash(guidHash);

    if (row > -1) {
        Article *a = d->articles.at(row);
        if (a->feedId() == feedId) {
            a->setStarred(starred);
            Q_EMIT dataChanged(index(row, 0), index(row, 0), QVector<int>(1, Qt::DisplayRole));
        }
    }
}


void AbstractArticleModel::itemsStarred(const QList<QPair<qint64, QString> > &articles, bool starred)
{
    Q_ASSERT_X(!articles.empty(), "star items", "empty articles list");

    if (rowCount() <= 0) {
        return;
    }

    for (const QPair<qint64, QString> &p : articles) {
        itemStarred(p.first, p.second, starred);
    }
}


void AbstractArticleModel::allItemsMarkedRead(qint64 newestItemId)
{
    if (rowCount() <= 0) {
        return;
    }

    const ArticleList as = articles();
    for (Article *a : as) {
        if (a->unread() && (a->id() <= newestItemId)) {
            a->setUnread(false);
        }
    }

    Q_EMIT dataChanged(index(0, 0), index(rowCount()-1, 0), QVector<int>(1, Qt::DisplayRole));
}


void AbstractArticleModel::allItemsMarkedReadInQueue()
{
    if (rowCount() <= 0) {
        return;
    }

    const ArticleList as = articles();

    for (Article *a : as) {
        if (a->unread()) {
            FuotenEnums::QueueActions qa = a->queue();
            if (qa.testFlag(FuotenEnums::MarkAsUnread)) {
                qa ^= FuotenEnums::MarkAsUnread;
            } else {
                qa |= FuotenEnums::MarkAsRead;
            }
            a->setQueue(qa);
            a->setUnread(false);
        }
    }

    Q_EMIT dataChanged(index(0, 0), index(rowCount()-1, 0), QVector<int>(1, Qt::DisplayRole));
}


void AbstractArticleModel::queueCleared()
{
    if (rowCount() <= 0) {

    }

    const ArticleList as = articles();

    for (Article *a : as) {
        a->setQueue(FuotenEnums::QueueActions(0));
    }
}

#include "moc_abstractarticlemodel.cpp"
