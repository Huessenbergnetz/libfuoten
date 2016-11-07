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

#include "abstractfeedmodel_p.h"
#include "../Storage/abstractstorage.h"
#include "../article.h"
#ifdef QT_DEBUG
#include <QtDebug>
#endif

using namespace Fuoten;

AbstractFeedModel::AbstractFeedModel(QObject *parent) :
    BaseModel(* new AbstractFeedModelPrivate, parent)
{

}


AbstractFeedModel::AbstractFeedModel(AbstractFeedModelPrivate &dd, QObject *parent) :
    BaseModel(dd, parent)
{

}



void AbstractFeedModel::handleStorageChanged()
{
    AbstractStorage *s = storage();
    connect(s, &AbstractStorage::readyChanged, this, &AbstractFeedModel::load);

    connect(s, &AbstractStorage::markedReadFolder, this, &AbstractFeedModel::folderMarkedRead);
    connect(s, &AbstractStorage::deletedFolder, this, &AbstractFeedModel::folderDeleted);

    connect(s, &AbstractStorage::requestedFeeds, this, &AbstractFeedModel::feedsRequested);
    connect(s, &AbstractStorage::createdFeed, this, &AbstractFeedModel::feedCreated);
    connect(s, &AbstractStorage::deletedFeed, this, &AbstractFeedModel::feedDeleted);
    connect(s, &AbstractStorage::movedFeed, this, &AbstractFeedModel::feedMoved);
    connect(s, &AbstractStorage::renamedFeed, this, &AbstractFeedModel::feedRenamed);
    connect(s, &AbstractStorage::markedReadFeed, this, &AbstractFeedModel::feedMarkedRead);

    connect(s, &AbstractStorage::requestedItems, this, &AbstractFeedModel::itemsRquested);
    connect(s, &AbstractStorage::markedItem, this, &AbstractFeedModel::itemMarked);
    connect(s, &AbstractStorage::markedItems, this, &AbstractFeedModel::itemsMarked);
    connect(s, &AbstractStorage::markedAllItemsRead, this, &AbstractFeedModel::itemsMarked);
}



void AbstractFeedModel::load()
{
    if (!storage()) {
        qWarning("Can not load feeds, no storage available.");
        return;
    }

    if (!storage()->ready() || loaded()) {
        return;
    }

    setInOperation(true);

    QueryArgs qa;
    qa.parentId = parentId();

    const QList<Feed*> fs = storage()->getFeeds(qa);

    if (!fs.isEmpty()) {

        Q_D(AbstractFeedModel);

        beginInsertRows(QModelIndex(), 0, fs.count() - 1);

        d->feeds = fs;

        endInsertRows();
    }

    setLoaded(true);

    setInOperation(false);
}



QModelIndex AbstractFeedModel::findByID(qint64 id) const
{
    Q_D(const AbstractFeedModel);
    if (d->feeds.isEmpty()) {
        return QModelIndex();
    }

    int idx = -1;

    for (int i = 0; i < d->feeds.count(); ++i) {
        if (d->feeds.at(i)->id() == id) {
            idx = i;
            break;
        }
    }

    return (idx > -1) ? index(idx, 0) : QModelIndex();
}



QHash<qint64, QModelIndex> AbstractFeedModel::findByIDs(const IdList &ids) const
{
    Q_D(const AbstractFeedModel);

    QHash<qint64, QModelIndex> idxs;

    if (d->feeds.isEmpty()) {
        return idxs;
    }

    for (int i = 0; i < d->feeds.count(); ++i) {
        if (ids.contains(d->feeds.at(i)->id())) {
            idxs.insert(d->feeds.at(i)->id(), index(i, 0));
        }
    }

    return idxs;
}



QList<Feed*> AbstractFeedModel::feeds() const
{
    Q_D(const AbstractFeedModel);
    return d->feeds;
}



