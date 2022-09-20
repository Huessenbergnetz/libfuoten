/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "createfeed_p.h"
#include "../error.h"
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

using namespace Fuoten;


CreateFeedPrivate::CreateFeedPrivate() :
    ComponentPrivate()
{
    expectedJSONType = Component::Object;
    namOperation = QNetworkAccessManager::PostOperation;
    apiRoute = QStringLiteral("/feeds");
}


CreateFeedPrivate::~CreateFeedPrivate()
{

}


CreateFeed::CreateFeed(QObject *parent) :
    Component(* new CreateFeedPrivate, parent)
{
}


CreateFeed::CreateFeed(CreateFeedPrivate &dd, QObject *parent) :
    Component(dd, parent)
{

}


CreateFeed::~CreateFeed()
{

}


void CreateFeed::execute()
{
    if (inOperation()) {
        qWarning("Still in operation. Returning.");
        return;
    }

    qDebug("%s", "Start to create a feed on the server.");

    setInOperation(true);

    setError(nullptr);

    QJsonObject plo; // payload object
    plo.insert(QStringLiteral("url"), QJsonValue(url().toString()));
    plo.insert(QStringLiteral("folderId"), QJsonValue(folderId()));

    setPayload(plo);

    sendRequest();
}


bool CreateFeed::checkInput()
{
    if (Q_LIKELY(Component::checkInput())) {

        if (Q_UNLIKELY(!url().isValid())) {
            //% "The URL of the feed is not valid."
            setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-invalid-feed-url"), url().toString(), this));
            Q_EMIT failed(error());
            return false;
        }

        if (Q_UNLIKELY(folderId() < 0)) {
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


bool CreateFeed::checkOutput()
{
    if (Q_LIKELY(Component::checkOutput())) {

        if (Q_UNLIKELY(jsonResult().object().value(QStringLiteral("feeds")).toArray().isEmpty())) {
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


void CreateFeed::extractError(QNetworkReply *reply)
{
    const int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if (statusCode == 409) {
        //% "The feed does already exist on the server."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-feed-exists"), QString(), this));
        Q_EMIT failed(error());
    } else if (statusCode == 422) {
        //% "The feed can not be read (most likely contains errors)."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-feed-unreadable"), QString(), this));
        Q_EMIT failed(error());
    } else {
        Component::extractError(reply);
    }
}


void CreateFeed::successCallback()
{
    if (isUseStorageEnabled() && storage()) {
        storage()->feedCreated(jsonResult());
    }

    setInOperation(false);

    qDebug("%s", "Successfully create the feed on the server.");

    Q_EMIT succeeded(jsonResult());
}


QUrl CreateFeed::url() const { Q_D(const CreateFeed); return d->url; }

void CreateFeed::setUrl(const QUrl &nUrl)
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Can not change property %s, still in operation.", "url");
        return;
    }

    Q_D(CreateFeed);
    if (nUrl != d->url) {
        d->url = nUrl;
        qDebug("Changed URL to %s.", d->url.toString().toLocal8Bit().constData());
        Q_EMIT urlChanged(url());
    }
}


qint64 CreateFeed::folderId() const { Q_D(const CreateFeed); return d->folderId; }

void CreateFeed::setFolderId(qint64 nFolderId)
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Can not change property %s, still in operation.", "folderId");
        return;
    }

    Q_D(CreateFeed);
    if (nFolderId != d->folderId) {
        d->folderId = nFolderId;
        qDebug("Changed folderId to %lli.", d->folderId);
        Q_EMIT folderIdChanged(folderId());
    }
}

#include "moc_createfeed.cpp"
