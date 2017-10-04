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

#include "getupdateditems_p.h"
#include "../error.h"
#include <QJsonObject>
#include <QJsonValue>
#include <QUrlQuery>
#include <QMetaEnum>

using namespace Fuoten;

GetUpdatedItems::GetUpdatedItems(QObject *parent) :
    Component(* new GetUpdatedItemsPrivate, parent)
{
}


GetUpdatedItems::GetUpdatedItems(const QDateTime &lastModified, FuotenEnums::Type type, qint64 parentId, QObject *parent) :
    Component(* new GetUpdatedItemsPrivate(lastModified, type, parentId), parent)
{
}


GetUpdatedItems::GetUpdatedItems(GetUpdatedItemsPrivate &dd, QObject *parent) :
    Component(dd, parent)
{
}






void GetUpdatedItems::execute()
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Still in operation. Returning.");
        return;
    }

    qDebug("%s", "Start requesting updated items from the server.");

    setInOperation(true);

    QUrlQuery uq;
    uq.addQueryItem(QStringLiteral("lastModified"), QString::number(lastModified().toTime_t()));
    uq.addQueryItem(QStringLiteral("type"), QString::number((int)type()));
    uq.addQueryItem(QStringLiteral("id"), QString::number(parentId()));

    setUrlQuery(uq);

    sendRequest();
}


void GetUpdatedItems::successCallback()
{
    if (storage()) {
        storage()->itemsRequested(jsonResult());
    }

    setInOperation(false);

    qDebug("%s", "Successfully requested updated items from the server.");

    Q_EMIT succeeded(jsonResult());
}



bool GetUpdatedItems::checkOutput()
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




bool GetUpdatedItems::checkInput()
{
    if (Q_LIKELY(Component::checkInput())) {

        if (Q_UNLIKELY(!lastModified().isValid())) {
            //% "Invalid last modified time."
            setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-error-invalid-time"), QString(), this));
            setInOperation(false);
            Q_EMIT failed(error());
            return false;
        }

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







QDateTime GetUpdatedItems::lastModified() const { Q_D(const GetUpdatedItems); return d->lastModified; }

void GetUpdatedItems::setLastModified(const QDateTime &nLastModified)
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Can not change property %s, still in operation.", "lastModified");
        return;
    }

    Q_D(GetUpdatedItems); 
    if (nLastModified != d->lastModified) {
        d->lastModified = nLastModified;
        qDebug("Changed lastModified to %s.", qUtf8Printable(d->lastModified.toString(Qt::ISODate)));
        Q_EMIT lastModifiedChanged(lastModified());
    }
}




FuotenEnums::Type GetUpdatedItems::type() const { Q_D(const GetUpdatedItems); return d->type; }

void GetUpdatedItems::setType(FuotenEnums::Type nType)
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Can not change property %s, still in operation.", "type");
        return;
    }

    Q_D(GetUpdatedItems); 
    if (nType != d->type) {
        d->type = nType;
        qDebug("Changed type to %s.", FuotenEnums::staticMetaObject.enumerator(FuotenEnums::staticMetaObject.indexOfEnumerator("Type")).valueToKey(d->type));
        Q_EMIT typeChanged(type());
    }
}




qint64 GetUpdatedItems::parentId() const { Q_D(const GetUpdatedItems); return d->parentId; }

void GetUpdatedItems::setParentId(qint64 nParentId)
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Can not change property %s, still in operation.", "parentId");
        return;
    }

    Q_D(GetUpdatedItems); 
    if (nParentId != d->parentId) {
        d->parentId = nParentId;
        qDebug("Changed parentId to %lli.", d->parentId);
        Q_EMIT parentIdChanged(parentId());
    }
}




