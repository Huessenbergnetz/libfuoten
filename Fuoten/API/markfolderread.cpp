/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "markfolderread_p.h"
#include "../error.h"
#include <QJsonObject>
#include <QJsonValue>

using namespace Fuoten;

MarkFolderReadPrivate::MarkFolderReadPrivate() :
    ComponentPrivate(),
    folderId(0),
    newestItemId(0)
{
    expectedJSONType = Component::Empty;
    namOperation = QNetworkAccessManager::PutOperation;
}


MarkFolderReadPrivate::~MarkFolderReadPrivate()
{

}


MarkFolderRead::MarkFolderRead(QObject *parent) :
    Component(* new MarkFolderReadPrivate, parent)
{
}


MarkFolderRead::MarkFolderRead(MarkFolderReadPrivate &dd, QObject *parent) :
    Component(dd, parent)
{
}


MarkFolderRead::~MarkFolderRead()
{

}


void MarkFolderRead::execute()
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Still in operation. Returning.");
        return;
    }

    qDebug("Start to mark all items in folder with ID %lli as read on the server. Newest item ID: %lli.", folderId(), newestItemId());

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
    if (Q_LIKELY(Component::checkInput())) {

        if (Q_UNLIKELY(folderId() <= 0)) {
            //% "The folder ID is not valid."
            setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-invalid-folder-id"), QString(), this));
            Q_EMIT failed(error());
            return false;
        }

        if (Q_UNLIKELY(newestItemId() <= 0)) {
            //% "The item ID is not valid."
            setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-invalid-item-id"), QString(), this));
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
    if (isUseStorageEnabled() && storage()) {
        storage()->folderMarkedRead(folderId(), newestItemId());
    }

    setInOperation(false);

    qDebug("Successfully marked the folder with ID %lli as read on the server. Newest item ID: %lli.", folderId(), newestItemId());

    Q_EMIT succeeded(folderId(), newestItemId());
}


void MarkFolderRead::extractError(QNetworkReply *reply)
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


qint64 MarkFolderRead::folderId() const { Q_D(const MarkFolderRead); return d->folderId; }

void MarkFolderRead::setFolderId(qint64 nFolderId)
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Can not change property %s, still in operation.", "folderId");
        return;
    }

    Q_D(MarkFolderRead);
    if (nFolderId != d->folderId) {
        d->folderId = nFolderId;
        qDebug("Changed folderId to %lli.", d->folderId);
        Q_EMIT folderIdChanged(folderId());
    }
}


qint64 MarkFolderRead::newestItemId() const { Q_D(const MarkFolderRead); return d->newestItemId; }

void MarkFolderRead::setNewestItemId(qint64 nNewestItemId)
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Can not change property %s, still in operation.", "newestItemId");
        return;
    }

    Q_D(MarkFolderRead);
    if (nNewestItemId != d->newestItemId) {
        d->newestItemId = nNewestItemId;
        qDebug("Changed newestItemId to %lli.", d->newestItemId);
        Q_EMIT newestItemIdChanged(newestItemId());
    }
}

#include "moc_markfolderread.cpp"
