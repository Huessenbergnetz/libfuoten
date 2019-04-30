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

#include "deletefolder_p.h"
#include "../error.h"

using namespace Fuoten;


DeleteFolder::DeleteFolder(QObject *parent) :
    Component(* new DeleteFolderPrivate, parent)
{
}


DeleteFolder::DeleteFolder(DeleteFolderPrivate &dd, QObject *parent) :
    Component(dd, parent)
{
}


void DeleteFolder::execute()
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Still in operation. Returning.");
        return;
    }

    qDebug("Start to delete folder with ID %lli.", folderId());

    setInOperation(true);

    setError(nullptr);

    QStringList rl(QStringLiteral("folders"));
    rl.append(QString::number(folderId()));

    setApiRoute(rl);

    sendRequest();
}


bool DeleteFolder::checkInput()
{
    if (Q_LIKELY(Component::checkInput())) {

        if (Q_UNLIKELY(folderId() <= 0)) {
            //% "The folder ID is not valid."
            setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-invalid-folder-id"), QString(), this));
            Q_EMIT failed(error());
            return false;
        }

    } else {
        return false;
    }

    return true;
}


void DeleteFolder::successCallback()
{
    if (isUseStorageEnabled() && storage()) {
        storage()->folderDeleted(folderId());
    }

    setInOperation(false);

    qDebug("Successfully deleted folder with ID %lli.", folderId());

    Q_EMIT succeeded(folderId());
}


void DeleteFolder::extractError(QNetworkReply *reply)
{
    Q_ASSERT_X(reply, "extract error", "invalid QNetworkReply");
    if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 404) {
        //% "The folder was not found on the server."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-folder-not-exists"), QString(), this));
    } else {
        setError(new Error(reply, this));
    }
    Q_EMIT failed(error());
}


qint64 DeleteFolder::folderId() const { Q_D(const DeleteFolder); return d->folderId; }

void DeleteFolder::setFolderId(qint64 nFolderId)
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Can not change property %s, still in operation.", "folderId");
        return;
    }

    Q_D(DeleteFolder);
    if (nFolderId != d->folderId) {
        d->folderId = nFolderId;
        qDebug("Changed folderId to %lli.", d->folderId);
        Q_EMIT folderIdChanged(folderId());
    }
}

#include "moc_deletefolder.cpp"
