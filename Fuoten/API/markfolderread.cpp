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

#include "markfolderread_p.h"
#include "../error.h"
#include <QJsonObject>
#include <QJsonValue>
#ifdef QT_DEBUG
#include <QtDebug>
#endif

using namespace Fuoten;


MarkFolderRead::MarkFolderRead(QObject *parent) :
    Component(* new MarkFolderReadPrivate, parent)
{
}



MarkFolderRead::MarkFolderRead(MarkFolderReadPrivate &dd, QObject *parent) :
    Component(dd, parent)
{
}


void MarkFolderRead::execute()
{
    if (inOperation()) {
        qWarning("Still in operation. Returning.");
        return;
    }

#ifdef QT_DEBUG
    qDebug() << "Start to mark all items in folder as read on server.";
#endif

    setInOperation(true);

    setError(nullptr);

    QStringList rl(QStringLiteral("folders"));
    rl.append(QString::number(folderId()));
    rl.append(QStringLiteral("read"));
    setApiRoute(rl);

    QJsonObject plo; // payload object
    plo.insert(QStringLiteral("newestItemId"), QJsonValue(newestItemId()));

    setPayload(plo);

    sendRequest();
}



bool MarkFolderRead::checkInput()
{
    if (Component::checkInput()) {

        if (folderId() <= 0) {
            //% "The folder ID is not valid."
            setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-invalid-folder-id"), QString(), this));
            setInOperation(false);
            Q_EMIT failed(error());
            return false;
        }

        if (newestItemId() <= 0) {
            //% "The item ID is not valid."
            setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-invalid-item-id"), QString(), this));
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


void MarkFolderRead::successCallback()
{
    if (storage()) {
        storage()->folderMarkedRead(folderId(), newestItemId());
    }

    setInOperation(false);

#ifdef QT_DEBUG
    qDebug() << "Successfully marked the folder as read on the server.";
#endif

    Q_EMIT succeeded(folderId(), newestItemId());
}


void MarkFolderRead::extractError(QNetworkReply *reply)
{
    if (reply) {
        if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 404) {
            //% "The folder was not found on the server."
            setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-folder-not-exists"), QString(), this));
        } else {
            setError(new Error(reply, this));
        }
    } else {
        qFatal("Invalid QNetworkReply!");
    }

    setInOperation(false);
    Q_EMIT failed(error());
}



qint64 MarkFolderRead::folderId() const { Q_D(const MarkFolderRead); return d->folderId; }

void MarkFolderRead::setFolderId(qint64 nFolderId)
{
    if (!inOperation()) {
        Q_D(MarkFolderRead);
        if (nFolderId != d->folderId) {
            d->folderId = nFolderId;
#ifdef QT_DEBUG
            qDebug() << "Changed folderId to" << d->folderId;
#endif
            Q_EMIT folderIdChanged(folderId());
        }
    }
}




qint64 MarkFolderRead::newestItemId() const { Q_D(const MarkFolderRead); return d->newestItemId; }

void MarkFolderRead::setNewestItemId(qint64 nNewestItemId)
{
    if (!inOperation()) {
        Q_D(MarkFolderRead);
        if (nNewestItemId != d->newestItemId) {
            d->newestItemId = nNewestItemId;
#ifdef QT_DEBUG
            qDebug() << "Changed newestItemId to" << d->newestItemId;
#endif
            Q_EMIT newestItemIdChanged(newestItemId());
        }
    }
}




