/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "markitem_p.h"
#include "../error.h"

using namespace Fuoten;

MarkItemPrivate::MarkItemPrivate() :
    ComponentPrivate(),
    itemId(-1),
    unread(false)
{
    expectedJSONType = Component::Empty;
    namOperation = QNetworkAccessManager::PutOperation;
}


MarkItemPrivate::MarkItemPrivate(qint64 nItemId, bool nUnread) :
    itemId(nItemId),
    unread(nUnread)
{
    expectedJSONType = Component::Empty;
    namOperation = QNetworkAccessManager::PutOperation;
}


MarkItemPrivate::~MarkItemPrivate()
{
}


MarkItem::MarkItem(QObject *parent) :
    Component(* new MarkItemPrivate, parent)
{
}


MarkItem::MarkItem(qint64 itemId, bool unread, QObject *parent) :
    Component(* new MarkItemPrivate(itemId, unread), parent)
{
}


MarkItem::MarkItem(MarkItemPrivate &dd, QObject *parent) :
    Component(dd, parent)
{
}


MarkItem::~MarkItem()
{
}


void MarkItem::execute()
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Still in operation. Returning.");
        return;
    }

    qDebug("Start to mark item with ID %lli as %s on the server.", itemId(), unread() ? "unread" : "read");

    setInOperation(true);

    setError(nullptr);

    QStringList r(QStringLiteral("items"));
    r.append(QString::number(itemId()));
    if (unread()) {
        r.append(QStringLiteral("unread"));
    } else {
        r.append(QStringLiteral("read"));
    }

    setApiRoute(r);

    setPayload(QByteArrayLiteral("{}"));

    sendRequest();
}


bool MarkItem::checkInput()
{
    if (Q_LIKELY(Component::checkInput())) {

        if (Q_UNLIKELY(itemId() <= 0)) {
            //% "The article ID is not valid."
            setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-invalid-article-id"), QString(), this));
            Q_EMIT failed(error());
            return false;
        }

    } else {
        return false;
    }

    return true;
}


void MarkItem::successCallback()
{
    if (isUseStorageEnabled() && storage()) {
        storage()->itemMarked(itemId(), unread());
    }

    setInOperation(false);

    qDebug("Successfully marked the item with ID %lli as %s on the server.", itemId(), unread() ? "unread" : "read");

    Q_EMIT succeeded(itemId(), unread());
}


void MarkItem::extractError(QNetworkReply *reply)
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


qint64 MarkItem::itemId() const { Q_D(const MarkItem); return d->itemId; }

void MarkItem::setItemId(qint64 nItemId)
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Can not change property %s, still in operation.", "itemId");
        return;
    }

    Q_D(MarkItem);
    if (nItemId != d->itemId) {
        d->itemId = nItemId;
        qDebug("Changed itemId to %lli.", d->itemId);
        Q_EMIT itemIdChanged(itemId());
    }
}


bool MarkItem::unread() const { Q_D(const MarkItem); return d->unread; }

void MarkItem::setUnread(bool nUnread)
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Can not change property %s, still in operation.", "unread");
        return;
    }

    Q_D(MarkItem);
    if (nUnread != d->unread) {
        d->unread = nUnread;
        qDebug("Changed unread to %s.", d->unread ? "true" : "false");
        Q_EMIT unreadChanged(unread());
    }
}

#include "moc_markitem.cpp"
