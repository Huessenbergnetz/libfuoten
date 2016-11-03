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
#ifdef QT_DEBUG
#include <QtDebug>
#endif

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
    if (inOperation()) {
        qWarning("Still in operation. Returning.");
        return;
    }

#ifdef QT_DEBUG
    qDebug() << "Start requesting items from the server.";
#endif

    setInOperation(true);


    QJsonObject plo; // payload object
    if (batchSize() < 0) {
        plo.insert(QStringLiteral("batchSize"), QJsonValue(-1));
    } else {
        plo.insert(QStringLiteral("batchSize"), QJsonValue(batchSize()));
    }

    if (offset() > 0) {
        plo.insert(QStringLiteral("offset"), QJsonValue(offset()));
    }

    plo.insert(QStringLiteral("type"), QJsonValue((int)type()));
    plo.insert(QStringLiteral("id"), QJsonValue(parentId()));
    plo.insert(QStringLiteral("getRead"), QJsonValue(getRead()));
    plo.insert(QStringLiteral("oldestFirst"), QJsonValue(oldestFirst()));

    setPayload(plo);


    sendRequest();
}


void GetItems::successCallback()
{
    if (storage()) {
        storage()->itemsRequested(jsonResult());
    }

    setInOperation(false);

#ifdef QT_DEBUG
    qDebug() << "Successfully requested the items from the server.";
#endif

    Q_EMIT succeeded(jsonResult());
}



void GetItems::extractError(QNetworkReply *reply)
{
    setError(new Error(reply, this));
    setInOperation(false);
    Q_EMIT failed(error());
}



bool GetItems::checkOutput()
{
    if (Component::checkOutput()) {

        if (!jsonResult().object().value(QStringLiteral("items")).isArray()) {
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
    if (Component::checkInput()) {

        if (parentId() < 0) {
            //% "Invalid ID"
            setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-erro-invalid-id"), QString::number(parentId()), this));
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
    Q_D(GetItems); 
    if (nBatchSize != d->batchSize) {
        d->batchSize = nBatchSize;
#ifdef QT_DEBUG
        qDebug() << "Changed batchSize to" << d->batchSize;
#endif
        Q_EMIT batchSizeChanged(batchSize());
    }
}




qint64 GetItems::offset() const { Q_D(const GetItems); return d->offset; }

void GetItems::setOffset(qint64 nOffset)
{
    Q_D(GetItems); 
    if (nOffset != d->offset) {
        d->offset = nOffset;
#ifdef QT_DEBUG
        qDebug() << "Changed offset to" << d->offset;
#endif
        Q_EMIT offsetChanged(offset());
    }
}




FuotenEnums::Type GetItems::type() const { Q_D(const GetItems); return d->type; }

void GetItems::setType(FuotenEnums::Type nType)
{
    Q_D(GetItems); 
    if (nType != d->type) {
        d->type = nType;
#ifdef QT_DEBUG
        qDebug() << "Changed type to" << d->type;
#endif
        Q_EMIT typeChanged(type());
    }
}




qint64 GetItems::parentId() const { Q_D(const GetItems); return d->parentId; }

void GetItems::setParentId(qint64 nParentId)
{
    Q_D(GetItems); 
    if (nParentId != d->parentId) {
        d->parentId = nParentId;
#ifdef QT_DEBUG
        qDebug() << "Changed parentId to" << d->parentId;
#endif
        Q_EMIT parentIdChanged(parentId());
    }
}




bool GetItems::getRead() const { Q_D(const GetItems); return d->getRead; }

void GetItems::setGetRead(bool nGetRead)
{
    Q_D(GetItems); 
    if (nGetRead != d->getRead) {
        d->getRead = nGetRead;
#ifdef QT_DEBUG
        qDebug() << "Changed getRead to" << d->getRead;
#endif
        Q_EMIT getReadChanged(getRead());
    }
}




bool GetItems::oldestFirst() const { Q_D(const GetItems); return d->oldestFirst; }

void GetItems::setOldestFirst(bool nOldestFirst)
{
    Q_D(GetItems); 
    if (nOldestFirst != d->oldestFirst) {
        d->oldestFirst = nOldestFirst;
#ifdef QT_DEBUG
        qDebug() << "Changed oldestFirst to" << d->oldestFirst;
#endif
        Q_EMIT oldestFirstChanged(oldestFirst());
    }
}




