/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "article_p.h"
#include "API/component.h"
#include "API/markitem.h"
#include "API/staritem.h"
#include "fuoten.h"
#include "error.h"

using namespace Fuoten;

ArticlePrivate::ArticlePrivate() :
    BaseItemPrivate()
{

}

ArticlePrivate::ArticlePrivate(qint64 nId,
               qint64 nFeedId,
               const QString &nFeedTitle,
               const QString &nGuid,
               const QString &nGuidHash,
               const QUrl &nUrl,
               const QString &nTitle,
               const QString &nAuthor,
               const QDateTime &nPubDate,
               const QString &nBody,
               const QString &nEnclosureMime,
               const QUrl &nEnclosureLink,
               bool nUnread,
               bool nStarred,
               const QDateTime &nLastModified,
               const QString &nFingerprint,
               qint64 nFolderId,
               const QString &nFolderName,
               FuotenEnums::QueueActions nQueue,
               bool nRtl,
               const QUrl &nMediaThumbnail,
               const QString &nMediaDescription) :
    BaseItemPrivate(nId),
    feedId(nFeedId),
    folderId(nFolderId),
    feedTitle(nFeedTitle),
    guid(nGuid),
    guidHash(nGuidHash),
    title(nTitle),
    author(nAuthor),
    body(nBody),
    enclosureMime(nEnclosureMime),
    fingerprint(nFingerprint),
    folderName(nFolderName),
    mediaDescription(nMediaDescription),
    url(nUrl),
    enclosureLink(nEnclosureLink),
    mediaThumbnail(nMediaThumbnail),
    pubDate(nPubDate),
    lastModified(nLastModified),
    queue(nQueue),
    unread(nUnread),
    starred(nStarred),
    rtl(nRtl)
{
    createHumanPubDateTime();
}

ArticlePrivate::ArticlePrivate(Article *other) :
    BaseItemPrivate()
{
    if (other) {
        id = other->id();
        feedId = other->feedId();
        feedTitle = other->feedTitle();
        guid = other->guid();
        guidHash = other->guidHash();
        url = other->url();
        title = other->title();
        author = other->author();
        pubDate = other->pubDate();
        body = other->body();
        enclosureLink = other->enclosureLink();
        enclosureMime = other->enclosureMime();
        unread = other->unread();
        starred = other->starred();
        lastModified = other->lastModified();
        fingerprint = other->fingerprint();
        folderId = other->folderId();
        folderName = other->folderName();
        queue = other->queue();
        rtl = other->rtl();
        mediaThumbnail = other->mediaThumbnail();
        mediaDescription = other->mediaDescription();
        createHumanPubDateTime();
    }
}


ArticlePrivate::~ArticlePrivate()
{

}


void ArticlePrivate::createHumanPubDateTime()
{

    QTime lt = pubDate.toLocalTime().time();    // the local publication time
    QDate ld = pubDate.toLocalTime().date();    // the local publication date
    QDate cd = QDate::currentDate();            // the current date

    qint64 dayDiff = ld.daysTo(cd);
    if (dayDiff == 0) {
        //% "Today"
        humanPubDate = qtTrId("libfuoten-tody");
    } else if (dayDiff == 1) {
        //% "Yesterday"
        humanPubDate = qtTrId("libfuoten-yesterday");
    } else if (dayDiff < 7) {
        humanPubDate = ld.toString(QStringLiteral("dddd"));
    } else if ((dayDiff < 365) && (dayDiff > -365)) {
        //% "d. MMMM"
        humanPubDate = ld.toString(qtTrId("libfuoten-short-date-format"));
    } else {
        //% "d. MMM yyyy"
        humanPubDate = ld.toString(qtTrId("libfuoten-long-date-format"));
    }

    //% "hh:mm"
    humanPubTime = lt.toString(qtTrId("libfuoten-time-format"));
}


Article::Article(QObject *parent) :
    BaseItem(* new ArticlePrivate, parent)
{
    qDebug("Creating new empty Article object at %p.", this);
}


