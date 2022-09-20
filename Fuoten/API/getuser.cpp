/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "getuser_p.h"
#include "../error.h"
#include <QJsonObject>
#include <QJsonValue>

using namespace Fuoten;

GetUserPrivate::GetUserPrivate() :
    ComponentPrivate()
{

}


GetUserPrivate::~GetUserPrivate()
{

}


GetUser::GetUser(QObject *parent) :
    Component(*new GetUserPrivate, parent)
{
    setApiRoute(QStringLiteral("/user"));
    setExpectedJSONType(Component::Object);
}


GetUser::GetUser(GetUserPrivate &dd, QObject *parent) :
    Component(dd, parent)
{
    setApiRoute(QStringLiteral("/user"));
    setExpectedJSONType(Component::Object);
}


GetUser::~GetUser()
{

}


void GetUser::execute()
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Still in operation. Returning.");
        return;
    }

    qDebug("%s", "Start requesting user information from the server.");

    setInOperation(true);

    sendRequest();
}


void GetUser::successCallback()
{
    setInOperation(false);

    qDebug("%s", "Successfully requested user information from the server.");
    Q_EMIT succeeded(jsonResult());
}


bool GetUser::checkOutput()
{
    if (Q_LIKELY(Component::checkOutput())) {

        const QJsonObject o = jsonResult().object();

        if (Q_UNLIKELY(!o.contains(QStringLiteral("displayName")))) {
            //% "Can not find the user's display name in the server reply."
            setError(new Error(Error::OutputError, Error::Critical, qtTrId("err-displayname-not-found"), QString(), this));
            Q_EMIT failed(error());
            return false;
        }

    } else {
        return false;
    }

    return true;
}

#include "moc_getuser.cpp"
