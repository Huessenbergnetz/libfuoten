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

#include "abstractfoldermodel_p.h"
#include "../Storage/abstractstorage.h"
#include "../fuoten.h"

#ifdef QT_DEBUG
#include <QtDebug>
#endif

using namespace Fuoten;

AbstractFolderModel::AbstractFolderModel(QObject *parent) :
    BaseModel(* new AbstractFolderModelPrivate, parent)
{

}



AbstractFolderModel::AbstractFolderModel(AbstractFolderModelPrivate &dd, QObject *parent) :
    BaseModel(dd, parent)
{

}



void AbstractFolderModel::handleStorageChanged()
{
    AbstractStorage *s = storage();
    connect(s, &AbstractStorage::readyChanged, this, &AbstractFolderModel::load);
    connect(s, &AbstractStorage::requestedFolders, this, &AbstractFolderModel::foldersRequested);
    connect(s, &AbstractStorage::renamedFolder, this, &AbstractFolderModel::folderRenamed);
    connect(s, &AbstractStorage::createdFolder, this, &AbstractFolderModel::folderCreated);
    connect(s, &AbstractStorage::deletedFolder, this, &AbstractFolderModel::folderDeleted);
    connect(s, &AbstractStorage::markedReadFolder, this, &AbstractFolderModel::folderMarkedRead);

    connect(s, &AbstractStorage::requestedFeeds, this, &AbstractFolderModel::feedsRequested);
    connect(s, &AbstractStorage::createdFeed, this, &AbstractFolderModel::feedCreated);
    connect(s, &AbstractStorage::deletedFeed, this, &AbstractFolderModel::updateCountValues);
    connect(s, &AbstractStorage::movedFeed, this, &AbstractFolderModel::updateCountValues);
    connect(s, &AbstractStorage::markedReadFeed, this, &AbstractFolderModel::feedMarkedRead);

    connect(s, &AbstractStorage::requestedItems, this, &AbstractFolderModel::updateCountValues);
    connect(s, &AbstractStorage::markedItems, this, &AbstractFolderModel::updateCountValues);
    connect(s, &AbstractStorage::starredItems, this, &AbstractFolderModel::updateCountValues);
}


void AbstractFolderModel::load()
{
    if (!storage()) {
        return;
    }

    if (!storage()->ready() || loaded()) {
        return;
    }

    setInOperation(true);

    Q_D(AbstractFolderModel);

    const QList<Folder*> fs = storage()->getFolders(FuotenEnums::Name, Qt::AscendingOrder);
    if (!fs.isEmpty()) {

        beginInsertRows(QModelIndex(), 0, fs.count() - 1);

        d->folders = fs;

        endInsertRows();
    }

    setLoaded(true);

    setInOperation(false);
}


QList<Folder*> AbstractFolderModel::folders() const
{
    Q_D(const AbstractFolderModel);
    return d->folders;
}



QModelIndex AbstractFolderModel::findByID(qint64 id) const
{
    Q_D(const AbstractFolderModel);
    if (d->folders.isEmpty()) {
        return QModelIndex();
    }

    int idx = -1;

    for (int i = 0; i < d->folders.count(); ++i) {
        if (d->folders.at(i)->id() == id) {
            idx = i;
            break;
        }
    }

    return (idx > -1) ? index(idx, 0) : QModelIndex();
}




void AbstractFolderModel::folderRenamed(qint64 id, const QString &newName)
{
    if (id == 0 || newName.isEmpty()) {
        qWarning("Can not rename folder. ID is invalid or the new name is empty.");
        return;
    }

    Q_D(AbstractFolderModel);

    QModelIndex i = findByID(id);

    if (i.isValid()) {
        d->folders.at(i.row())->setName(newName);
    }

    Q_EMIT dataChanged(i, i, QVector<int>(1, Qt::DisplayRole));
}




void AbstractFolderModel::folderCreated(qint64 id, const QString &name)
{
    if (id == 0 || name.isEmpty()) {
        qWarning("Can not create folder. ID is invalid or the new name is empty.");
        return;
    }

    Q_D(AbstractFolderModel);

    beginInsertRows(QModelIndex(), rowCount(), rowCount());

    d->folders.append(new Folder(id, name, 0, 0, 0));

    endInsertRows();
}



void AbstractFolderModel::foldersRequested(const QList<QPair<qint64, QString> > &updatedFolders, const QList<QPair<qint64, QString> > &newFolders, const IdList &deletedFolders)
{
    if (!storage()) {
        return;
    }

    Q_D(AbstractFolderModel);

    // process updated folders
    if (!updatedFolders.isEmpty()) {

        for (const QPair<qint64, QString> &p : updatedFolders) {
            QModelIndex i = findByID(p.first);
            if (i.isValid()) {
                d->folders.at(i.row())->setName(p.second);
                Q_EMIT dataChanged(i, i, QVector<int>(1, Qt::DisplayRole));
            }
        }
    }

    if (!newFolders.isEmpty()) {
        IdList nf;
        nf.reserve(newFolders.size());
        for (const QPair<qint64, QString> &p : newFolders) {
            nf.append(p.first);
        }

        if (!nf.isEmpty()) {
            const QList<Folder*> fs = storage()->getFolders(FuotenEnums::Name, Qt::AscendingOrder, nf);

            if (!fs.isEmpty()) {

                beginInsertRows(QModelIndex(), rowCount(), rowCount() + fs.count() - 1);

                d->folders.append(fs);

                endInsertRows();
            }
        }
    }

    if (!deletedFolders.isEmpty()) {

        for (qint64 id : deletedFolders) {

            int idx = d->rowByID(id);

            if (idx > -1) {

                beginRemoveRows(QModelIndex(), idx, idx);

                delete d->folders.takeAt(idx);

                endRemoveRows();

            }

        }
    }
}



