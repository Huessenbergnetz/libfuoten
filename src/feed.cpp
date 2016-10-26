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
#ifdef QT_DEBUG
#include <QtDebug>
#endif

using namespace Fuoten;


Feed::Feed(QObject *parent) :
    BaseItem(* new FeedPrivate, parent)
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

void Feed::setOrdering(const FeedOrdering &nOrdering)
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




