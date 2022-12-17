/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENARTICLE_P_H
#define FUOTENARTICLE_P_H

#include "article.h"
#include "baseitem_p.h"

namespace Fuoten {

class ArticlePrivate : public BaseItemPrivate
{
public:
    ArticlePrivate();

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
                   FuotenEnums::QueueActions nQueue,
                   bool nRtl,
                   const QUrl &nMediaThumbnail,
                   const QString &nMediaDescription);

    ArticlePrivate(Article *other);

    ~ArticlePrivate() override;

    void createHumanPubDateTime();

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
    QString mediaDescription;
    QUrl url;
    QUrl enclosureLink;
    QUrl mediaThumbnail;
    QDateTime pubDate;
    QDateTime lastModified;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
    FuotenEnums::QueueActions queue;
#else
    FuotenEnums::QueueActions queue = 0;
#endif
    bool unread = false;
    bool starred = false;
    bool rtl = false;

private:
    Q_DISABLE_COPY(ArticlePrivate)
};

}

#endif // FUOTENARTICLE_P_H
