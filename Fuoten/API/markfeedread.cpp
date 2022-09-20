/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "markfeedread_p.h"
#include <QJsonObject>
#include "../error.h"

using namespace Fuoten;

MarkFeedReadPrivate::MarkFeedReadPrivate() :
    ComponentPrivate(),
    feedId(0),
    newestItemId(0)
{
    expectedJSONType = Component::Empty;
    namOperation = QNetworkAccessManager::PutOperation;
}


MarkFeedReadPrivate::~MarkFeedReadPrivate()
{

}


MarkFeedRead::MarkFeedRead(QObject *parent) :
    Component(* new MarkFeedReadPrivate, parent)
{
}


MarkFeedRead::MarkFeedRead(MarkFeedReadPrivate &dd, QObject *parent) :
    Component(dd, parent)
{
}


MarkFeedRead::~MarkFeedRead()
{

}


void MarkFeedRead::execute()
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Still in operation. Returning.");
        return;
    }

    qDebug("Start to mark all items in feed with ID %lli as read on server. Newest item ID: %lli.", feedId(), newestItemId());

    setInOperation(true);

    setError(nullptr);

    QStringList rl(QStringLiteral("feeds"));
    rl.append(QString::number(feedId()));
    rl.append(QStringLiteral("read"));
    setApiRoute(rl);

    QJsonObject plo; // payload object
    plo.insert(QStringLiteral("newestItemId"), QJsonValue(newestItemId()));

    setPayload(plo);

    sendRequest();
}


bool MarkFeedRead::checkInput()
{
    if (Q_LIKELY(Component::checkInput())) {

        if (Q_UNLIKELY(feedId() <= 0)) {
            //% "The feed ID is not valid."
            setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-invalid-feed-id"), QString(), this));
            Q_EMIT failed(error());
            return false;
        }

        if (Q_UNLIKELY(newestItemId() <= 0)) {
            //% "The item ID is not valid."
            setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-invalid-item-id"), QString(), this));
            Q_EMIT failed(error());
            return false;
        }

    } else {
        return false;
    }

    return true;
}


void MarkFeedRead::successCallback()
{
    if (isUseStorageEnabled() && storage()) {
        storage()->feedMarkedRead(feedId(), newestItemId());
    }

    setInOperation(false);

    qDebug("Successfully marked the feed with ID %lli as read on the server. Newest itme ID: %lli.", feedId(), newestItemId());

    Q_EMIT succeeded(feedId(), newestItemId());
}


void MarkFeedRead::extractError(QNetworkReply *reply)
{
    Q_ASSERT_X(reply, "extract error", "invalid QNetworkReply");

    if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 404) {
        //% "The feed was not found on the server."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-feed-not-exists"), QString(), this));
        Q_EMIT failed(error());
    } else {
        Component::extractError(reply);
    }
}


qint64 MarkFeedRead::feedId() const { Q_D(const MarkFeedRead); return d->feedId; }

void MarkFeedRead::setFeedId(qint64 nFeedId)
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Can not change property %s, still in operation.", "feedId");
        return;
    }

    Q_D(MarkFeedRead);
    if (nFeedId != d->feedId) {
        d->feedId = nFeedId;
        qDebug("Changed feedId to %lli.", d->feedId);
        Q_EMIT feedIdChanged(feedId());
    }
}


qint64 MarkFeedRead::newestItemId() const { Q_D(const MarkFeedRead); return d->newestItemId; }

void MarkFeedRead::setNewestItemId(qint64 nNewestItemId)
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Can not change property %s, still in operation.", "newestItemId");
        return;
    }

    Q_D(MarkFeedRead);
    if (nNewestItemId != d->newestItemId) {
        d->newestItemId = nNewestItemId;
        qDebug("Changed newestItemId to %lli.", d->newestItemId);
        Q_EMIT newestItemIdChanged(newestItemId());
    }
}

#include "moc_markfeedread.cpp"
