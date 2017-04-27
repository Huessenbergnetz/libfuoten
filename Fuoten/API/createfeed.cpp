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

#include "createfeed_p.h"
#include "../error.h"
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#ifdef QT_DEBUG
#include <QtDebug>
#endif

using namespace Fuoten;


CreateFeed::CreateFeed(QObject *parent) :
    Component(* new CreateFeedPrivate, parent)
{
}



CreateFeed::CreateFeed(CreateFeedPrivate &dd, QObject *parent) :
    Component(dd, parent)
{

}





void CreateFeed::execute()
{
    if (inOperation()) {
        qWarning("Still in operation. Returning.");
        return;
    }

#ifdef QT_DEBUG
    qDebug() << "Start to create a feed on the server.";
#endif

    setInOperation(true);

    setError(nullptr);

    QJsonObject plo; // payload object
    plo.insert(QStringLiteral("url"), QJsonValue(url().toString()));
    plo.insert(QStringLiteral("folderId"), QJsonValue(folderId()));

    setPayload(plo);

    sendRequest();
}


bool CreateFeed::checkInput()
{
    if (Q_LIKELY(Component::checkInput())) {

        if (Q_UNLIKELY(!url().isValid())) {
            //% "The URL of the feed is not valid."
            setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-invalid-feed-url"), url().toString(), this));
            setInOperation(false);
            Q_EMIT failed(error());
            return false;
        }

        if (Q_UNLIKELY(folderId() < 0)) {
            //% "The folder ID is not valid."
            setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-invalid-folder-id"), QString(), this));
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



bool CreateFeed::checkOutput()
{
    if (Q_LIKELY(Component::checkOutput())) {

        if (Q_UNLIKELY(jsonResult().object().value(QStringLiteral("feeds")).toArray().isEmpty())) {
            //% "The data the server replied does not contain a \"feeds\" array."
            setError(new Error(Error::OutputError, Error::Critical, qtTrId("libfuoten-err-no-feeds-array-in-reply"), QString(), this));
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



void CreateFeed::extractError(QNetworkReply *reply)
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    switch(statusCode) {
    case 409:
        //% "The feed does already exist on the server."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-feed-exists"), QString(), this));
        break;
    case 422:
        //% "The feed can not be read (most likely contains errors)."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-feed-unreadable"), QString(), this));
        break;
    default:
        setError(new Error(reply, this));
        break;
    }
    setInOperation(false);
    Q_EMIT failed(error());
}



void CreateFeed::successCallback()
{
    if (storage()) {
        storage()->feedCreated(jsonResult());
    }

    setInOperation(false);

#ifdef QT_DEBUG
    qDebug() << "Successfully create the feed on the server.";
#endif

    Q_EMIT succeeded(jsonResult());
}



QUrl CreateFeed::url() const { Q_D(const CreateFeed); return d->url; }

void CreateFeed::setUrl(const QUrl &nUrl)
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Can not change property %s, still in operation.", "url");
        return;
    }

    Q_D(CreateFeed); 
    if (nUrl != d->url) {
        d->url = nUrl;
#ifdef QT_DEBUG
        qDebug() << "Changed url to" << d->url;
#endif
        Q_EMIT urlChanged(url());
    }
}




qint64 CreateFeed::folderId() const { Q_D(const CreateFeed); return d->folderId; }

void CreateFeed::setFolderId(qint64 nFolderId)
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Can not change property %s, still in operation.", "folderId");
        return;
    }

    Q_D(CreateFeed); 
    if (nFolderId != d->folderId) {
        d->folderId = nFolderId;
#ifdef QT_DEBUG
        qDebug() << "Changed folderId to" << d->folderId;
#endif
        Q_EMIT folderIdChanged(folderId());
    }
}




