/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENFEED_P_H
#define FUOTENFEED_P_H

#include "feed.h"
#include "baseitem_p.h"

namespace Fuoten {

class FeedPrivate : public BaseItemPrivate
{
public:
    FeedPrivate();

    FeedPrivate(qint64 _id, qint64 _folderId, const QString &_title, const QUrl &_url, const QUrl &_link, const QDateTime &_added, uint _unreadCount, Feed::FeedOrdering _ordering, bool _pinned, uint _updateErrorCount, const QString &_lastUpdateError, const QUrl &_faviconLink, const QString &_folderName);

    ~FeedPrivate() override;

    qint64 folderId = 0;
    QString title;
    QString folderName;
    QString lastUpdateError;
    QUrl url;
    QUrl link;
    QUrl faviconLink;
    QDateTime added;
    uint unreadCount = 0;
    uint updateErrorCount = 0;
    Feed::FeedOrdering ordering = Feed::NoSpecial;
    bool pinned = false;

private:
    Q_DISABLE_COPY(FeedPrivate)
};

}

#endif // FUOTENFEED_P_H
