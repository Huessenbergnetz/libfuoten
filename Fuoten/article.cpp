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

#include "article_p.h"
#include "API/markitem.h"
#include "API/staritem.h"
#ifdef QT_DEBUG
#include <QtDebug>
#endif

using namespace Fuoten;

Article::Article(QObject *parent) :
    BaseItem(* new ArticlePrivate, parent)
{
}


Article::Article(qint64 id, qint64 feedId, const QString &feedTitle, const QString &guid, const QString &guidHash, const QUrl &url, const QString &title, const QString &author, const QDateTime &pubDate, const QString &body, const QString &enclosureMime, const QUrl &enclosureLink, bool unread, bool starred, const QDateTime &lastModified, const QString &fingerprint, qint64 folderId, const QString &folderName, QObject *parent) :
    BaseItem(* new ArticlePrivate(id, feedId, feedTitle, guid, guidHash, url, title, author, pubDate, body, enclosureMime, enclosureLink, unread, starred, lastModified, fingerprint, folderId, folderName), parent)
{
}


Article::Article(Article *other, QObject *parent) :
    BaseItem(* new ArticlePrivate(other), parent)
{
}

Article::Article(ArticlePrivate &dd, QObject *parent) :
    BaseItem(dd, parent)
{

}



qint64 Article::feedId() const { Q_D(const Article); return d->feedId; }

void Article::setFeedId(qint64 nFeedId)
{
    Q_D(Article);
    if (nFeedId != d->feedId) {
        d->feedId = nFeedId;
#ifdef QT_DEBUG
        qDebug() << "Changed feedId to" << d->feedId;
#endif
        Q_EMIT feedIdChanged(feedId());
    }
}




QString Article::feedTitle() const { Q_D(const Article); return d->feedTitle; }

void Article::setFeedTitle(const QString &nFeedTitle)
{
    Q_D(Article);
    if (nFeedTitle != d->feedTitle) {
        d->feedTitle = nFeedTitle;
#ifdef QT_DEBUG
        qDebug() << "Changed feedTitle to" << d->feedTitle;
#endif
        Q_EMIT feedTitleChanged(feedTitle());
    }
}




QString Article::guid() const { Q_D(const Article); return d->guid; }

void Article::setGuid(const QString &nGuid)
{
    Q_D(Article);
    if (nGuid != d->guid) {
        d->guid = nGuid;
#ifdef QT_DEBUG
        qDebug() << "Changed guid to" << d->guid;
#endif
        Q_EMIT guidChanged(guid());
    }
}




QString Article::guidHash() const { Q_D(const Article); return d->guidHash; }

void Article::setGuidHash(const QString &nGuidHash)
{
    Q_D(Article);
    if (nGuidHash != d->guidHash) {
        d->guidHash = nGuidHash;
#ifdef QT_DEBUG
        qDebug() << "Changed guidHash to" << d->guidHash;
#endif
        Q_EMIT guidHashChanged(guidHash());
    }
}




QUrl Article::url() const { Q_D(const Article); return d->url; }

void Article::setUrl(const QUrl &nUrl)
{
    Q_D(Article);
    if (nUrl != d->url) {
        d->url = nUrl;
#ifdef QT_DEBUG
        qDebug() << "Changed url to" << d->url;
#endif
        Q_EMIT urlChanged(url());
    }
}




QString Article::title() const { Q_D(const Article); return d->title; }

void Article::setTitle(const QString &nTitle)
{
    Q_D(Article);
    if (nTitle != d->title) {
        d->title = nTitle;
#ifdef QT_DEBUG
        qDebug() << "Changed title to" << d->title;
#endif
        Q_EMIT titleChanged(title());
    }
}




QString Article::author() const { Q_D(const Article); return d->author; }

void Article::setAuthor(const QString &nAuthor)
{
    Q_D(Article);
    if (nAuthor != d->author) {
        d->author = nAuthor;
#ifdef QT_DEBUG
        qDebug() << "Changed author to" << d->author;
#endif
        Q_EMIT authorChanged(author());
    }
}




QDateTime Article::pubDate() const { Q_D(const Article); return d->pubDate; }

void Article::setPubDate(const QDateTime &nPubDate)
{
    Q_D(Article);
    if (nPubDate != d->pubDate) {
        d->pubDate = nPubDate;
#ifdef QT_DEBUG
        qDebug() << "Changed pubDate to" << d->pubDate;
#endif
        Q_EMIT pubDateChanged(pubDate());
        d->createHumanPubDate();
        Q_EMIT humanPubDateChanged(humanPubDate());
    }
}




QString Article::body() const { Q_D(const Article); return d->body; }

void Article::setBody(const QString &nBody)
{
    Q_D(Article);
    if (nBody != d->body) {
        d->body = nBody;
#ifdef QT_DEBUG
        qDebug() << "Changed body to" << d->body;
#endif
        Q_EMIT bodyChanged(body());
    }
}




QString Article::enclosureMime() const { Q_D(const Article); return d->enclosureMime; }

void Article::setEnclosureMime(const QString &nEnclosureMime)
{
    Q_D(Article);
    if (nEnclosureMime != d->enclosureMime) {
        d->enclosureMime = nEnclosureMime;
#ifdef QT_DEBUG
        qDebug() << "Changed enclosureMime to" << d->enclosureMime;
#endif
        Q_EMIT enclosureMimeChanged(enclosureMime());
    }
}




