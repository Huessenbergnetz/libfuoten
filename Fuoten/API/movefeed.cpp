/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016-2017 Matthias Fehring
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

#include "movefeed_p.h"
#include <QJsonObject>
#include <QJsonValue>
#include "../error.h"

using namespace Fuoten;


MoveFeed::MoveFeed(QObject *parent) :
    Component(* new MoveFeedPrivate, parent)
{
}


MoveFeed::MoveFeed(MoveFeedPrivate &dd, QObject *parent) :
    Component(dd, parent)
{
}


void MoveFeed::execute()
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Still in operation. Returning.");
        return;
    }

    qDebug("Start to move feed with ID %lli to folder with ID %lli.", feedId(), folderId());

    setInOperation(true);

    setError(nullptr);

    QStringList route(QStringLiteral("feeds"));
    route.append(QString::number(feedId()));
    route.append(QStringLiteral("move"));
    setApiRoute(route);

    QJsonObject plo; // payload object
    plo.insert(QStringLiteral("folderId"), QJsonValue(folderId()));

    setPayload(plo);

    sendRequest();
}


bool MoveFeed::checkInput()
{
    if (Q_LIKELY(Component::checkInput())) {

        if (Q_UNLIKELY(feedId() <= 0)) {
            //% "The feed ID is not valid."
            setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-invalid-feed-id"), QString(), this));
            Q_EMIT failed(error());
            return false;
        }

        if (Q_UNLIKELY(folderId() < 0)) {
            //% "The folder ID is not valid."
            setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-invalid-folder-id"), QString(), this));
            Q_EMIT failed(error());
            return false;
        }

    } else {
        return false;
    }

    return true;
}


void MoveFeed::extractError(QNetworkReply *reply)
{
    Q_ASSERT_X(reply, "extract error", "invalid QNetworkReply");

    if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 404) {
        //% "The feed was not found on the server."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-feed-not-exists"), QString(), this));
    } else {
        setError(new Error(reply, this));
    }

    Q_EMIT failed(error());
}


void MoveFeed::successCallback()
{
    if (isUseStorageEnabled() && storage()) {
        storage()->feedMoved(feedId(), folderId());
    }

    setInOperation(false);

    qDebug("Successfully moved feed with ID %lli to folder with ID %lli.", feedId(), folderId());

    Q_EMIT succeeded(feedId(), folderId());
}


qint64 MoveFeed::feedId() const { Q_D(const MoveFeed); return d->feedId; }

void MoveFeed::setFeedId(qint64 nFeedId)
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Can not change property %s, still in operation.", "feedId");
        return;
    }

    Q_D(MoveFeed); 
    if (nFeedId != d->feedId) {
        d->feedId = nFeedId;
        qDebug("Changed feedId to %lli.", d->feedId);
        Q_EMIT feedIdChanged(feedId());
    }
}


qint64 MoveFeed::folderId() const { Q_D(const MoveFeed); return d->folderId; }

void MoveFeed::setFolderId(qint64 nFolderId)
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Can not change property %s, still in operation.", "folderId");
        return;
    }

    Q_D(MoveFeed); 
    if (nFolderId != d->folderId) {
        d->folderId = nFolderId;
        qDebug("Changed folderId to %lli.", d->folderId);
        Q_EMIT folderIdChanged(folderId());
    }
}

#include "moc_movefeed.cpp"
