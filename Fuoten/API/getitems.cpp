/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "getitems_p.h"
#include "../error.h"
#include <QJsonObject>
#include <QJsonValue>
#include <QUrlQuery>
#include <QMetaEnum>

using namespace Fuoten;

GetItemsPrivate::GetItemsPrivate() :
    ComponentPrivate()
{
    apiRoute = QStringLiteral("/items");
    expectedJSONType = Component::Object;
}

GetItemsPrivate::GetItemsPrivate(int nBatchSize, qint64 nOffset, FuotenEnums::Type nType, qint64 nParentId, bool nGetRead, bool nOldestFirst) :
    ComponentPrivate(),
    offset(nOffset),
    parentId(nParentId),
    batchSize(nBatchSize),
    type(nType),
    getRead(nGetRead),
    oldestFirst(nOldestFirst)
{
    apiRoute = QStringLiteral("/items");
    expectedJSONType = Component::Object;
}

GetItemsPrivate::~GetItemsPrivate()
{

}


GetItems::GetItems(QObject *parent) :
    Component(* new GetItemsPrivate, parent)
{
}


GetItems::GetItems(int batchSize, qint64 offset, FuotenEnums::Type type, qint64 parentId, bool getRead, bool oldestFirst, QObject *parent) :
    Component(* new GetItemsPrivate(batchSize, offset, type, parentId, getRead, oldestFirst), parent)
{
}


GetItems::GetItems(GetItemsPrivate &dd, QObject *parent) :
    Component(dd, parent)
{
}


GetItems::~GetItems()
{

}


void GetItems::execute()
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Still in operation. Returning.");
        return;
    }

    qDebug("%s", "Start requesting items from the server.");

    setInOperation(true);

    QUrlQuery uq;

    QString bs = (batchSize() < 0) ? QStringLiteral("-1") : QString::number(batchSize());
    uq.addQueryItem(QStringLiteral("batchSize"), bs);

    if (offset() > 0) {
        uq.addQueryItem(QStringLiteral("offset"), QString::number(offset()));
    }

    uq.addQueryItem(QStringLiteral("type"), QString::number((int)type()));

    QString gr = getRead() ? QStringLiteral("true") : QStringLiteral("false");
    uq.addQueryItem(QStringLiteral("getRead"), gr);

    QString of = oldestFirst() ? QStringLiteral("true") : QStringLiteral("false");
    uq.addQueryItem(QStringLiteral("oldestFirst"), of);

    setUrlQuery(uq);

    sendRequest();
}


void GetItems::successCallback()
{
    if (isUseStorageEnabled() && storage()) {
        storage()->itemsRequested(jsonResult());
    }

    setInOperation(false);

    qDebug("Successfully requested the items from the server.");

    Q_EMIT succeeded(jsonResult());
}


bool GetItems::checkOutput()
{
    if (Q_LIKELY(Component::checkOutput())) {

        if (Q_UNLIKELY(!jsonResult().object().value(QStringLiteral("items")).isArray())) {
            //% "The data the server replied does not contain an \"items\" array."
            setError(new Error(Error::OutputError, Error::Critical, qtTrId("libfuoten-err-no-items-array-in-reply"), QString(), this));
            Q_EMIT failed(error());
            return false;
        }


    } else {
        setInOperation(false);
        return false;
    }

    return true;
}


bool GetItems::checkInput()
{
    if (Q_LIKELY(Component::checkInput())) {

        if (Q_UNLIKELY(parentId() < 0)) {
            //% "Invalid ID"
            setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-error-invalid-id"), QString::number(parentId()), this));
            Q_EMIT failed(error());
            return false;
        }

    } else {
        return false;
    }

    return true;
}


int GetItems::batchSize() const { Q_D(const GetItems); return d->batchSize; }

void GetItems::setBatchSize(int nBatchSize)
{
    if (inOperation()) {
        qWarning("Can not change property %s, still in operation.", "batchSize");
        return;
    }

    Q_D(GetItems);
    if (nBatchSize != d->batchSize) {
        d->batchSize = nBatchSize;
        qDebug("Changed batchSize to %i.", d->batchSize);
        Q_EMIT batchSizeChanged(batchSize());
    }
}


qint64 GetItems::offset() const { Q_D(const GetItems); return d->offset; }

void GetItems::setOffset(qint64 nOffset)
{
    if (inOperation()) {
        qWarning("Can not change property %s, still in operation.", "offset");
        return;
    }

    Q_D(GetItems);
    if (nOffset != d->offset) {
        d->offset = nOffset;
        qDebug("Changed offset to %lli.", d->offset);
        Q_EMIT offsetChanged(offset());
    }
}


FuotenEnums::Type GetItems::type() const { Q_D(const GetItems); return d->type; }

void GetItems::setType(FuotenEnums::Type nType)
{
    if (inOperation()) {
        qWarning("Can not change property %s, still in operation.", "type");
        return;
    }

    Q_D(GetItems);
    if (nType != d->type) {
        d->type = nType;
        qDebug("Changed type to %s.", FuotenEnums::staticMetaObject.enumerator(FuotenEnums::staticMetaObject.indexOfEnumerator("Type")).valueToKey(d->type));
        Q_EMIT typeChanged(type());
    }
}


qint64 GetItems::parentId() const { Q_D(const GetItems); return d->parentId; }

void GetItems::setParentId(qint64 nParentId)
{
    if (inOperation()) {
        qWarning("Can not change property %s, still in operation.", "parentId");
        return;
    }

    Q_D(GetItems);
    if (nParentId != d->parentId) {
        d->parentId = nParentId;
        qDebug("Changed parentId to %lli.", d->parentId);
        Q_EMIT parentIdChanged(parentId());
    }
}


bool GetItems::getRead() const { Q_D(const GetItems); return d->getRead; }

void GetItems::setGetRead(bool nGetRead)
{
    if (inOperation()) {
        qWarning("Can not change property %s, still in operation.", "getRead");
        return;
    }

    Q_D(GetItems);
    if (nGetRead != d->getRead) {
        d->getRead = nGetRead;
        qDebug("Changed getRead to %s.", d->getRead ? "true" : "false");
        Q_EMIT getReadChanged(getRead());
    }
}


bool GetItems::oldestFirst() const { Q_D(const GetItems); return d->oldestFirst; }

void GetItems::setOldestFirst(bool nOldestFirst)
{
    if (inOperation()) {
        qWarning("Can not change property %s, still in operation.", "oldestFirst");
        return;
    }

    Q_D(GetItems);
    if (nOldestFirst != d->oldestFirst) {
        d->oldestFirst = nOldestFirst;
        qDebug("Changed oldestFirst to %s.", d->oldestFirst ? "true" : "false");
        Q_EMIT oldestFirstChanged(oldestFirst());
    }
}

#include "moc_getitems.cpp"
