/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "getfolders_p.h"
#include "../error.h"
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>

using namespace Fuoten;

GetFoldersPrivate::GetFoldersPrivate() :
    ComponentPrivate()
{

}


GetFoldersPrivate::~GetFoldersPrivate()
{

}


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


GetFolders::~GetFolders()
{

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
