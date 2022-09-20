/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "markmultipleitems_p.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include "../error.h"

using namespace Fuoten;

MarkMultipleItemsPrivate::MarkMultipleItemsPrivate() :
    ComponentPrivate(),
    unread(false)
{
    expectedJSONType = Component::Empty;
    namOperation = QNetworkAccessManager::PutOperation;
}

MarkMultipleItemsPrivate::MarkMultipleItemsPrivate(const IdList &nItemIds, bool nUnread) :
    ComponentPrivate(),
    itemIds(nItemIds),
    unread(nUnread)
{
    expectedJSONType = Component::Empty;
    namOperation = QNetworkAccessManager::PutOperation;
}


MarkMultipleItemsPrivate::~MarkMultipleItemsPrivate()
{
}


MarkMultipleItems::MarkMultipleItems(QObject *parent) :
    Component(* new MarkMultipleItemsPrivate, parent)
{
}


MarkMultipleItems::MarkMultipleItems(const IdList &itemIds, bool unread, QObject *parent) :
    Component(* new MarkMultipleItemsPrivate(itemIds, unread), parent)
{
}


MarkMultipleItems::MarkMultipleItems(MarkMultipleItemsPrivate &dd, QObject *parent) :
    Component(dd, parent)
{
}


MarkMultipleItems::~MarkMultipleItems()
{
}


void MarkMultipleItems::execute()
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Still in operation. Returning.");
        return;
    }

    qDebug("Start to mark mutltiple items as %s on the server.", unread() ? "unread" : "read");

    setInOperation(true);

    setError(nullptr);

    if (unread()) {
        setApiRoute(QStringLiteral("/items/unread/multiple"));
    } else {
        setApiRoute(QStringLiteral("/items/read/multiple"));
    }

    QJsonArray ia;
    if (!itemIds().isEmpty()) {
        const IdList iids = itemIds();
        for (qint64 id : iids) {
            ia.append(QJsonValue(id));
        }
    }

    QJsonObject plo; // payload object
    plo.insert(QStringLiteral("items"), QJsonValue(ia));

    setPayload(plo);

    sendRequest();
}


bool MarkMultipleItems::checkInput()
{
    if (Q_LIKELY(Component::checkInput())) {

        if (Q_UNLIKELY(itemIds().isEmpty())) {
            //% "The list of IDs to mark is empty."
            setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-empty-id-list"), QString(), this));
            Q_EMIT failed(error());
            return false;
        }

    } else {
        return false;
    }

    return true;
}


void MarkMultipleItems::successCallback()
{
    if (isUseStorageEnabled() && storage()) {
        storage()->itemsMarked(itemIds(), unread());
    }

    setInOperation(false);

    qDebug("Successfully marked multiple items as %s on the server.", unread() ? "unread" : "read");

    Q_EMIT succeeded(itemIds(), unread());
}


IdList MarkMultipleItems::itemIds() const { Q_D(const MarkMultipleItems); return d->itemIds; }

void MarkMultipleItems::setItemIds(const IdList &nItemIds)
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Can not change property %s, still in operation.", "itemIds");
        return;
    }

    Q_D(MarkMultipleItems);
    if (nItemIds != d->itemIds) {
        d->itemIds = nItemIds;
        qDebug("Changed itemIds.");
        Q_EMIT itemIdsChanged(itemIds());
    }
}


bool MarkMultipleItems::unread() const { Q_D(const MarkMultipleItems); return d->unread; }

void MarkMultipleItems::setUnread(bool nUnread)
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Can not change property %s, still in operation.", "unread");
        return;
    }

    Q_D(MarkMultipleItems);
    if (nUnread != d->unread) {
        d->unread = nUnread;
        qDebug("Changed unread to %s.", d->unread ? "true" : "false");
        Q_EMIT unreadChanged(unread());
    }
}

#include "moc_markmultipleitems.cpp"
