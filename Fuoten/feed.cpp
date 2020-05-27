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

#include "feed_p.h"
#include "API/component.h"
#include "API/renamefeed.h"
#include "API/deletefeed.h"
#include "API/movefeed.h"
#include "API/markfeedread.h"
#include "error.h"
#include <QMetaEnum>

using namespace Fuoten;


FeedPrivate::FeedPrivate() :
    BaseItemPrivate()
{

}


FeedPrivate::FeedPrivate(qint64 _id, qint64 _folderId, const QString &_title, const QUrl &_url, const QUrl &_link, const QDateTime &_added, uint _unreadCount, Feed::FeedOrdering _ordering, bool _pinned, uint _updateErrorCount, const QString &_lastUpdateError, const QUrl &_faviconLink, const QString &_folderName) :
    BaseItemPrivate(_id),
    folderId(_folderId),
    title(_title),
    folderName(_folderName),
    lastUpdateError(_lastUpdateError),
    url(_url),
    link(_link),
    faviconLink(_faviconLink),
    added(_added),
    unreadCount(_unreadCount),
    updateErrorCount(_updateErrorCount),
    ordering(_ordering),
    pinned(_pinned)
{

}


FeedPrivate::~FeedPrivate()
{

}


Feed::Feed(QObject *parent) :
    BaseItem(* new FeedPrivate, parent)
{
    qDebug("Creatig new empty Feed object at %p.", this);
}


Feed::Feed(qint64 id, qint64 folderId, const QString &title, const QUrl &url, const QUrl &link, const QDateTime &added, uint unreadCount, Feed::FeedOrdering ordering, bool pinned, uint updateErrorCount, const QString &lastUpdateError, const QUrl &faviconLink, const QString &folderName, QObject *parent) :
    BaseItem(* new FeedPrivate(id, folderId, title, url, link, added, unreadCount, ordering, pinned, updateErrorCount, lastUpdateError, faviconLink, folderName), parent)
{
    qDebug("Creating new Feed object (ID: %lli, Title: %s) at %p.", id, qUtf8Printable(title), this);
}


Feed::Feed(FeedPrivate &dd, QObject *parent) :
    BaseItem(dd, parent)
{
}


Feed::~Feed()
{

}


QUrl Feed::url() const { Q_D(const Feed); return d->url; }

void Feed::setUrl(const QUrl &nUrl)
{
    Q_D(Feed); 
    if (nUrl != d->url) {
        d->url = nUrl;
        qDebug("Changed URL to %s.", qUtf8Printable(d->url.toString()));
        Q_EMIT urlChanged(url());
    }
}




QString Feed::title() const { Q_D(const Feed); return d->title; }

void Feed::setTitle(const QString &nTitle)
{
    Q_D(Feed); 
    if (nTitle != d->title) {
        d->title = nTitle;
        qDebug("Changed title to %s.", qUtf8Printable(d->title));
        Q_EMIT titleChanged(title());
    }
}




QDateTime Feed::added() const { Q_D(const Feed); return d->added; }

void Feed::setAdded(const QDateTime &nAdded)
{
    Q_D(Feed); 
    if (nAdded != d->added) {
        d->added = nAdded;
        qDebug("Changed added to %s.", qUtf8Printable(d->added.toString(Qt::ISODate)));
        Q_EMIT addedChanged(added());
    }
}




qint64 Feed::folderId() const { Q_D(const Feed); return d->folderId; }

void Feed::setFolderId(qint64 nFolderId)
{
    Q_D(Feed); 
    if (nFolderId != d->folderId) {
        d->folderId = nFolderId;
        qDebug("Changed folderId to %lli.", d->folderId);
        Q_EMIT folderIdChanged(folderId());
    }
}




QString Feed::folderName() const { Q_D(const Feed); return d->folderName; }

void Feed::setFolderName(const QString &nFolderName)
{
    Q_D(Feed); 
    if (nFolderName != d->folderName) {
        d->folderName = nFolderName;
        qDebug("Changed folderName to %s.", qUtf8Printable(d->folderName));
        Q_EMIT folderNameChanged(folderName());
    }
}




uint Feed::unreadCount() const { Q_D(const Feed); return d->unreadCount; }

void Feed::setUnreadCount(uint nUnreadCount)
{
    Q_D(Feed); 
    if (nUnreadCount != d->unreadCount) {
        d->unreadCount = nUnreadCount;
        qDebug("Changed unreadCount to %u.", d->unreadCount);
        Q_EMIT unreadCountChanged(unreadCount());
    }
}




Feed::FeedOrdering Feed::ordering() const { Q_D(const Feed); return d->ordering; }

void Feed::setOrdering(FeedOrdering nOrdering)
{
    Q_D(Feed); 
    if (nOrdering != d->ordering) {
        d->ordering = nOrdering;
        qDebug("Changed ordering to %s.", metaObject()->enumerator(metaObject()->indexOfEnumerator("FeedOrdering")).valueToKey(d->ordering));
        Q_EMIT orderingChanged(ordering());
    }
}




QUrl Feed::link() const { Q_D(const Feed); return d->link; }

void Feed::setLink(const QUrl &nLink)
{
    Q_D(Feed); 
    if (nLink != d->link) {
        d->link = nLink;
        qDebug("Changed link to %s.", qUtf8Printable(d->link.toString()));
        Q_EMIT linkChanged(link());
    }
}




bool Feed::pinned() const { Q_D(const Feed); return d->pinned; }

