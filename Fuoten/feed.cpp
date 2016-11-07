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

#include "feed_p.h"
#include "API/renamefeed.h"
#include "API/deletefeed.h"
#include "API/movefeed.h"
#include "API/markfeedread.h"
#ifdef QT_DEBUG
#include <QtDebug>
#endif

using namespace Fuoten;


Feed::Feed(QObject *parent) :
    BaseItem(* new FeedPrivate, parent)
{
}


Feed::Feed(qint64 id, qint64 folderId, const QString &title, const QUrl &url, const QUrl &link, const QDateTime &added, uint unreadCount, Feed::FeedOrdering ordering, bool pinned, uint updateErrorCount, const QString &lastUpdateError, const QUrl &faviconLink, const QString &folderName, QObject *parent) :
    BaseItem(* new FeedPrivate(id, folderId, title, url, link, added, unreadCount, ordering, pinned, updateErrorCount, lastUpdateError, faviconLink, folderName), parent)
{

}


Feed::Feed(FeedPrivate &dd, QObject *parent) :
    BaseItem(dd, parent)
{
}



QUrl Feed::url() const { Q_D(const Feed); return d->url; }

void Feed::setUrl(const QUrl &nUrl)
{
    Q_D(Feed); 
    if (nUrl != d->url) {
        d->url = nUrl;
#ifdef QT_DEBUG
        qDebug() << "Changed url to" << d->url;
#endif
        Q_EMIT urlChanged(url());
    }
}




QString Feed::title() const { Q_D(const Feed); return d->title; }

void Feed::setTitle(const QString &nTitle)
{
    Q_D(Feed); 
    if (nTitle != d->title) {
        d->title = nTitle;
#ifdef QT_DEBUG
        qDebug() << "Changed title to" << d->title;
#endif
        Q_EMIT titleChanged(title());
    }
}




QDateTime Feed::added() const { Q_D(const Feed); return d->added; }

void Feed::setAdded(const QDateTime &nAdded)
{
    Q_D(Feed); 
    if (nAdded != d->added) {
        d->added = nAdded;
#ifdef QT_DEBUG
        qDebug() << "Changed added to" << d->added;
#endif
        Q_EMIT addedChanged(added());
    }
}




qint64 Feed::folderId() const { Q_D(const Feed); return d->folderId; }

void Feed::setFolderId(qint64 nFolderId)
{
    Q_D(Feed); 
    if (nFolderId != d->folderId) {
        d->folderId = nFolderId;
#ifdef QT_DEBUG
        qDebug() << "Changed folderId to" << d->folderId;
#endif
        Q_EMIT folderIdChanged(folderId());
    }
}




QString Feed::folderName() const { Q_D(const Feed); return d->folderName; }

void Feed::setFolderName(const QString &nFolderName)
{
    Q_D(Feed); 
    if (nFolderName != d->folderName) {
        d->folderName = nFolderName;
#ifdef QT_DEBUG
        qDebug() << "Changed folderName to" << d->folderName;
#endif
        Q_EMIT folderNameChanged(folderName());
    }
}




uint Feed::unreadCount() const { Q_D(const Feed); return d->unreadCount; }

void Feed::setUnreadCount(uint nUnreadCount)
{
    Q_D(Feed); 
    if (nUnreadCount != d->unreadCount) {
        d->unreadCount = nUnreadCount;
#ifdef QT_DEBUG
        qDebug() << "Changed unreadCount to" << d->unreadCount;
#endif
        Q_EMIT unreadCountChanged(unreadCount());
    }
}




Feed::FeedOrdering Feed::ordering() const { Q_D(const Feed); return d->ordering; }

void Feed::setOrdering(FeedOrdering nOrdering)
{
    Q_D(Feed); 
    if (nOrdering != d->ordering) {
        d->ordering = nOrdering;
#ifdef QT_DEBUG
        qDebug() << "Changed ordering to" << d->ordering;
#endif
        Q_EMIT orderingChanged(ordering());
    }
}




QUrl Feed::link() const { Q_D(const Feed); return d->link; }

void Feed::setLink(const QUrl &nLink)
{
    Q_D(Feed); 
    if (nLink != d->link) {
        d->link = nLink;
#ifdef QT_DEBUG
        qDebug() << "Changed link to" << d->link;
#endif
        Q_EMIT linkChanged(link());
    }
}




bool Feed::pinned() const { Q_D(const Feed); return d->pinned; }

void Feed::setPinned(bool nPinned)
{
    Q_D(Feed); 
    if (nPinned != d->pinned) {
        d->pinned = nPinned;
#ifdef QT_DEBUG
        qDebug() << "Changed pinned to" << d->pinned;
#endif
        Q_EMIT pinnedChanged(pinned());
    }
}




uint Feed::updateErrorCount() const { Q_D(const Feed); return d->updateErrorCount; }

void Feed::setUpdateErrorCount(uint nUpdateErrorCount)
{
    Q_D(Feed); 
    if (nUpdateErrorCount != d->updateErrorCount) {
        d->updateErrorCount = nUpdateErrorCount;
#ifdef QT_DEBUG
        qDebug() << "Changed updateErrorCount to" << d->updateErrorCount;
#endif
        Q_EMIT updateErrorCountChanged(updateErrorCount());
    }
}




QString Feed::lastUpdateError() const { Q_D(const Feed); return d->lastUpdateError; }

