/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016-2018 Hüssenbergnetz/Matthias Fehring
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

#include "getfolders_p.h"
#include "../error.h"
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>

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
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Still in operation. Returning.");
        return;
    }

    qDebug("%s", "Start requesting folders from the server.");

    setInOperation(true);

    sendRequest();
}


void GetFolders::successCallback()
{
    if (isUseStorageEnabled() && storage()) {
        storage()->foldersRequested(jsonResult());
    }

    setInOperation(false);

    qDebug("Successfully requested the folder list from the server.");

    Q_EMIT succeeded(jsonResult());
}


bool GetFolders::checkOutput()
{
    if (Q_LIKELY(Component::checkOutput())) {

        if (Q_UNLIKELY(!jsonResult().object().value(QStringLiteral("folders")).isArray())) {
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

#include "moc_getfolders.cpp"
