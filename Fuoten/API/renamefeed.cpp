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

#include "renamefeed_p.h"
#include "../error.h"
#include <QJsonObject>
#ifdef QT_DEBUG
#include <QtDebug>
#endif

using namespace Fuoten;


RenameFeed::RenameFeed(QObject *parent) :
    Component(* new RenameFeedPrivate, parent)
{
}


RenameFeed::RenameFeed(RenameFeedPrivate &dd, QObject *parent) :
    Component(dd, parent)
{
}


qint64 RenameFeed::feedId() const { Q_D(const RenameFeed); return d->feedId; }

void RenameFeed::setFeedId(qint64 nFeedId)
{
    Q_D(RenameFeed); 
    if (nFeedId != d->feedId) {
        d->feedId = nFeedId;
#ifdef QT_DEBUG
        qDebug() << "Changed feedId to" << d->feedId;
#endif
        Q_EMIT feedIdChanged(feedId());
    }
}




QString RenameFeed::newName() const { Q_D(const RenameFeed); return d->newName; }

void RenameFeed::setNewName(const QString &nNewName)
{
    Q_D(RenameFeed); 
    if (nNewName != d->newName) {
        d->newName = nNewName;
#ifdef QT_DEBUG
        qDebug() << "Changed newName to" << d->newName;
#endif
        Q_EMIT newNameChanged(newName());
    }
}



void RenameFeed::execute()
{
    if (inOperation()) {
        qWarning("Still in operation. Returning.");
        return;
    }

    setInOperation(true);

    setError(nullptr);

#ifdef QT_DEBUG
    qDebug() << "Start renaming a feed on the server.";
#endif

    QStringList rl(QStringLiteral("feeds"));
    rl.append(QString::number(feedId()));
    rl.append(QStringLiteral("rename"));

    setApiRoute(rl);

    QJsonObject plo; // payload object
    plo.insert(QStringLiteral("feedTitle"), QJsonValue(newName()));

    setPayload(plo);

    sendRequest();
}




bool RenameFeed::checkInput()
{
    if (Component::checkInput()) {

        if (feedId() == 0) {
            //% "The feed ID is not valid."
            setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-invalid-feed-id"), QString(), this));
            setInOperation(false);
            Q_EMIT failed(error());
            return false;
        }

        if (newName().isEmpty()) {
            //% "The feed name can not be empty."
            setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-empty-feed-name"), QString(), this));
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



void RenameFeed::successCallback()
{
    if (storage()) {
        storage()->feedRenamed(feedId(), newName());
    }

    setInOperation(false);

#ifdef QT_DEBUG
    qDebug() << "Successfully renamed the feed on the server.";
#endif

    Q_EMIT succeeded(feedId(), newName());
}





void RenameFeed::extractError(QNetworkReply *reply)
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    switch(statusCode) {
    case 404:
        //% "The feed was not found on the server."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-feed-not-exists"), QString(), this));
        break;
    default:
        setError(new Error(reply, this));
        break;
    }
    setInOperation(false);
    Q_EMIT failed(error());
}