void Feed::setLastUpdateError(const QString &nLastUpdateError)
{
    Q_D(Feed); 
    if (nLastUpdateError != d->lastUpdateError) {
        d->lastUpdateError = nLastUpdateError;
#ifdef QT_DEBUG
        qDebug() << "Changed lastUpdateErro to" << d->lastUpdateError;
#endif
        Q_EMIT lastUpdateErrorChanged(lastUpdateError());
    }
}




QUrl Feed::faviconLink() const { Q_D(const Feed); return d->faviconLink; }

void Feed::setFaviconLink(const QUrl &nFaviconLink)
{
    Q_D(Feed); 
    if (nFaviconLink != d->faviconLink) {
        d->faviconLink = nFaviconLink;
#ifdef QT_DEBUG
        qDebug() << "Changed faviconLink to" << d->faviconLink;
#endif
        Q_EMIT faviconLinkChanged(faviconLink());
    }
}




void Feed::copy(Fuoten::BaseItem *other)
{
    Feed *o = qobject_cast<Feed*>(other);

    if (o && (o->id() == id())) {
        setAdded(o->added());
        setFaviconLink(o->faviconLink());
        setFolderId(o->folderId());
        setFolderName(o->folderName());
        setLastUpdateError(o->lastUpdateError());
        setLink(o->link());
        setOrdering(o->ordering());
        setPinned(o->pinned());
        setTitle(o->title());
        setUnreadCount(o->unreadCount());
        setUpdateErrorCount(o->updateErrorCount());
        setUrl(o->url());
    } else {
        qCritical("Failed to cast BaseItem to Feed when trying to create a deep copy!");
    }
}



void Feed::rename(const QString &newName, AbstractConfiguration *config, AbstractStorage *storage)
{
    if (inOperation()) {
        qWarning("Folder is still in operation.");
        return;
    }

    if (!config) {
        qWarning("Can not change the folder name. No configuration available.");
        return;
    }

    RenameFeed *rf = new RenameFeed(this);
    rf->setConfiguration(config);
    rf->setStorage(storage);
    rf->setFeedId(id());
    rf->setNewName(newName);
    if (!storage) {
        connect(rf, &RenameFeed::succeeded, [=] (qint64 id, const QString &newName) {
            Q_UNUSED(id)
            setTitle(newName);
            setComponent(nullptr);
        });
    } else {
        connect(rf, &RenameFeed::succeeded, [=] () {setComponent(nullptr);});
    }
    connect(rf, &RenameFeed::succeeded, rf, &QObject::deleteLater);
    setComponent(rf);
    component()->execute();
    Q_EMIT inOperationChanged(inOperation());
}




void Feed::remove(AbstractConfiguration *config, AbstractStorage *storage)
{
    if (inOperation()) {
        qWarning("Feed is still in operation.");
        return;
    }

    if (!config) {
        qWarning("Can not delete the folder. No AbstractConfiguration available.");
        return;
    }

    DeleteFeed *df = new DeleteFeed(this);
    df->setConfiguration(config);
    df->setStorage(storage);
    df->setFeedId(id());
    connect(df, &DeleteFeed::succeeded, this, &QObject::deleteLater);
    setComponent(df);
    component()->execute();
    Q_EMIT inOperationChanged(inOperation());
}



void Feed::move(qint64 targetFolderId, AbstractConfiguration *config, AbstractStorage *storage)
{
    if (inOperation()) {
        qWarning("Feed is still in operation.");
        return;
    }

    if (!config) {
        qWarning("Can not delete the folder. No AbstractConfiguration available.");
        return;
    }

    MoveFeed *mf = new MoveFeed(this);
    mf->setConfiguration(config);
    mf->setStorage(storage);
    mf->setFeedId(id());
    mf->setFolderId(targetFolderId);
    if (!storage) {
        connect(mf, &MoveFeed::succeeded, [=] (qint64 id, qint64 targetFolder) {
            Q_UNUSED(id)
            setFolderId(targetFolder);
            setComponent(nullptr);
        });
    } else {
        connect(mf, &MoveFeed::succeeded, [=] () {setComponent(nullptr);});
    }
    connect(mf, &MoveFeed::succeeded, mf, &QObject::deleteLater);
    setComponent(mf);
    component()->execute();
    Q_EMIT inOperationChanged(inOperation());
}



void Feed::markAsRead(AbstractConfiguration *config, AbstractStorage *storage)
{
    if (inOperation()) {
        qWarning("Folder is still in operation.");
        return;
    }

    if (!config || !storage) {
        qWarning("Can not delete the folder. No configuration and no storage available.");
        return;
    }

    const qint64 newestItemId = storage->getNewestItemId(FuotenEnums::Feed, id());

    MarkFeedRead *mfr = new MarkFeedRead(this);
    mfr->setConfiguration(config);
    mfr->setStorage(storage);
    mfr->setFeedId(id());
    mfr->setNewestItemId(newestItemId);
    connect(mfr, &MarkFeedRead::succeeded, [=] () {
        setUnreadCount(0);
        setComponent(nullptr);
    });
    connect(mfr, &MarkFeedRead::succeeded, mfr, &QObject::deleteLater);
    setComponent(mfr);
    component()->execute();
    Q_EMIT inOperationChanged(inOperation());
}
