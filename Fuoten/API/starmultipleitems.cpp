/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016-2019 Hüssenbergnetz/Matthias Fehring
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

#include "starmultipleitems_p.h"
#include "../error.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

using namespace Fuoten;

StarMultipleItemsPrivate::StarMultipleItemsPrivate() : ComponentPrivate()
{
    expectedJSONType = Component::Empty;
    namOperation = QNetworkAccessManager::PutOperation;
}


StarMultipleItemsPrivate::StarMultipleItemsPrivate(bool nStarred) :
    ComponentPrivate(),
    starred(nStarred)
{
    expectedJSONType = Component::Empty;
    namOperation = QNetworkAccessManager::PutOperation;
}


StarMultipleItemsPrivate::~StarMultipleItemsPrivate()
{

}


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


StarMultipleItems::~StarMultipleItems()
{
}


void StarMultipleItems::execute()
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Still in operation. Returning.");
        return;
    }

    qDebug("Start to %s multiple items on the remote server.", starred() ? "start" : "unstar");

    setInOperation(true);

    setError(nullptr);

    if (starred()) {
        setApiRoute(QStringLiteral("/items/star/multiple"));
    } else {
        setApiRoute(QStringLiteral("/items/unstar/multiple"));
    }

    QJsonArray arr;
    if (!itemsToStar().isEmpty()) {
        const QList<QPair<qint64,QString>> its = itemsToStar();
        for (const QPair<qint64,QString> &p : its) {
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
    if (Q_LIKELY(Component::checkInput())) {

        if (Q_UNLIKELY(itemsToStar().isEmpty())) {
            //% "The list of articles to star/unstar can not be empty."
            setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-articles-star-unstar-list"), QString(), this));
            Q_EMIT failed(error());
            return false;
        }

    } else {
        return false;
    }

    return true;
}


void StarMultipleItems::successCallback()
{
    if (isUseStorageEnabled() && storage()) {
        storage()->itemsStarred(itemsToStar(), starred());
    }

    setInOperation(false);

    qDebug("Successfully %s multiple items on the remote server.", starred() ? "starred" : "unstarred");

    Q_EMIT succeeded(itemsToStar(), starred());
}


QList<QPair<qint64,QString>> StarMultipleItems::itemsToStar() const { Q_D(const StarMultipleItems); return d->itemsToStar; }

void StarMultipleItems::setItemsToStar(const QList<QPair<qint64, QString> > &items)
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Can not set items, still in operation.");
        return;
    }

    qDebug("Set %i items to star/unstar.", items.size());

    Q_D(StarMultipleItems);
    d->itemsToStar = items;
}


void StarMultipleItems::addItem(qint64 feedId, const QString &guidHash)
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Can not add the item, still in operation.");
        return;
    }

    qDebug("Add item to star/unstar. Feed ID %lli, GUID: %s", feedId, qUtf8Printable(guidHash));

    Q_D(StarMultipleItems);
    d->itemsToStar.append(qMakePair(feedId, guidHash));
}


bool StarMultipleItems::starred() const { Q_D(const StarMultipleItems); return d->starred; }

void StarMultipleItems::setStarred(bool nStarred)
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Can not change property %s, still in operation.", "starred");
        return;
    }

    Q_D(StarMultipleItems); 
    if (nStarred != d->starred) {
        d->starred = nStarred;
        qDebug("Changed starred to %s.", d->starred ? "true" : "false");
        Q_EMIT starredChanged(starred());
    }
}

#include "moc_starmultipleitems.cpp"
