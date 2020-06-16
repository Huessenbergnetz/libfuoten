/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016-2020 Hüssenbergnetz/Matthias Fehring
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
