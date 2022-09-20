/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "markallitemsread_p.h"
#include <QJsonObject>
#include "../error.h"

using namespace Fuoten;

MarkAllItemsReadPrivate::MarkAllItemsReadPrivate() :
    ComponentPrivate(),
    newestItemId(0)
{

}


MarkAllItemsReadPrivate::MarkAllItemsReadPrivate(qint64 nNewestItemId) :
    ComponentPrivate(),
    newestItemId(nNewestItemId)
{

}


MarkAllItemsReadPrivate::~MarkAllItemsReadPrivate()
{

}


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


MarkAllItemsRead::~MarkAllItemsRead()
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
