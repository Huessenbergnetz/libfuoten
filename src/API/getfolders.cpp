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

#include "getfolders_p.h"
#include "../error.h"
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#ifdef QT_DEBUG
#include <QtDebug>
#endif

using namespace Fuoten;

GetFolders::GetFolders(QObject *parent) :
    Component(* new GetFoldersPrivate, parent)
{
    setApiRoute(QStringLiteral("/folders"));
    setExpectedJSONType(Component::Object);
}


GetFolders::GetFolders(GetFoldersPrivate &dd, QObject *parent) :
    Component(dd, parent)
{
    setApiRoute(QStringLiteral("/folders"));
    setExpectedJSONType(Component::Object);
}



void GetFolders::execute()
{
    if (inOperation()) {
        qWarning("Still in operation. Returning.");
        return;
    }

#ifdef QT_DEBUG
    qDebug() << "Start requesting folders from the server.";
#endif

    setInOperation(true);

    sendRequest();
}


void GetFolders::successCallback()
{
    if (storage()) {
        storage()->foldersRequested(jsonResult());
    }

    setInOperation(false);

#ifdef QT_DEBUG
    qDebug() << "Successfully requested the folder list from the server.";
#endif

    Q_EMIT succeeded(jsonResult());
}



void GetFolders::extractError(QNetworkReply *reply)
{
    setError(new Error(reply, this));
    setInOperation(false);
    Q_EMIT failed(error());
}



bool GetFolders::checkOutput()
{
    if (Component::checkOutput()) {

        if (!jsonResult().object().value(QStringLiteral("folders")).isArray()) {
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
