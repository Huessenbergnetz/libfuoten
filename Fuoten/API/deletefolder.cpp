/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "deletefolder_p.h"
#include "../error.h"

using namespace Fuoten;


DeleteFolderPrivate::DeleteFolderPrivate() :
    ComponentPrivate()
{
    expectedJSONType = Component::Empty;
    namOperation = QNetworkAccessManager::DeleteOperation;
}


DeleteFolderPrivate::~DeleteFolderPrivate()
{

}


DeleteFolder::DeleteFolder(QObject *parent) :
    Component(* new DeleteFolderPrivate, parent)
{
}


DeleteFolder::DeleteFolder(DeleteFolderPrivate &dd, QObject *parent) :
    Component(dd, parent)
{
}


DeleteFolder::~DeleteFolder()
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
        Q_EMIT failed(error());
    } else {
        Component::extractError(reply);
    }
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
