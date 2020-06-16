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

#include "deletefeed_p.h"
#include "../error.h"

using namespace Fuoten;


DeleteFeedPrivate::DeleteFeedPrivate() :
    ComponentPrivate()
{
    expectedJSONType = Component::Empty;
    namOperation = QNetworkAccessManager::DeleteOperation;
}


DeleteFeedPrivate::~DeleteFeedPrivate()
{

}


DeleteFeed::DeleteFeed(QObject *parent) :
    Component(* new DeleteFeedPrivate, parent)
{
}


DeleteFeed::DeleteFeed(DeleteFeedPrivate &dd, QObject *parent) :
    Component(dd, parent)
{
}


DeleteFeed::~DeleteFeed()
{

}


qint64 DeleteFeed::feedId() const { Q_D(const DeleteFeed); return d->feedId; }

void DeleteFeed::setFeedId(qint64 nFeedId)
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Can not change property %s, still in operation.", "feedId");
        return;
    }

    Q_D(DeleteFeed); 
    if (nFeedId != d->feedId) {
        d->feedId = nFeedId;
        qDebug("Changed feedId to %lli.", d->feedId);
        Q_EMIT feedIdChanged(feedId());
    }
}


void DeleteFeed::execute()
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Still in operation. Returning.");
        return;
    }

    qDebug("Start to delete feed with ID %lli.", feedId());

    setInOperation(true);

    setError(nullptr);

    QStringList rl(QStringLiteral("feeds"));
    rl.append(QString::number(feedId()));

    setApiRoute(rl);

    sendRequest();
}


bool DeleteFeed::checkInput()
{
    if (Q_LIKELY(Component::checkInput())) {

        if (Q_UNLIKELY(feedId() <= 0)) {
            //% "The feed ID is not valid."
            setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-invalid-feed-id"), QString(), this));
            Q_EMIT failed(error());
            return false;
        }

    } else {
        return false;
    }

    return true;
}


void DeleteFeed::successCallback()
{
    if (isUseStorageEnabled() && storage()) {
        storage()->feedDeleted(feedId());
    }

    setInOperation(false);

    qDebug("Successfully deleted feed with ID %lli.", feedId());

    Q_EMIT succeeded(feedId());
}


void DeleteFeed::extractError(QNetworkReply *reply)
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

#include "moc_deletefeed.cpp"
