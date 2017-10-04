/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016 Buschtrommel / Matthias Fehring
 * https://www.buschmann23.de/entwicklung/bibliotheken/libfuoten/
 * https://github.com/Buschtrommel/libfuoten
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

#include "markmultipleitems_p.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include "../error.h"

using namespace Fuoten;

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
            setInOperation(false);
            Q_EMIT failed(error());
            return false;
        }

    } else {
        setInOperation(false);
        return false;
    }

    return true;
}


void MarkMultipleItems::successCallback()
{
    if (storage()) {
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