QUrl Article::enclosureLink() const { Q_D(const Article); return d->enclosureLink; }

void Article::setEnclosureLink(const QUrl &nEnclosureLink)
{
    Q_D(Article);
    if (nEnclosureLink != d->enclosureLink) {
        d->enclosureLink = nEnclosureLink;
#ifdef QT_DEBUG
        qDebug() << "Changed enclosureLink to" << d->enclosureLink;
#endif
        Q_EMIT enclosureLinkChanged(enclosureLink());
    }
}




bool Article::unread() const { Q_D(const Article); return d->unread; }

void Article::setUnread(bool nUnread)
{
    Q_D(Article);
    if (nUnread != d->unread) {
        d->unread = nUnread;
#ifdef QT_DEBUG
        qDebug() << "Changed unread to" << d->unread;
#endif
        Q_EMIT unreadChanged(unread());
    }
}




bool Article::starred() const { Q_D(const Article); return d->starred; }

void Article::setStarred(bool nStarred)
{
    Q_D(Article);
    if (nStarred != d->starred) {
        d->starred = nStarred;
#ifdef QT_DEBUG
        qDebug() << "Changed starred to" << d->starred;
#endif
        Q_EMIT starredChanged(starred());
    }
}




QDateTime Article::lastModified() const { Q_D(const Article); return d->lastModified; }

void Article::setLastModified(const QDateTime &nLastModified)
{
    Q_D(Article);
    if (nLastModified != d->lastModified) {
        d->lastModified = nLastModified;
#ifdef QT_DEBUG
        qDebug() << "Changed lastModified to" << d->lastModified;
#endif
        Q_EMIT lastModifiedChanged(lastModified());
    }
}




QString Article::fingerprint() const { Q_D(const Article); return d->fingerprint; }

void Article::setFingerprint(const QString &nFingerprint)
{
    Q_D(Article);
    if (nFingerprint != d->fingerprint) {
        d->fingerprint = nFingerprint;
#ifdef QT_DEBUG
        qDebug() << "Changed fingerprint to" << d->fingerprint;
#endif
        Q_EMIT fingerprintChanged(fingerprint());
    }
}




qint64 Article::folderId() const { Q_D(const Article); return d->folderId; }

void Article::setFolderId(qint64 nFolderId)
{
    Q_D(Article);
    if (nFolderId != d->folderId) {
        d->folderId = nFolderId;
#ifdef QT_DEBUG
        qDebug() << "Changed folderId to" << d->folderId;
#endif
        Q_EMIT folderIdChanged(folderId());
    }
}




QString Article::folderName() const { Q_D(const Article); return d->folderName; }

void Article::setFolderName(const QString &nFolderName)
{
    Q_D(Article);
    if (nFolderName != d->folderName) {
        d->folderName = nFolderName;
#ifdef QT_DEBUG
        qDebug() << "Changed folderName to" << d->folderName;
#endif
        Q_EMIT folderNameChanged(folderName());
    }
}


QString Article::humanPubDate() const { Q_D(const Article); return d->humanPubDate; }



void Article::copy(BaseItem *other)
{
    Article *o = qobject_cast<Article*>(other);

    if (o && (o->id() == id())) {
        setFeedId(o->feedId());
        setFeedTitle(o->feedTitle());
        setGuid(o->guid());
        setGuidHash(o->guidHash());
        setUrl(o->url());
        setTitle(o->title());
        setAuthor(o->author());
        setPubDate(o->pubDate());
        setBody(o->body());
        setEnclosureLink(o->enclosureLink());
        setEnclosureMime(o->enclosureMime());
        setUnread(o->unread());
        setStarred(o->starred());
        setLastModified(o->lastModified());
        setFingerprint(o->fingerprint());
        setFolderId(o->folderId());
        setFolderName(o->folderName());
    } else {
        qCritical("Failed to cast BaseItem to Article when trying to create a deep copy!");
    }
}



void Article::mark(bool unread, AbstractConfiguration *config, AbstractStorage *storage)
{
    if (inOperation()) {
        qWarning("Item is still in operation.");
        return;
    }

    if (!config) {
        qWarning("Can not mark item. No configuration available.");
        return;
    }

    MarkItem *mi = new MarkItem(id(), unread, this);
    mi->setConfiguration(config);
    mi->setStorage(storage);
    connect(mi, &MarkItem::succeeded, [=] () {
        setUnread(unread);
        setComponent(nullptr);
    });
    connect(mi, &MarkItem::succeeded, mi, &QObject::deleteLater);
    setComponent(mi);
    component()->execute();
    Q_EMIT inOperationChanged(inOperation());
}



void Article::star(bool starred, AbstractConfiguration *config, AbstractStorage *storage)
{
    if (inOperation()) {
        qWarning("Item is still in operation.");
        return;
    }

    if (!config) {
        qWarning("Can not mark item. No configuration available.");
        return;
    }

    StarItem *si = new StarItem(feedId(), guidHash(), starred, this);
    si->setConfiguration(config);
    si->setStorage(storage);
    connect(si, &StarItem::succeeded, [=] () {
        setStarred(starred);
        setComponent(nullptr);
    });
    connect(si, &StarItem::succeeded, si, &QObject::deleteLater);
    setComponent(si);
    component()->execute();
    Q_EMIT inOperationChanged(inOperation());
}
