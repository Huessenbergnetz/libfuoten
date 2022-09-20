/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "staritem_p.h"
#include "../error.h"

using namespace Fuoten;

StarItemPrivate::StarItemPrivate() :
    ComponentPrivate(),
    feedId(0),
    starred(false)
{
    expectedJSONType = Component::Empty;
    namOperation = QNetworkAccessManager::PutOperation;
}


StarItemPrivate::StarItemPrivate(qint64 nFeedId, const QString &nGuidHash, bool nStarred) :
    ComponentPrivate(),
    feedId(nFeedId),
    guidHash(nGuidHash),
    starred(nStarred)
{
    expectedJSONType = Component::Empty;
    namOperation = QNetworkAccessManager::PutOperation;
}


StarItemPrivate::~StarItemPrivate()
{

}


StarItem::StarItem(QObject *parent) :
   Component(* new StarItemPrivate, parent)
{
}


StarItem::StarItem(qint64 feedId, const QString &guidHash, bool starred, QObject *parent) :
    Component(* new StarItemPrivate(feedId, guidHash, starred), parent)
{
}


StarItem::StarItem(StarItemPrivate &dd, QObject *parent) :
    Component(dd, parent)
{
}


StarItem::~StarItem()
{
}


void StarItem::execute()
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Still in operation. Returning.");
        return;
    }

    qDebug("Start to %s the item with GUID %s on the server.", starred() ? "start" : "unstar", qUtf8Printable(guidHash()));

    setInOperation(true);

    setError(nullptr);

    QStringList rl(QStringLiteral("items"));
    rl.append(QString::number(feedId()));
    rl.append(guidHash());
    if (starred()) {
        rl.append(QStringLiteral("star"));
    } else {
        rl.append(QStringLiteral("unstar"));
    }
    setApiRoute(rl);

    setPayload(QByteArrayLiteral("{}"));

    sendRequest();
}


bool StarItem::checkInput()
{
    if (Q_LIKELY(Component::checkInput())) {

        if (Q_UNLIKELY(feedId() <= 0)) {
            //% "The feed ID is not valid."
            setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-invalid-feed-id"), QString(), this));
            Q_EMIT failed(error());
            return false;
        }

        if (Q_UNLIKELY(guidHash().isEmpty())) {
            //% "The GUID hash can not be empty."
            setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-empty-guidhash"), QString(), this));
            Q_EMIT failed(error());
            return false;
        }

    } else {
        setInOperation(false);
        return false;
    }

    return true;
}


void StarItem::successCallback()
{
    if (isUseStorageEnabled() && storage()) {
        storage()->itemStarred(feedId(), guidHash(), starred());
    }

    setInOperation(false);

    qDebug("Successfully %s the item with GUID %s on the server.", starred() ? "starred" : "unstarred", qUtf8Printable(guidHash()));

    Q_EMIT succeeded(feedId(), guidHash(), starred());
}


void StarItem::extractError(QNetworkReply *reply)
{
    Q_ASSERT_X(reply, "extract error", "invalid QNetworkReply");

    if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 404) {
        //% "The article was not found on the server."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-article-not-exists"), QString(), this));
        Q_EMIT failed(error());
    } else {
        Component::extractError(reply);
    }
}


qint64 StarItem::feedId() const { Q_D(const StarItem); return d->feedId; }

void StarItem::setFeedId(qint64 nFeedId)
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Can not change property %s, still in operation.", "feedId");
        return;
    }

    Q_D(StarItem);
    if (nFeedId != d->feedId) {
        d->feedId = nFeedId;
        qDebug("Changed feedId to %lli.", d->feedId);
        Q_EMIT feedIdChanged(feedId());
    }
}


QString StarItem::guidHash() const { Q_D(const StarItem); return d->guidHash; }

void StarItem::setGuidHash(const QString &nGuidHash)
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Can not change property %s, still in operation.", "guidHash");
        return;
    }

    Q_D(StarItem);
    if (nGuidHash != d->guidHash) {
        d->guidHash = nGuidHash;
        qDebug("Changed guidHash to \"%s\".", qUtf8Printable(d->guidHash));
        Q_EMIT guidHashChanged(guidHash());
    }
}


bool StarItem::starred() const { Q_D(const StarItem); return d->starred; }

void StarItem::setStarred(bool nStarred)
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Can not change property %s, still in operation.", "starred");
        return;
    }

    Q_D(StarItem);
    if (nStarred != d->starred) {
        d->starred = nStarred;
        qDebug("Changed starred to %s.", d->starred ? "true" : "false");
        Q_EMIT starredChanged(starred());
    }
}

#include "moc_staritem.cpp"
