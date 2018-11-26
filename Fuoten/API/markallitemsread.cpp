/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016-2018 Hüssenbergnetz/Matthias Fehring
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

#include "markallitemsread_p.h"
#include <QJsonObject>
#include "../error.h"

using namespace Fuoten;

MarkAllItemsRead::MarkAllItemsRead(QObject *parent) :
    Component(* new MarkAllItemsReadPrivate, parent)
{
}


MarkAllItemsRead::MarkAllItemsRead(qint64 newestItemId, QObject *parent) :
    Component(* new MarkAllItemsReadPrivate(newestItemId), parent)
{
}


MarkAllItemsRead::MarkAllItemsRead(MarkAllItemsReadPrivate &dd, QObject *parent) :
    Component(dd, parent)
{
}


void MarkAllItemsRead::execute()
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Still in operation. Returning.");
        return;
    }

    qDebug("%s", "Start to mark all items on the server as read.");

    setInOperation(true);

    setError(nullptr);

    QJsonObject plo; // payload object
    plo.insert(QStringLiteral("newestItemId"), QJsonValue(newestItemId()));

    setPayload(plo);

    sendRequest();
}


bool MarkAllItemsRead::checkInput()
{
    if (Q_LIKELY(Component::checkInput())) {

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


void MarkAllItemsRead::successCallback()
{
    if (isUseStorageEnabled() && storage()) {
        storage()->allItemsMarkedRead(newestItemId());
    }

    setInOperation(false);

    qDebug("%s", "Successfully marked all items as read on the server.");

    Q_EMIT succeeded(newestItemId());
}


qint64 MarkAllItemsRead::newestItemId() const { Q_D(const MarkAllItemsRead); return d->newestItemId; }

void MarkAllItemsRead::setNewestItemId(qint64 nNewestItemId)
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Can not change property %s, still in operation.", "newestItemId");
        return;
    }

    Q_D(MarkAllItemsRead); 
    if (nNewestItemId != d->newestItemId) {
        d->newestItemId = nNewestItemId;
        qDebug("Changed newestItemId to %lli.", d->newestItemId);
        Q_EMIT newestItemIdChanged(newestItemId());
    }
}

#include "moc_markallitemsread.cpp"
