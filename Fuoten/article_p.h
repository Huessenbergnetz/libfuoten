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
                   FuotenEnums::QueueActions nQueue);

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