Article::Article(qint64 id, qint64 feedId, const QString &feedTitle, const QString &guid, const QString &guidHash, const QUrl &url, const QString &title, const QString &author, const QDateTime &pubDate, const QString &body, const QString &enclosureMime, const QUrl &enclosureLink, bool unread, bool starred, const QDateTime &lastModified, const QString &fingerprint, qint64 folderId, const QString &folderName, FuotenEnums::QueueActions queue, bool rtl, const QUrl &mediaThumbnail, const QString &mediaDescription, QObject *parent) :
    BaseItem(* new ArticlePrivate(id, feedId, feedTitle, guid, guidHash, url, title, author, pubDate, body, enclosureMime, enclosureLink, unread, starred, lastModified, fingerprint, folderId, folderName, queue, rtl, mediaThumbnail, mediaDescription), parent)
{
    qDebug("Creating new Article object (ID: %lli, Title: %s) at %p.", id, qUtf8Printable(title), this);
}


Article::Article(Article *other, QObject *parent) :
    BaseItem(* new ArticlePrivate(other), parent)
{
}


Article::Article(ArticlePrivate &dd, QObject *parent) :
    BaseItem(dd, parent)
{

}


Article::~Article()
{

}


qint64 Article::feedId() const { Q_D(const Article); return d->feedId; }

void Article::setFeedId(qint64 nFeedId)
{
    Q_D(Article);
    if (nFeedId != d->feedId) {
        d->feedId = nFeedId;
        qDebug("Changed feedId to %lli.", d->feedId);
        Q_EMIT feedIdChanged(feedId());
    }
}


QString Article::feedTitle() const { Q_D(const Article); return d->feedTitle; }

void Article::setFeedTitle(const QString &nFeedTitle)
{
    Q_D(Article);
    if (nFeedTitle != d->feedTitle) {
        d->feedTitle = nFeedTitle;
        qDebug("Changed feedTitle to \"%s\".", qUtf8Printable(d->feedTitle));
        Q_EMIT feedTitleChanged(feedTitle());
    }
}


QString Article::guid() const { Q_D(const Article); return d->guid; }

void Article::setGuid(const QString &nGuid)
{
    Q_D(Article);
    if (nGuid != d->guid) {
        d->guid = nGuid;
        qDebug("Changed guid to \"%s\".", qUtf8Printable(d->guid));
        Q_EMIT guidChanged(guid());
    }
}


QString Article::guidHash() const { Q_D(const Article); return d->guidHash; }

void Article::setGuidHash(const QString &nGuidHash)
{
    Q_D(Article);
    if (nGuidHash != d->guidHash) {
        d->guidHash = nGuidHash;
        qDebug("Changed guidHash to \"%s\".", qUtf8Printable(d->guidHash));
        Q_EMIT guidHashChanged(guidHash());
    }
}


QUrl Article::url() const { Q_D(const Article); return d->url; }

void Article::setUrl(const QUrl &nUrl)
{
    Q_D(Article);
    if (nUrl != d->url) {
        d->url = nUrl;
        qDebug("Changed url to %s.", qUtf8Printable(d->url.toString()));
        Q_EMIT urlChanged(url());
    }
}


QString Article::title() const { Q_D(const Article); return d->title; }

void Article::setTitle(const QString &nTitle)
{
    Q_D(Article);
    if (nTitle != d->title) {
        d->title = nTitle;
        qDebug("Changed title to \"%s\".", qUtf8Printable(d->title));
        Q_EMIT titleChanged(title());
    }
}


QString Article::author() const { Q_D(const Article); return d->author; }

void Article::setAuthor(const QString &nAuthor)
{
    Q_D(Article);
    if (nAuthor != d->author) {
        d->author = nAuthor;
        qDebug("Changed author to \"%s\".", qUtf8Printable(d->author));
        Q_EMIT authorChanged(author());
    }
}


QDateTime Article::pubDate() const { Q_D(const Article); return d->pubDate; }

void Article::setPubDate(const QDateTime &nPubDate)
{
    Q_D(Article);
    if (nPubDate != d->pubDate) {
        d->pubDate = nPubDate;
        qDebug("Changed pubDate to %s.", qUtf8Printable(d->pubDate.toString(Qt::ISODate)));
        Q_EMIT pubDateChanged(pubDate());
        d->createHumanPubDateTime();
        Q_EMIT humanPubDateChanged(humanPubDate());
        Q_EMIT humanPubTimeChanged(humanPubTime());
    }
}