void AbstractFeedModel::feedsRequested(const IdList &updatedFeeds, const IdList &newFeeds, const IdList &deletedFeeds)
{
    if (!storage()) {
        qWarning("Can not load feeds, no storage available.");
        return;
    }

#ifdef QT_DEBUG
    qDebug() << "Feeds requested. Updating feeds model.";
#endif

    Q_D(AbstractFeedModel);

    if (!updatedFeeds.isEmpty()) {

        // find all feed IDs and the model indices of the updated feeds that are part of this model instance
        QHash<qint64, QModelIndex> updIxs = findByIDs(updatedFeeds);

        // request the updated feed data from the local storage
        if (!updIxs.isEmpty()) {

            QueryArgs qa;
            qa.inIds = updIxs.keys();
            qa.inIdsType = FuotenEnums::Feed;
            const QList<Feed*> ufs = storage()->getFeeds(qa);

            if (!ufs.isEmpty()) {

                IdList movedIds;

                for (Feed *f : ufs) {
                    QModelIndex idx = updIxs.value(f->id());

                    if (idx.isValid()) {
                        Feed *mf = d->feeds.at(idx.row());

                        if ((parentId() < 0) || (f->folderId() == parentId())) {
                            // the feed has not moved, let's copy the new data
                            mf->copy(f);
                        } else {
                            // the feed is not longer part of this folder
                            movedIds.append(f->id());
                        }
                        Q_EMIT dataChanged(idx, idx, QVector<int>(1, Qt::DisplayRole));
                    }
                }
                qDeleteAll(ufs);

                // remove moved feeds from the model
                if (!movedIds.isEmpty()) {
                    for (qint64 mid : movedIds) {
                        int row = d->rowByID(mid);
                        if (row > -1) {

                            beginRemoveRows(QModelIndex(), row, row);

                            delete d->feeds.takeAt(row);

                            endRemoveRows();
                        }
                    }
                }
            }
        }
    }





    if (!newFeeds.isEmpty()) {

        QueryArgs qa;
        qa.parentId = parentId();
        qa.inIds = newFeeds;
        qa.inIdsType = FuotenEnums::Feed;
        const QList<Feed*> nfs = storage()->getFeeds(qa);

        if (!nfs.isEmpty()) {

            beginInsertRows(QModelIndex(), rowCount(), rowCount() + nfs.count() - 1);

            d->feeds.append(nfs);

            endInsertRows();
        }
    }






    if (!deletedFeeds.isEmpty()) {

        for (qint64 id : deletedFeeds) {

            int row = d->rowByID(id);
            if (row > -1) {

                beginRemoveRows(QModelIndex(), row, row);

                delete d->feeds.takeAt(row);

                endRemoveRows();
            }
        }
    }
}



void AbstractFeedModel::feedCreated(qint64 id, qint64 folderId)
{
    if (!storage()) {
        qWarning("Can not add feed, no storage available.");
        return;
    }

    if (parentId() < 0 || parentId() == folderId) {

        Feed *f = storage()->getFeed(id);

        if (f) {

            Q_D(AbstractFeedModel);

            beginInsertRows(QModelIndex(), rowCount(), rowCount());

            d->feeds.append(f);

            endInsertRows();
        }
    }
}


void AbstractFeedModel::feedDeleted(qint64 id)
{
    Q_D(AbstractFeedModel);

    int row = d->rowByID(id);

    if (row > -1) {

        beginRemoveRows(QModelIndex(), row, row);

        Feed *f = d->feeds.takeAt(row);

        endRemoveRows();

        f->deleteLater();
    }
}




void AbstractFeedModel::feedRenamed(qint64 id, const QString &newName)
{    
    QModelIndex idx = findByID(id);

    if (idx.isValid()) {

        Q_D(AbstractFeedModel);

        d->feeds.at(idx.row())->setTitle(newName);

        Q_EMIT dataChanged(idx, idx, QVector<int>(1, Qt::DisplayRole));

    }
}


void AbstractFeedModel::feedMarkedRead(qint64 id, qint64 newestItemId)
{
    Q_UNUSED(newestItemId)

    QModelIndex idx = findByID(id);

    if (idx.isValid()) {

        Q_D(AbstractFeedModel);

        d->feeds.at(idx.row())->setUnreadCount(0);

        Q_EMIT dataChanged(idx, idx, QVector<int>(1, Qt::DisplayRole));
    }
}


void AbstractFeedModel::feedMoved(qint64 id, qint64 targetFolderId)
{
    if (!storage()) {
        qWarning("Can not move feed, no storage available.");
        return;
    }

    Feed *f = storage()->getFeed(id);

    if (!f) {
        qWarning("Can not find feed in local storage.");
        return;
    }

    QModelIndex idx = findByID(id);

    Q_D(AbstractFeedModel);

    if (idx.isValid() && (parentId() < 0)) {

        d->feeds.at(idx.row())->setFolderId(f->folderId());
        d->feeds.at(idx.row())->setFolderName(f->folderName());

        Q_EMIT dataChanged(idx, idx, QVector<int>(1, Qt::DisplayRole));

        delete f;

    } else if (idx.isValid() && (parentId() != targetFolderId)) {

        beginRemoveRows(QModelIndex(), idx.row(), idx.row());

        Feed *movedFeed = d->feeds.takeAt(idx.row());

        endRemoveRows();

        movedFeed->deleteLater();

        delete f;

    } else if (!idx.isValid() && (parentId() == targetFolderId)) {

        beginInsertRows(QModelIndex(), rowCount(), rowCount());

        d->feeds.append(f);

        endInsertRows();

    }
}



