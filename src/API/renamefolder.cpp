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

#include "renamefolder_p.h"
#include <QJsonObject>
#include <QJsonValue>
#include "../error.h"
#ifdef QT_DEBUG
#include <QtDebug>
#endif

using namespace Fuoten;


RenameFolder::RenameFolder(QObject *parent) :
    Component(* new RenameFolderPrivate, parent)
{
//    setExpectedJSONType(Component::Empty);
//    setNetworkOperation(QNetworkAccessManager::PutOperation);
}



RenameFolder::RenameFolder(RenameFolderPrivate &dd, QObject *parent) :
    Component(dd, parent)
{
//    setExpectedJSONType(Component::Empty);
//    setNetworkOperation(QNetworkAccessManager::PutOperation);
}



void RenameFolder::execute()
{
    if (inOperation()) {
        qWarning("Still in operation. Returning.");
        return;
    }

    setInOperation(true);

    setError(nullptr);

#ifdef QT_DEBUG
    qDebug() << "Start renaming a folder on the server.";
#endif

    QStringList rl(QStringLiteral("folders"));
    rl.append(QString::number(folderId()));

    setApiRoute(rl);

    QJsonObject plo; // payload object
    plo.insert(QStringLiteral("name"), QJsonValue(newName()));

    setPayload(plo);

    sendRequest();
}


bool RenameFolder::checkInput()
{
    if (Component::checkInput()) {

        if (folderId() == 0) {
            //% "The folder ID is not valid."
            setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-invalid-folder-id"), QString(), this));
            setInOperation(false);
            Q_EMIT failed(error());
            return false;
        }

        if (newName().isEmpty()) {
            //% "The folder name can not be empty."
            setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-empty-folder-name"), QString(), this));
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



void RenameFolder::successCallback()
{
    if (storage()) {
        storage()->folderRenamed(folderId(), newName());
    }

    setInOperation(false);

#ifdef QT_DEBUG
    qDebug() << "Successfully renamed the folder on the server.";
#endif

    Q_EMIT succeeded(folderId(), newName());
}





void RenameFolder::extractError(QNetworkReply *reply)
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    switch(statusCode) {
    case 409:
        //% "The folder name does already exist on the server."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-folder-name-exists"), QString(), this));
        break;
    case 404:
        //% "The folder was not found on the server."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-folder-not-exists"), QString(), this));
        break;
    case 422:
        //% "The folder name is invalid (for instance empty)."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-folder-invalid-name"), QString(), this));
        break;
    default:
        setError(new Error(reply, this));
        break;
    }
    setInOperation(false);
    Q_EMIT failed(error());
}


qint64 RenameFolder::folderId() const { Q_D(const RenameFolder); return d->folderId; }

void RenameFolder::setFolderId(qint64 nFolderId)
{
    Q_D(RenameFolder); 
    if (nFolderId != d->folderId) {
        d->folderId = nFolderId;
#ifdef QT_DEBUG
        qDebug() << "Changed folderId to" << d->folderId;
#endif
        Q_EMIT folderIdChanged(folderId());
    }
}




QString RenameFolder::newName() const { Q_D(const RenameFolder); return d->newName; }

void RenameFolder::setNewName(const QString &nNewName)
{
    Q_D(RenameFolder);
    if (nNewName.simplified() != d->newName) {
        d->newName = nNewName.simplified();
#ifdef QT_DEBUG
        qDebug() << "Changed newName to" << d->newName;
#endif
        Q_EMIT newNameChanged(newName());
    }
}




