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

#include "folder_p.h"
#include <QJsonDocument>
#include <QJsonObject>
#include "API/deletefolder.h"
#include "API/renamefolder.h"
#include "error.h"
#ifdef QT_DEBUG
#include <QtDebug>
#endif

using namespace Fuoten;


Folder::Folder(QObject *parent) :
    BaseItem(* new FolderPrivate, parent)
{
}



Folder::Folder(qint64 id, const QString &name, uint feedCount, uint unreadCount, QObject *parent) :
    BaseItem(* new FolderPrivate(id, name, feedCount, unreadCount), parent)
{

}


Folder::Folder(FolderPrivate &dd, QObject *parent) :
    BaseItem(dd, parent)
{

}


QString Folder::name() const { Q_D(const Folder); return d->name; }

void Folder::setName(const QString &nName)
{
    Q_D(Folder);
    if (nName != d->name) {
        d->name = nName;
#ifdef QT_DEBUG
        qDebug() << "Changed name to" << d->name;
#endif
        Q_EMIT nameChanged(name());
    }
}




uint Folder::feedCount() const { Q_D(const Folder); return d->feedCount; }

void Folder::setFeedCount(uint nFeedCount)
{
    Q_D(Folder);
    if (nFeedCount != d->feedCount) {
        d->feedCount = nFeedCount;
#ifdef QT_DEBUG
        qDebug() << "Changed feedCount to" << d->feedCount;
#endif
        Q_EMIT feedCountChanged(feedCount());
    }
}




uint Folder::unreadCount() const { Q_D(const Folder); return d->unreadCount; }

void Folder::setUnreadCount(uint nUnreadCount)
{
    Q_D(Folder);
    if (nUnreadCount != d->unreadCount) {
        d->unreadCount = nUnreadCount;
#ifdef QT_DEBUG
        qDebug() << "Changed unreadCount to" << d->unreadCount;
#endif
        Q_EMIT unreadCountChanged(unreadCount());
    }
}




void Folder::rename(const QString &newName, AbstractConfiguration *config, AbstractStorage *storage)
{
    if (inOperation()) {
        qWarning("Folder is still in operation.");
        return;
    }

    if (id() == 0) {
        qWarning("Can not change the folder name. No ID available.");
        return;
    }

    if (!config) {
        qWarning("Can not change the folder name. No configuration available.");
        return;
    }

    RenameFolder *rf = new RenameFolder(this);
    rf->setAbstractConfiguration(config);
    rf->setStorage(storage);
    rf->setFolderId(id());
    rf->setNewName(newName);
    if (!storage) {
        connect(rf, &RenameFolder::succeeded, [=] (qint64 id, const QString &newName) {
            Q_UNUSED(id)
            setName(newName);
            setComponent(nullptr);
        });
    } else {
        connect(rf, &RenameFolder::succeeded, [=] () {setComponent(nullptr);});
    }
    connect(rf, &RenameFolder::succeeded, rf, &QObject::deleteLater);
    setComponent(rf);
    component()->execute();
    Q_EMIT inOperationChanged(inOperation());
}



void Folder::remove(AbstractConfiguration *config, AbstractStorage *storage)
{
    if (inOperation()) {
        qWarning("Folder is still in operation.");
        return;
    }

    if (!config) {
        qWarning("Can not delete the folder. No AbstractConfiguration available.");
        return;
    }

    DeleteFolder *df = new DeleteFolder(this);
    df->setAbstractConfiguration(config);
    df->setStorage(storage);
    df->setFolderId(id());
    connect(df, &DeleteFolder::succeeded, this, &QObject::deleteLater);
    setComponent(df);
    component()->execute();
    Q_EMIT inOperationChanged(inOperation());
}