QString Article::body() const { Q_D(const Article); return d->body; }

void Article::setBody(const QString &nBody)
{
    Q_D(Article);
    if (nBody != d->body) {
        d->body = nBody;
        qDebug("%s", "Changed body.");
        Q_EMIT bodyChanged(body());
    }
}


QString Article::enclosureMime() const { Q_D(const Article); return d->enclosureMime; }

void Article::setEnclosureMime(const QString &nEnclosureMime)
{
    Q_D(Article);
    if (nEnclosureMime != d->enclosureMime) {
        d->enclosureMime = nEnclosureMime;
        qDebug("Changed enclosureMime to \"%s\".", qUtf8Printable(d->enclosureMime));
        Q_EMIT enclosureMimeChanged(enclosureMime());
    }
}


QUrl Article::enclosureLink() const { Q_D(const Article); return d->enclosureLink; }

void Article::setEnclosureLink(const QUrl &nEnclosureLink)
{
    Q_D(Article);
    if (nEnclosureLink != d->enclosureLink) {
        d->enclosureLink = nEnclosureLink;
        qDebug("Changed enclosureLink to %s.", qUtf8Printable(d->enclosureLink.toString()));
        Q_EMIT enclosureLinkChanged(enclosureLink());
    }
}


bool Article::unread() const { Q_D(const Article); return d->unread; }

void Article::setUnread(bool nUnread)
{
    Q_D(Article);
    if (nUnread != d->unread) {
        d->unread = nUnread;
        qDebug("Changed unread to %s.", d->unread ? "true" : "false");
        Q_EMIT unreadChanged(unread());
    }
}


bool Article::starred() const { Q_D(const Article); return d->starred; }

void Article::setStarred(bool nStarred)
{
    Q_D(Article);
    if (nStarred != d->starred) {
        d->starred = nStarred;
        qDebug("Changed starred to %s.", d->starred ? "true" : "false");
        Q_EMIT starredChanged(starred());
    }
}


QDateTime Article::lastModified() const { Q_D(const Article); return d->lastModified; }

void Article::setLastModified(const QDateTime &nLastModified)
{
    Q_D(Article);
    if (nLastModified != d->lastModified) {
        d->lastModified = nLastModified;
        qDebug("Changed lastModified to %s.", qUtf8Printable(d->lastModified.toString(Qt::ISODate)));
        Q_EMIT lastModifiedChanged(lastModified());
    }
}


QString Article::fingerprint() const { Q_D(const Article); return d->fingerprint; }

void Article::setFingerprint(const QString &nFingerprint)
{
    Q_D(Article);
    if (nFingerprint != d->fingerprint) {
        d->fingerprint = nFingerprint;
        qDebug("Changed fingerprint to \"%s\".", qUtf8Printable(d->fingerprint));
        Q_EMIT fingerprintChanged(fingerprint());
    }
}


qint64 Article::folderId() const { Q_D(const Article); return d->folderId; }

void Article::setFolderId(qint64 nFolderId)
{
    Q_D(Article);
    if (nFolderId != d->folderId) {
        d->folderId = nFolderId;
        qDebug("Changed folderId to %lli.", d->folderId);
        Q_EMIT folderIdChanged(folderId());
    }
}


QString Article::folderName() const { Q_D(const Article); return d->folderName; }

void Article::setFolderName(const QString &nFolderName)
{
    Q_D(Article);
    if (nFolderName != d->folderName) {
        d->folderName = nFolderName;
        qDebug("Changed folderName to \"%s\".", qUtf8Printable(d->folderName));
        Q_EMIT folderNameChanged(folderName());
    }
}


QString Article::humanPubDate() const { Q_D(const Article); return d->humanPubDate; }


QString Article::humanPubTime() const { Q_D(const Article); return d->humanPubTime; }


bool Article::rtl() const { Q_D(const Article); return d->rtl; }

