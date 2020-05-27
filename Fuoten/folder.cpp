/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016-2019 Hüssenbergnetz/Matthias Fehring
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

#include "folder_p.h"
#include <QJsonDocument>
#include <QJsonObject>
#include "API/deletefolder.h"
#include "API/renamefolder.h"
#include "API/markfolderread.h"
#include "error.h"

using namespace Fuoten;


FolderPrivate::FolderPrivate() :
    BaseItemPrivate(),
    feedCount(0),
    unreadCount(0)
{

}


FolderPrivate::FolderPrivate(qint64 _id, const QString &_name, uint _feedCount, uint _unreadCount) :
    BaseItemPrivate(_id),
    name(_name),
    feedCount(_feedCount),
    unreadCount(_unreadCount)
{

}


FolderPrivate::~FolderPrivate()
{

}


Folder::Folder(QObject *parent) :
    BaseItem(* new FolderPrivate, parent)
{
    qDebug("Creating new empty Folder object at %p.", this);
}



Folder::Folder(qint64 id, const QString &name, uint feedCount, uint unreadCount, QObject *parent) :
    BaseItem(* new FolderPrivate(id, name, feedCount, unreadCount), parent)
{
    qDebug("Creating new Folder object (ID: %lli, Name: %s) at %p.", id, qUtf8Printable(name), this);
}


Folder::Folder(FolderPrivate &dd, QObject *parent) :
    BaseItem(dd, parent)
{

}


Folder::~Folder()
{

}


QString Folder::name() const { Q_D(const Folder); return d->name; }

void Folder::setName(const QString &nName)
{
    Q_D(Folder);
    if (nName != d->name) {
        d->name = nName;
        qDebug("Changed name to %s.", qUtf8Printable(d->name));
        Q_EMIT nameChanged(name());
    }
}




uint Folder::feedCount() const { Q_D(const Folder); return d->feedCount; }

void Folder::setFeedCount(uint nFeedCount)
{
    Q_D(Folder);
    if (nFeedCount != d->feedCount) {
        d->feedCount = nFeedCount;
        qDebug("Changed feedCount to %u.", d->feedCount);
        Q_EMIT feedCountChanged(feedCount());
    }
}




uint Folder::unreadCount() const { Q_D(const Folder); return d->unreadCount; }

void Folder::setUnreadCount(uint nUnreadCount)
{
    Q_D(Folder);
    if (nUnreadCount != d->unreadCount) {
        d->unreadCount = nUnreadCount;
        qDebug("Changed unreadCount to %u.", d->unreadCount);
        Q_EMIT unreadCountChanged(unreadCount());
    }
}




void Folder::rename(const QString &newName, AbstractConfiguration *config, AbstractStorage *storage)
{
    Q_ASSERT_X(config, "rename folder", "missing configuratoin");
    Q_ASSERT_X(!newName.isEmpty(), "rename folder", "empty new name");

    if (Q_UNLIKELY(inOperation())) {
        qWarning("Folder is still in operation.");
        return;
    }

    RenameFolder *rf = new RenameFolder(this);
    rf->setConfiguration(config);
    rf->setStorage(storage);
    rf->setFolderId(id());
    rf->setNewName(newName);
    if (storage) {
        connect(rf, &RenameFolder::succeeded, this, [=] () {setComponent(nullptr);});
    } else {
        connect(rf, &RenameFolder::succeeded, this, [=] (qint64 id, const QString &newName) {
            Q_UNUSED(id)
            setName(newName);
            setComponent(nullptr);
        });
    }
    connect(rf, &RenameFolder::succeeded, rf, &QObject::deleteLater);
    setComponent(rf);
    component()->execute();
    Q_EMIT inOperationChanged(inOperation());
}


void Folder::rename(const QString &newName)
{
    rename(newName, Component::defaultConfiguration(), Component::defaultStorage());
}


void Folder::remove(AbstractConfiguration *config, AbstractStorage *storage)
{
    Q_ASSERT_X(config, "remove folder", "missing configuration");

    if (Q_UNLIKELY(inOperation())) {
        qWarning("Folder is still in operation.");
        return;
    }

    DeleteFolder *df = new DeleteFolder(this);
    df->setConfiguration(config);
    df->setStorage(storage);
    df->setFolderId(id());
    connect(df, &DeleteFolder::succeeded, this, &QObject::deleteLater);
    setComponent(df);
    component()->execute();
    Q_EMIT inOperationChanged(inOperation());
}


void Folder::remove()
{
    remove(Component::defaultConfiguration(), Component::defaultStorage());
}


void Folder::markAsRead(AbstractConfiguration *config, AbstractStorage *storage, bool enqueue)
{
    Q_ASSERT_X(config, "mark folder as read", "missing configuration");

    if (Q_UNLIKELY(inOperation())) {
        qWarning("Folder is still in operation.");
        return;
    }

    const qint64 newestItemId = storage->getNewestItemId(FuotenEnums::Folder, id());

    if (enqueue) {

        Q_ASSERT_X(storage, "enqueue mark folder as read", "missing storage");
        if (!storage->enqueueMarkFolderRead(id(), newestItemId)) {
            setError(storage->error());
        }

    } else {

        MarkFolderRead *mfr = new MarkFolderRead(this);
        mfr->setConfiguration(config);
        mfr->setStorage(storage);
        mfr->setFolderId(id());
        mfr->setNewestItemId(newestItemId);
        connect(mfr, &MarkFolderRead::succeeded, this, [=] () {
            setUnreadCount(0);
            setComponent(nullptr);
        });
        connect(mfr, &MarkFolderRead::succeeded, mfr, &QObject::deleteLater);
        setComponent(mfr);
        component()->execute();
        Q_EMIT inOperationChanged(true);
    }
}


void Folder::markAsRead(bool enqueue)
{
    markAsRead(Component::defaultConfiguration(), Component::defaultStorage(), enqueue);
}


void Folder::copy(BaseItem *other)
{
    Folder *o = qobject_cast<Folder*>(other);

    if (o && (o->id() == id())) {
        setName(o->name());
        setFeedCount(o->feedCount());
        setUnreadCount(o->unreadCount());
    } else {
        qCritical("Failed to cast BaseItem to Folder when trying to create a deep copy!");
    }

}

#include "moc_folder.cpp"
