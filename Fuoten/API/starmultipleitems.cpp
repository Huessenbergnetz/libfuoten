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

#include "starmultipleitems_p.h"
#include "../error.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#ifdef QT_DEBUG
#include <QtDebug>
#endif

using namespace Fuoten;

StarMultipleItems::StarMultipleItems(QObject *parent) :
    Component(* new StarMultipleItemsPrivate, parent)
{
}


StarMultipleItems::StarMultipleItems(bool starred, QObject *parent) :
    Component(* new StarMultipleItemsPrivate(starred), parent)
{
}


StarMultipleItems::StarMultipleItems(StarMultipleItemsPrivate &dd, QObject *parent) :
    Component(dd, parent)
{
}






void StarMultipleItems::execute()
{
    if (inOperation()) {
        qWarning("Still in operation. Returning.");
        return;
    }

#ifdef QT_DEBUG
    qDebug() << "Start to" << (starred() ? "star" : "unstar") << "the items on the remove server.";
#endif

    setInOperation(true);

    setError(nullptr);

    if (starred()) {
        setApiRoute(QStringLiteral("/items/star/multiple"));
    } else {
        setApiRoute(QStringLiteral("/items/unstar/multiple"));
    }

    QJsonArray arr;
    if (!itemsToStar().isEmpty()) {
        for (const QPair<qint64,QString> &p : itemsToStar()) {
            QJsonObject o;
            o.insert(QStringLiteral("feedId"), QJsonValue(p.first));
            o.insert(QStringLiteral("guidHash"), QJsonValue(p.second));
            arr.append(QJsonValue(o));
        }
    }

    QJsonObject plo; // payload object
    plo.insert(QStringLiteral("items"), QJsonValue(arr));

    setPayload(plo);

    sendRequest();
}



bool StarMultipleItems::checkInput()
{
    if (Component::checkInput()) {

        if (itemsToStar().isEmpty()) {
            //% "The list of articles to star/unstar can not be empty."
            setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-articles-star-unstar-list"), QString(), this));
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


void StarMultipleItems::successCallback()
{
    if (storage()) {
        storage()->itemsStarred(itemsToStar(), starred());
    }

    setInOperation(false);

#ifdef QT_DEBUG
    qDebug() << "Successfully" << (starred() ? "starred" : "unstarred") << "the items on the remote server.";
#endif

    Q_EMIT succeeded(itemsToStar(), starred());
}


void StarMultipleItems::extractError(QNetworkReply *reply)
{
    if (reply) {
        setError(new Error(reply, this));
    } else {
        qFatal("Invalid QNetworkReply!");
    }

    setInOperation(false);
    Q_EMIT failed(error());
}





QList<QPair<qint64,QString>> StarMultipleItems::itemsToStar() const { Q_D(const StarMultipleItems); return d->itemsToStar; }

void StarMultipleItems::setItemsToStar(const QList<QPair<qint64, QString> > &items)
{
    Q_D(StarMultipleItems);
    d->itemsToStar = items;
}


void StarMultipleItems::addItem(qint64 feedId, const QString &guidHash)
{
    Q_D(StarMultipleItems);
    d->itemsToStar.append(qMakePair(feedId, guidHash));
}



bool StarMultipleItems::starred() const { Q_D(const StarMultipleItems); return d->starred; }

void StarMultipleItems::setStarred(bool nStarred)
{
    Q_D(StarMultipleItems); 
    if (nStarred != d->starred) {
        d->starred = nStarred;
#ifdef QT_DEBUG
        qDebug() << "Changed starred to" << d->starred;
#endif
        Q_EMIT starredChanged(starred());
    }
}




