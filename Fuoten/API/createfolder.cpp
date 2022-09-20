/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "createfolder_p.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include "../error.h"

using namespace Fuoten;


CreateFolderPrivate::CreateFolderPrivate() :
    ComponentPrivate()
{
    expectedJSONType = Component::Object;
    namOperation = QNetworkAccessManager::PostOperation;
    apiRoute = QStringLiteral("/folders");
}


CreateFolderPrivate::~CreateFolderPrivate()
{

}


CreateFolder::CreateFolder(QObject *parent) :
    Component(* new CreateFolderPrivate, parent)
{
}


CreateFolder::CreateFolder(CreateFolderPrivate &dd, QObject *parent) :
    Component(dd, parent)
{

}


CreateFolder::~CreateFolder()
{

}


void CreateFolder::execute()
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Still in operation. Returning.");
        return;
    }

    setInOperation(true);

    setError(nullptr);

    qDebug("Start creating folder \"%s\" on the server.", qUtf8Printable(name()));

    QJsonObject plo; // payload object
    plo.insert(QStringLiteral("name"), QJsonValue(name()));

    setPayload(plo);

    sendRequest();
}


bool CreateFolder::checkInput()
{
    if (Q_LIKELY(Component::checkInput())) {

        if (Q_UNLIKELY(name().isEmpty())) {
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


bool CreateFolder::checkOutput()
{
    if (Q_LIKELY(Component::checkOutput())) {

        if (Q_UNLIKELY(jsonResult().object().value(QStringLiteral("folders")).toArray().isEmpty())) {
            //% "The data the server replied does not contain a \"folders\" array."
            setError(new Error(Error::OutputError, Error::Critical, qtTrId("libfuoten-err-no-folders-array-in-reply"), QString(), this));
            Q_EMIT failed(error());
            return false;
        }


    } else {
        return false;
    }

    return true;
}


void CreateFolder::extractError(QNetworkReply *reply)
{
    const int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if (statusCode == 409) {
        //% "The folder name does already exist on the server."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-folder-name-exists"), QString(), this));
        Q_EMIT failed(error());
    } else if (statusCode == 422) {
        //% "The folder name is invalid (for instance empty)."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-folder-invalid-name"), QString(), this));
        Q_EMIT failed(error());
    } else {
        Component::extractError(reply);
    }
}


void CreateFolder::successCallback()
{
    if (isUseStorageEnabled() && storage()) {
        storage()->folderCreated(jsonResult());
    }

    setInOperation(false);

    qDebug("Successfully created the folder \"%s\" on the server.", qUtf8Printable(name()));

    Q_EMIT succeeded(jsonResult());
}


QString CreateFolder::name() const { Q_D(const CreateFolder); return d->name; }

void CreateFolder::setName(const QString &nName)
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Can not change property %s, still in operation.", "name");
        return;
    }

    Q_D(CreateFolder);
    if (nName.simplified() != d->name) {
        d->name = nName.simplified();
        qDebug("Changed name to \"%s\".", qUtf8Printable(d->name));
        Q_EMIT nameChanged(name());
    }
}

#include "moc_createfolder.cpp"