void AbstractFolderModel::folderDeleted(qint64 id)
{
    if (id <= 0) {
        qWarning("Can not delete folder. Invalid folder ID.");
        return;
    }

    Q_D(AbstractFolderModel);

    int idx = d->rowByID(id);

    if (idx < 0) {
        qWarning("Can not find folder ID in the model. Can not remove folder from model.");
        return;
    }

    beginRemoveRows(QModelIndex(), idx, idx);

    Folder *f = d->folders.takeAt(idx);

    endRemoveRows();

    f->deleteLater();
}



void AbstractFolderModel::folderMarkedRead(qint64 id, qint64 newestItem)
{
    Q_UNUSED(newestItem)

    if (id <= 0) {
        qWarning("Can not mark folder as read. Invalid folder ID.");
        return;
    }

    QModelIndex i = findByID(id);
    if (i.isValid()) {
        Q_D(AbstractFolderModel);
        d->folders.at(i.row())->setUnreadCount(0);
        Q_EMIT dataChanged(i, i, QVector<int>(1, Qt::DisplayRole));
    }
}



void AbstractFolderModel::feedsRequested(const IdList &updatedFeeds, const IdList &newFeeds, const IdList &deletedFeeds)
{
    if (!storage()) {
        qWarning("Can not update folders, no storage available.");
        return;
    }

#ifdef QT_DEBUG
    qDebug() << "Feeds requested. Updating folder model.";
#endif

    if (!updatedFeeds.isEmpty() || !newFeeds.isEmpty() || !deletedFeeds.isEmpty()) {

        const QList<Folder*> fs = storage()->getFolders(FuotenEnums::ID);

        if (!fs.isEmpty()) {
            Q_D(AbstractFolderModel);
            for (const Folder *f : fs) {
                QModelIndex i = findByID(f->id());
                if (i.isValid()) {
                    Folder *mf = d->folders.at(i.row());
                    mf->setFeedCount(f->feedCount());
                    mf->setUnreadCount(f->unreadCount());
                    Q_EMIT dataChanged(i, i, QVector<int>(1, Qt::DisplayRole));
                }
                delete f;
            }
        }
    }
}




void AbstractFolderModel::feedMarkedRead(qint64 id)
{
    if (!storage()) {
        qWarning("Can not update folders, no storage available.");
        return;
    }

    IdList l;
    l.append(id);

    const QList<Folder*> fs = storage()->getFolders(FuotenEnums::ID, Qt::AscendingOrder, l, FuotenEnums::Feed);

    if (!fs.isEmpty()) {
        Folder *f = fs.first();
        QModelIndex i = findByID(f->id());
        if (i.isValid()) {
            Q_D(AbstractFolderModel);
            Folder *mf = d->folders.at(i.row());
            mf->setFeedCount(f->feedCount());
            mf->setUnreadCount(f->unreadCount());
            Q_EMIT dataChanged(i, i, QVector<int>(1, Qt::DisplayRole));
        }
        delete f;
    }
}



void AbstractFolderModel::feedCreated(qint64 feedId, qint64 folderId)
{
    if (!storage()) {
        qWarning("Can not update folders, no storage available.");
        return;
    }

    Q_UNUSED(feedId);

    IdList l;
    l.append(folderId);

    const QList<Folder*> fs = storage()->getFolders(FuotenEnums::Name, Qt::AscendingOrder, l);

    if (!fs.isEmpty()) {
        Folder *f = fs.first();
        QModelIndex i = findByID(f->id());
        if (i.isValid()) {
            Q_D(AbstractFolderModel);
            Folder *mf = d->folders.at(i.row());
            mf->setFeedCount(f->feedCount());
            mf->setUnreadCount(f->unreadCount());
            Q_EMIT dataChanged(i, i, QVector<int>(1, Qt::DisplayRole));
        }
        delete f;
    }
}



void AbstractFolderModel::updateCountValues()
{
    if (!storage()) {
        qWarning("Can not update folders, no storage available.");
        return;
    }

    const QList<Folder*> fs = storage()->getFolders();

    if (!fs.isEmpty()) {
        Q_D(AbstractFolderModel);
        for (const Folder *f : fs) {
            int i = d->rowByID(f->id());
            if (i > -1) {
                Folder *mf = d->folders.at(i);
                mf->setFeedCount(f->feedCount());
                mf->setUnreadCount(f->unreadCount());
            }
            delete f;
        }
        Q_EMIT dataChanged(index(0, 0), index(rowCount()-1 ,0), QVector<int>(1, Qt::DisplayRole));
    }
}


void AbstractFolderModel::clear()
{
    Q_D(AbstractFolderModel);

    if (!d->folders.isEmpty()) {

        beginRemoveRows(QModelIndex(), 0, rowCount() - 1);

        qDeleteAll(d->folders);
        d->folders.clear();

        endRemoveRows();
    }
}
