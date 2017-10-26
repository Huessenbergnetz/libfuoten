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

#include "createfolder_p.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include "../error.h"

using namespace Fuoten;


CreateFolder::CreateFolder(QObject *parent) :
    Component(* new CreateFolderPrivate, parent)
{
}


CreateFolder::CreateFolder(CreateFolderPrivate &dd, QObject *parent) :
    Component(dd, parent)
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
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    switch(statusCode) {
    case 409:
        //% "The folder name does already exist on the server."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-folder-name-exists"), QString(), this));
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
