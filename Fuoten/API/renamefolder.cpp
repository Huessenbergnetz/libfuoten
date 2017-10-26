/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016-2017 Matthias Fehring
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

#include "renamefolder_p.h"
#include <QJsonObject>
#include <QJsonValue>
#include "../error.h"

using namespace Fuoten;


RenameFolder::RenameFolder(QObject *parent) :
    Component(* new RenameFolderPrivate, parent)
{
}


RenameFolder::RenameFolder(RenameFolderPrivate &dd, QObject *parent) :
    Component(dd, parent)
{
}


void RenameFolder::execute()
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Still in operation. Returning.");
        return;
    }

    setInOperation(true);

    setError(nullptr);

    qDebug("Start to rename folder with ID %lli to \"%s\".", folderId(), qUtf8Printable(newName()));

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
    if (Q_LIKELY(Component::checkInput())) {

        if (Q_UNLIKELY(folderId() == 0)) {
            //% "The folder ID is not valid."
            setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-invalid-folder-id"), QString(), this));
            Q_EMIT failed(error());
            return false;
        }

        if (Q_UNLIKELY(newName().isEmpty())) {
            //% "The folder name can not be empty."
            setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-empty-folder-name"), QString(), this));
            Q_EMIT failed(error());
            return false;
        }

    } else {
        return false;
    }

    return true;
}


void RenameFolder::successCallback()
{
    if (isUseStorageEnabled() && storage()) {
        storage()->folderRenamed(folderId(), newName());
    }

    setInOperation(false);

    qDebug("Successfully renamed folder with ID %lli to \"%s\".", folderId(), qUtf8Printable(newName()));

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

    Q_EMIT failed(error());
}


qint64 RenameFolder::folderId() const { Q_D(const RenameFolder); return d->folderId; }

void RenameFolder::setFolderId(qint64 nFolderId)
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Can not change property %s, still in operation.", "folderId");
        return;
    }

    Q_D(RenameFolder);
    if (nFolderId != d->folderId) {
        d->folderId = nFolderId;
        qDebug("Changed folderId to %lli.", d->folderId);
        Q_EMIT folderIdChanged(folderId());
    }
}


QString RenameFolder::newName() const { Q_D(const RenameFolder); return d->newName; }

void RenameFolder::setNewName(const QString &nNewName)
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Can not change property %s, still in operation.", "newName");
        return;
    }

    Q_D(RenameFolder);
    if (nNewName.simplified() != d->newName) {
        d->newName = nNewName.simplified();
        qDebug("Changed newName to \"%s\".", qUtf8Printable(d->newName));
        Q_EMIT newNameChanged(newName());
    }
}

#include "moc_renamefolder.cpp"
