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
}


void AbstractFolderModel::load()
{
    if (!storage()) {
        return;
    }

    if (!storage()->ready()) {
        return;
    }

    if (loaded()) {
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



void AbstractFolderModel::foldersRequested(const QList<QPair<qint64, QString> > &updatedFolders, const QList<QPair<qint64, QString> > &newFolders, const QList<qint64> &deletedFolders)
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
        QList<qint64> nf;
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