void Article::setRtl(bool nRtl)
{
    Q_D(Article);
    if (nRtl != d->rtl) {
        qDebug("Changed rtl from %s to %s.", d->rtl ? "true" : "false", nRtl ? "true" : "false");
        d->rtl = nRtl;
        Q_EMIT rtlChanged(rtl());
    }
}


QUrl Article::mediaThumbnail() const { Q_D(const Article); return d->mediaThumbnail; }

void Article::setMediaThumbnail(const QUrl &nMediaThumbnail)
{
    Q_D(Article);
    if (nMediaThumbnail != d->mediaThumbnail) {
        qDebug("Changed mediaThumbnail from \"%s\" to \"%s\".", qUtf8Printable(d->mediaThumbnail.toString()), qUtf8Printable(nMediaThumbnail.toString()));
        d->mediaThumbnail = nMediaThumbnail;
        Q_EMIT mediaThumbnailChanged(mediaThumbnail());
    }
}


QString Article::mediaDescription() const { Q_D(const Article); return d->mediaDescription; }

void Article::setMediaDescription(const QString &nMediaDescription)
{
    Q_D(Article);
    if (nMediaDescription != d->mediaDescription) {
        qDebug("%s", "Changed mediaDescription.");
        d->mediaDescription = nMediaDescription;
        Q_EMIT mediaDescriptionChanged(mediaDescription());
    }
}


FuotenEnums::QueueActions Article::queue() const { Q_D(const Article); return d->queue; }

void Article::setQueue(FuotenEnums::QueueActions queue)
{
    Q_D(Article);
    d->queue = queue;
}


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
        setQueue(o->queue());
    } else {
        qCritical("Failed to cast BaseItem to Article when trying to create a deep copy!");
    }
}


void Article::mark(bool unread, AbstractConfiguration *config, AbstractStorage *storage, bool enqueue)
{
    Q_ASSERT_X(config, "mark article as read", "invalid configuration");

    if (Q_UNLIKELY(inOperation())) {
        qWarning("Item is still in operation.");
        return;
    }

    if (enqueue) {

        Q_ASSERT_X(storage, "enqueue mark article", "invalid storage");

        FuotenEnums::QueueAction action = FuotenEnums::MarkAsRead;
        if (unread) {
            action = FuotenEnums::MarkAsUnread;
        }

        if (!storage->enqueueItem(action, this)) {
            setError(storage->error());
        }

    } else {

        MarkItem *mi = new MarkItem(id(), unread, this);
        mi->setConfiguration(config);
        mi->setStorage(storage);
        connect(mi, &MarkItem::succeeded, this, [=] () {
            setUnread(unread);
            setComponent(nullptr);
        });
        connect(mi, &MarkItem::succeeded, mi, &QObject::deleteLater);
        setComponent(mi);
        component()->execute();
        Q_EMIT inOperationChanged(inOperation());
    }
}


void Article::mark(bool unread, bool enqueue)
{
    mark(unread, Component::defaultConfiguration(), Component::defaultStorage(), enqueue);
}


void Article::star(bool starred, AbstractConfiguration *config, AbstractStorage *storage, bool enqueue)
{
    Q_ASSERT_X(config, "star article", "invalid configuration");

    if (Q_UNLIKELY(inOperation())) {
        qWarning("Item is still in operation.");
        return;
    }

    if (enqueue) {

        Q_ASSERT_X(storage, "enqueue star article", "invalid storage");

        FuotenEnums::QueueAction action = FuotenEnums::Unstar;
        if (starred) {
            action = FuotenEnums::Star;
        }

        if (!storage->enqueueItem(action, this)) {
            setError(storage->error());
        }

    } else {

        StarItem *si = new StarItem(feedId(), guidHash(), starred, this);
        si->setConfiguration(config);
        si->setStorage(storage);
        connect(si, &StarItem::succeeded, this, [=] () {
            setStarred(starred);
            setComponent(nullptr);
        });
        connect(si, &StarItem::succeeded, si, &QObject::deleteLater);
        setComponent(si);
        component()->execute();
        Q_EMIT inOperationChanged(inOperation());
    }
}

void Article::star(bool starred, bool enqueue)
{
    star(starred, Component::defaultConfiguration(), Component::defaultStorage(), enqueue);
}


#include "moc_article.cpp"
