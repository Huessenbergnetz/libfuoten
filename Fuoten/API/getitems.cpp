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

#include "getitems_p.h"
#include "../error.h"
#include <QJsonObject>
#include <QJsonValue>
#include <QUrlQuery>
#include <QMetaEnum>

using namespace Fuoten;

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
    if (storage()) {
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




bool GetItems::checkInput()
{
    if (Q_LIKELY(Component::checkInput())) {

        if (Q_UNLIKELY(parentId() < 0)) {
            //% "Invalid ID"
            setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-error-invalid-id"), QString::number(parentId()), this));
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




