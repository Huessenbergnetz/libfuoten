/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "getfeeds_p.h"
#include "../error.h"
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>

using namespace Fuoten;

GetFeedsPrivate::GetFeedsPrivate() :
    ComponentPrivate()
{
    apiRoute = QStringLiteral("/feeds");
    expectedJSONType = Component::Object;
}


GetFeedsPrivate::~GetFeedsPrivate()
{

}


GetFeeds::GetFeeds(QObject *parent) :
    Component(* new GetFeedsPrivate, parent)
{

}


GetFeeds::GetFeeds(GetFeedsPrivate &dd, QObject *parent) :
    Component(dd, parent)
{

}


GetFeeds::~GetFeeds()
{

}


void GetFeeds::execute()
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Still in operation. Returning.");
        return;
    }

    qDebug("%s", "Start requesting feeds from the server.");

    setInOperation(true);

    sendRequest();
}


void GetFeeds::successCallback()
{
    if (isUseStorageEnabled() && storage()) {
        storage()->feedsRequested(jsonResult());
    }

    setInOperation(false);

    qDebug("Successfully requested the feed list from the server.");

    Q_EMIT succeeded(jsonResult());
}


bool GetFeeds::checkOutput()
{
    if (Q_LIKELY(Component::checkOutput())) {

        if (Q_UNLIKELY(!jsonResult().object().value(QStringLiteral("feeds")).isArray())) {
            //% "The data the server replied does not contain a \"feeds\" array."
            setError(new Error(Error::OutputError, Error::Critical, qtTrId("libfuoten-err-no-feeds-array-in-reply"), QString(), this));
            Q_EMIT failed(error());
            return false;
        }


    } else {
        return false;
    }

    return true;
}

#include "moc_getfeeds.cpp"
