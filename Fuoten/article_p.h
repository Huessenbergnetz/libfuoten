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

#ifndef FUOTENARTICLE_P_H
#define FUOTENARTICLE_P_H

#include "article.h"
#include "baseitem_p.h"

namespace Fuoten {

class ArticlePrivate : public BaseItemPrivate
{
public:
    ArticlePrivate() :
        BaseItemPrivate(),
        feedId(0),
        unread(false),
        starred(false),
        folderId(0)
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
                   const QString &nFolderName) :
        BaseItemPrivate(nId),
        feedId(nFeedId),
        feedTitle(nFeedTitle),
        guid(nGuid),
        guidHash(nGuidHash),
        url(nUrl),
        title(nTitle),
        author(nAuthor),
        pubDate(nPubDate),
        body(nBody),
        enclosureMime(nEnclosureMime),
        enclosureLink(nEnclosureLink),
        unread(nUnread),
        starred(nStarred),
        lastModified(nLastModified),
        fingerprint(nFingerprint),
        folderId(nFolderId),
        folderName(nFolderName)
    {
        createHumanPubDate();
    }

    void createHumanPubDate() {
        QDateTime lt = pubDate.toLocalTime();
        QDate cd = QDate::currentDate();
        if (lt.date() == cd) {
            //% "Today, %1"
            humanPubDate = qtTrId("libfuoten-today-datetime").arg(
                                                            //% "hh:mm"
                                                            lt.toString(qtTrId("libfuoten-time-format")
                                                           ));
        } else if (lt.daysTo(QDateTime::currentDateTime()) == 1 ) {
            //% "Yesterday, %1"
            humanPubDate = qtTrId("libfuoten-yesterday-datetime").arg(
                        //% "hh:mm"
                        lt.toString(qtTrId("libfuoten-time-format")
                       ));
        } else if (lt.daysTo(QDateTime::currentDateTime()) < 7) {
            //% "dddd, hh:mm"
            humanPubDate = lt.toString(qtTrId("libfuoten-day-time-format"));
        } else if (lt.daysTo(QDateTime::currentDateTime()) < 365) {
            //% "d. MMMM, hh:mm"
            humanPubDate = lt.toString(qtTrId("libfuoten-short-datetime"));
        } else {
            //% "d. MMM yyyy, hh:mm"
            humanPubDate = lt.toString(qtTrId("libfuoten-date-time-format"));
        }
    }

    qint64 feedId;
    QString feedTitle;
    QString guid;
    QString guidHash;
    QUrl url;
    QString title;
    QString author;
    QDateTime pubDate;
    QString body;
    QString enclosureMime;
    QUrl enclosureLink;
    bool unread;
    bool starred;
    QDateTime lastModified;
    QString fingerprint;
    qint64 folderId;
    QString folderName;
    QString humanPubDate;
};

}

#endif // FUOTENARTICLE_P_H
