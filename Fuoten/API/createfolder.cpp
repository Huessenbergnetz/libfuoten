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

#include "createfolder_p.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include "../error.h"
#ifdef QT_DEBUG
#include <QtDebug>
#endif

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
    if (inOperation()) {
        qWarning("Still in operation. Returning.");
        return;
    }

    setInOperation(true);

    setError(nullptr);

#ifdef QT_DEBUG
    qDebug() << "Start creating a folder on the server.";
#endif

    QJsonObject plo; // payload object
    plo.insert(QStringLiteral("name"), QJsonValue(name()));

    setPayload(plo);

    sendRequest();
}



bool CreateFolder::checkInput()
{
    if (Component::checkInput()) {

        if (name().isEmpty()) {
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



bool CreateFolder::checkOutput()
{
    if (Component::checkOutput()) {

        if (jsonResult().object().value(QStringLiteral("folders")).toArray().isEmpty()) {
            //% "The data the server replied does not contain a \"folders\" array."
            setError(new Error(Error::OutputError, Error::Critical, qtTrId("libfuoten-err-no-folders-array-in-reply"), QString(), this));
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
    setInOperation(false);
    Q_EMIT failed(error());
}



void CreateFolder::successCallback()
{
    if (storage()) {
        storage()->folderCreated(jsonResult());
    }

    setInOperation(false);

#ifdef QT_DEBUG
    qDebug() << "Successfully created the foler on the server.";
#endif

    Q_EMIT succeeded(jsonResult());
}



QString CreateFolder::name() const { Q_D(const CreateFolder); return d->name; }

void CreateFolder::setName(const QString &nName)
{
    if (inOperation()) {
        qWarning("Can not change property %s, still in operation.", "name");
        return;
    }

    Q_D(CreateFolder); 
    if (nName.simplified() != d->name) {
        d->name = nName.simplified();
#ifdef QT_DEBUG
        qDebug() << "Changed name to" << d->name;
#endif
        Q_EMIT nameChanged(name());
    }
}




