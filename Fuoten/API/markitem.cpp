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

#include "markitem_p.h"
#include "../error.h"
#ifdef QT_DEBUG
#include <QtDebug>
#endif

using namespace Fuoten;

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





void MarkItem::execute()
{
    if (inOperation()) {
        qWarning("Still in operation. Returning.");
        return;
    }

#ifdef QT_DEBUG
    qDebug() << "Start to mark the item on server as" << (unread() ? "unread" : "read");
#endif

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
    if (Component::checkInput()) {

        if (itemId() <= 0) {
            //% "The article ID is not valid."
            setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-invalid-article-id"), QString(), this));
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


void MarkItem::successCallback()
{
    if (storage()) {
        storage()->itemMarked(itemId(), unread());
    }

    setInOperation(false);

#ifdef QT_DEBUG
    qDebug() << "Successfully marked the item on the server as" << (unread() ? "unread" : "read");;
#endif

    Q_EMIT succeeded(itemId(), unread());
}


void MarkItem::extractError(QNetworkReply *reply)
{
    if (reply) {
        if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 404) {
            //% "The article was not found on the server."
            setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-article-not-exists"), QString(), this));
        } else {
            setError(new Error(reply, this));
        }
    } else {
        qFatal("Invalid QNetworkReply!");
    }

    setInOperation(false);
    Q_EMIT failed(error());
}





qint64 MarkItem::itemId() const { Q_D(const MarkItem); return d->itemId; }

void MarkItem::setItemId(qint64 nItemId)
{
    Q_D(MarkItem); 
    if (nItemId != d->itemId) {
        d->itemId = nItemId;
#ifdef QT_DEBUG
        qDebug() << "Changed itemId to" << d->itemId;
#endif
        Q_EMIT itemIdChanged(itemId());
    }
}




bool MarkItem::unread() const { Q_D(const MarkItem); return d->unread; }

void MarkItem::setUnread(bool nUnread)
{
    Q_D(MarkItem); 
    if (nUnread != d->unread) {
        d->unread = nUnread;
#ifdef QT_DEBUG
        qDebug() << "Changed unread to" << d->unread;
#endif
        Q_EMIT unreadChanged(unread());
    }
}