void Feed::setPinned(bool nPinned)
{
    Q_D(Feed); 
    if (nPinned != d->pinned) {
        d->pinned = nPinned;
        qDebug("Changed pinned to %s.", d->pinned ? "true" : "false");
        Q_EMIT pinnedChanged(pinned());
    }
}




uint Feed::updateErrorCount() const { Q_D(const Feed); return d->updateErrorCount; }

void Feed::setUpdateErrorCount(uint nUpdateErrorCount)
{
    Q_D(Feed); 
    if (nUpdateErrorCount != d->updateErrorCount) {
        d->updateErrorCount = nUpdateErrorCount;
        qDebug("Changed updateErrorCount to %u.", d->updateErrorCount);
        Q_EMIT updateErrorCountChanged(updateErrorCount());
    }
}




QString Feed::lastUpdateError() const { Q_D(const Feed); return d->lastUpdateError; }

void Feed::setLastUpdateError(const QString &nLastUpdateError)
{
    Q_D(Feed); 
    if (nLastUpdateError != d->lastUpdateError) {
        d->lastUpdateError = nLastUpdateError;
        qDebug("Changed lastUpdateError to %s.", qUtf8Printable(d->lastUpdateError));
        Q_EMIT lastUpdateErrorChanged(lastUpdateError());
    }
}




QUrl Feed::faviconLink() const { Q_D(const Feed); return d->faviconLink; }

void Feed::setFaviconLink(const QUrl &nFaviconLink)
{
    Q_D(Feed); 
    if (nFaviconLink != d->faviconLink) {
        d->faviconLink = nFaviconLink;
        qDebug("Changed faviconLink to %s.", qUtf8Printable(d->faviconLink.toString()));
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
    Q_ASSERT_X(config, "rename feed", "invalid configuration");

    if (Q_UNLIKELY(inOperation())) {
        qWarning("Folder is still in operation.");
        return;
    }

    RenameFeed *rf = new RenameFeed(this);
    rf->setConfiguration(config);
    rf->setStorage(storage);
    rf->setFeedId(id());
    rf->setNewName(newName);
    if (!storage) {
        connect(rf, &RenameFeed::succeeded, this, [=] (qint64 id, const QString &newName) {
            Q_UNUSED(id)
            setTitle(newName);
            setComponent(nullptr);
        });
    } else {
        connect(rf, &RenameFeed::succeeded, this, [=] () {setComponent(nullptr);});
    }
    connect(rf, &RenameFeed::succeeded, rf, &QObject::deleteLater);
    setComponent(rf);
    component()->execute();
    Q_EMIT inOperationChanged(inOperation());
}


void Feed::rename(const QString &newName)
{
    rename(newName, Component::defaultConfiguration(), Component::defaultStorage());
}


void Feed::remove(AbstractConfiguration *config, AbstractStorage *storage)
{
    Q_ASSERT_X(config, "remove feed", "invalid configuration");

    if (inOperation()) {
        qWarning("Feed is still in operation.");
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


void Feed::remove()
{
    remove(Component::defaultConfiguration(), Component::defaultStorage());
}


void Feed::move(qint64 targetFolderId, AbstractConfiguration *config, AbstractStorage *storage)
{
    Q_ASSERT_X(config, "move feed", "invalid configuration");

    if (inOperation()) {
        qWarning("Feed is still in operation.");
        return;
    }

    MoveFeed *mf = new MoveFeed(this);
    mf->setConfiguration(config);
    mf->setStorage(storage);
    mf->setFeedId(id());
    mf->setFolderId(targetFolderId);
    if (!storage) {
        connect(mf, &MoveFeed::succeeded, this, [=] (qint64 id, qint64 targetFolder) {
            Q_UNUSED(id)
            setFolderId(targetFolder);
            setComponent(nullptr);
        });
    } else {
        connect(mf, &MoveFeed::succeeded, this, [=] () {setComponent(nullptr);});
    }
    connect(mf, &MoveFeed::succeeded, mf, &QObject::deleteLater);
    setComponent(mf);
    component()->execute();
    Q_EMIT inOperationChanged(inOperation());
}


void Feed::move(qint64 targetFolderId)
{
    move(targetFolderId, Component::defaultConfiguration(), Component::defaultStorage());
}


void Feed::markAsRead(AbstractConfiguration *config, AbstractStorage *storage, bool enqueue)
{
    Q_ASSERT_X(config, "enqueue mark feed as read", "missing configuration object");

    if (Q_UNLIKELY(inOperation())) {
        qWarning("Folder is still in operation.");
        return;
    }

    const qint64 newestItemId = storage->getNewestItemId(FuotenEnums::Feed, id());

    if (enqueue) {

        Q_ASSERT_X(storage, "enqueue mark feed as read", "no storage object available");
        if (!storage->enqueueMarkFeedRead(id(), newestItemId)) {
            setError(storage->error());
        }

    } else {

        MarkFeedRead *mfr = new MarkFeedRead(this);
        mfr->setConfiguration(config);
        mfr->setStorage(storage);
        mfr->setFeedId(id());
        mfr->setNewestItemId(newestItemId);
        connect(mfr, &MarkFeedRead::succeeded, this, [=] () {
            setUnreadCount(0);
            setComponent(nullptr);
        });
        connect(mfr, &MarkFeedRead::succeeded, mfr, &QObject::deleteLater);
        setComponent(mfr);
        component()->execute();
        Q_EMIT inOperationChanged(inOperation());

    }
}


void Feed::markAsRead(bool enqueue)
{
    markAsRead(Component::defaultConfiguration(), Component::defaultStorage(), enqueue);
}

#include "moc_feed.cpp"
