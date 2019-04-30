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

#ifndef FUOTENARTICLE_P_H
#define FUOTENARTICLE_P_H

#include "article.h"
#include "baseitem_p.h"

namespace Fuoten {

class ArticlePrivate : public BaseItemPrivate
{
public:
    ArticlePrivate() :
        BaseItemPrivate()
    {}

    ArticlePrivate(qint64 nId,
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
                   FuotenEnums::QueueActions nQueue) :
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
        url(nUrl),
        enclosureLink(nEnclosureLink),
        pubDate(nPubDate),
        lastModified(nLastModified),
        queue(nQueue),
        unread(nUnread),
        starred(nStarred)
    {
        createHumanPubDateTime();
    }

    ArticlePrivate(Article *other) :
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
            createHumanPubDateTime();
        }
    }

    void createHumanPubDateTime() {

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

    qint64 feedId = 0;
    qint64 folderId = 0;
    QString feedTitle;
    QString guid;
    QString guidHash;
    QString title;
    QString author;
    QString body;
    QString enclosureMime;
    QString fingerprint;
    QString folderName;
    QString humanPubDate;
    QString humanPubTime;
    QUrl url;
    QUrl enclosureLink;
    QDateTime pubDate;
    QDateTime lastModified;
    FuotenEnums::QueueActions queue = 0;
    bool unread = false;
    bool starred = false;

private:
    Q_DISABLE_COPY(ArticlePrivate)
};

}

#endif // FUOTENARTICLE_P_H