void AbstractFeedModel::folderMarkedRead(qint64 folderId, qint64 newestItemId)
{
    Q_UNUSED(newestItemId)

    if (rowCount() <= 0) {
        return;
    }

    if (folderId == parentId()) {

        const QList<Feed*> fs = feeds();

        for (Feed *f : fs) {
            f->setUnreadCount(0);
        }

        Q_EMIT dataChanged(index(0, 0), index(rowCount() - 1, 0), QVector<int>(1, Qt::DisplayRole));

    } else if (parentId() < 0) {

        Q_D(AbstractFeedModel);

        for (int i = 0; i < d->feeds.count(); ++i) {

            Feed *f = d->feeds.at(i);

            if (f->folderId() == folderId) {

                QModelIndex idx = index(i, 0);

                f->setUnreadCount(0);

                Q_EMIT dataChanged(idx, idx, QVector<int>(1, Qt::DisplayRole));
            }

        }
    }
}



void AbstractFeedModel::folderDeleted(qint64 folderId)
{
    if (rowCount() <= 0) {
        return;
    }

    Q_D(AbstractFeedModel);

    if (folderId == parentId()) {

        beginRemoveRows(QModelIndex(), 0, rowCount() - 1);

        qDeleteAll(d->feeds);
        d->feeds.clear();

        endRemoveRows();

    } else if (parentId() < 0) {


        IdList rmFeedIds;

        const QList<Feed*> fs = feeds();
        for (Feed *f : fs) {

            if (f->folderId() == folderId) {
                rmFeedIds.append(f->id());
            }
        }

        if (!rmFeedIds.isEmpty()) {

            for (int i = 0; i < rmFeedIds.count(); ++i) {

                int row = d->rowByID(rmFeedIds.at(i));

                if (row > -1) {

                    beginRemoveRows(QModelIndex(), row, row);

                    delete d->feeds.takeAt(row);

                    endRemoveRows();

                }

            }

        }

    }
}


void AbstractFeedModel::clear()
{
    Q_D(AbstractFeedModel);

    if (!d->feeds.isEmpty()) {

        beginRemoveRows(QModelIndex(), 0, rowCount() - 1);

        qDeleteAll(d->feeds);
        d->feeds.clear();

        endRemoveRows();
    }
}



void AbstractFeedModel::itemsRquested(const IdList &updatedItems, const IdList &newItems, const IdList &deletedItems)
{
    Q_UNUSED(updatedItems)
    Q_UNUSED(newItems)
    Q_UNUSED(deletedItems)

    if (!storage()) {
        qWarning("Can not update feeds, no storage available.");
        return;
    }

    QueryArgs qa;
    qa.parentId = parentId();

    const QList<Feed*> fs = storage()->getFeeds(qa);

    if (!fs.isEmpty()) {

        Q_D(AbstractFeedModel);

        for (Feed *f : fs) {

            QModelIndex idx = findByID(f->id());

            if (idx.isValid()) {
                d->feeds.at(idx.row())->copy(f);
                Q_EMIT dataChanged(idx, idx, QVector<int>(1, Qt::DisplayRole));
            }
        }

        qDeleteAll(fs);
    }
}


void AbstractFeedModel::itemMarked(qint64 itemId, bool unread)
{
    if (rowCount() <= 0) {
        return;
    }

    if (!storage()) {
        qWarning("Can not update feeds, no storage available.");
        return;
    }

    Article *a = storage()->getArticle(itemId, -1);

    if (a) {

        QModelIndex idx = findByID(a->feedId());

        if (idx.isValid()) {

            Q_D(AbstractFeedModel);

            Feed *f = d->feeds.at(idx.row());

            if (unread) {
                f->setUnreadCount(f->unreadCount()+1);
            } else {
                f->setUnreadCount(f->unreadCount()-1);
            }

            Q_EMIT dataChanged(idx, idx, QVector<int>(1, Qt::DisplayRole));
        }

        delete a;
    }
}



void AbstractFeedModel::itemsMarked()
{
    if (rowCount() <= 0) {
        return;
    }

    if (!storage()) {
        qWarning("Can not update feeds, no storage available.");
        return;
    }

    QueryArgs qa;
    qa.parentId = parentId();

    const QList<Feed*> fs = storage()->getFeeds(qa);

    if (!fs.isEmpty()) {
        Q_D(AbstractFeedModel);

        for (Feed *f : fs) {
            int row = d->rowByID(f->id());
            if (row > -1) {
                d->feeds.at(row)->setUnreadCount(f->unreadCount());
            }
        }

        Q_EMIT dataChanged(index(0, 0), index(rowCount()-1, 0), QVector<int>(1, Qt::DisplayRole));

        qDeleteAll(fs);
    }
}
