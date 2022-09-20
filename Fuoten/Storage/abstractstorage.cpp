/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "abstractstorage_p.h"
#include "../article.h"
#include "../Helpers/abstractconfiguration.h"
#include "../API/component.h"
#include <QRegularExpression>
#include <QJsonValue>
#include <cmath>

using namespace Fuoten;

AbstractStoragePrivate::AbstractStoragePrivate()
{

}

AbstractStoragePrivate::~AbstractStoragePrivate()
{

}

AbstractStorage::AbstractStorage(QObject *parent) :
    QObject(parent), d_ptr(new AbstractStoragePrivate)
{
}

AbstractStorage::AbstractStorage(AbstractStoragePrivate &dd, QObject *parent) :
    QObject(parent), d_ptr(&dd)
{
}

AbstractStorage::~AbstractStorage()
{

}


bool AbstractStorage::ready() const { Q_D(const AbstractStorage); return d->ready; }

void AbstractStorage::setReady(bool nReady)
{
    Q_D(AbstractStorage);
    if (nReady != d->ready) {
        d->ready = nReady;
        qDebug("Changed ready to %s.", d->ready ? "true" : "false");
        Q_EMIT readyChanged(ready());
    }
}

Error *AbstractStorage::error() const { Q_D(const AbstractStorage); return d->error; }

void AbstractStorage::setError(Error *nError)
{
    Q_D(AbstractStorage);
    if (nError != d->error) {
        Error *old = d->error;

        if (nError->parent() == this) {
            d->error = nError;
        } else if ((nError->parent() == nullptr) && (nError->thread() == this->thread())) {
            d->error = nError;
            d->error->setParent(this);
        } else if ((nError->parent() == nullptr) && (nError->thread() != this->thread())) {
            nError->moveToThread(this->thread());
            if (nError->thread() == this->thread()) {
                nError->setParent(this);
                d->error = nError;
            } else {
                d->error = new Error(nError->type(), nError->severity(), nError->text(), nError->data(), this);
                qWarning("Failed to move Error object to this thread. Creating deep copy.");
            }
        } else {
            d->error = new Error(nError->type(), nError->severity(), nError->text(), nError->data(), this);
        }
        qDebug("Changed error to %p.", d->error);
        Q_EMIT errorChanged(error());

        if (old && old->parent() == this) {
            delete old;
        }
    }
}


int AbstractStorage::totalUnread() const { Q_D(const AbstractStorage); return d->totalUnread; }

void AbstractStorage::setTotalUnread(int nTotalUnread)
{
    Q_D(AbstractStorage);
    if (nTotalUnread != d->totalUnread) {
        d->totalUnread = nTotalUnread;
        qDebug("Changed totalUnread to %i.", d->totalUnread);
        Q_EMIT totalUnreadChanged(totalUnread());
    }
}


int AbstractStorage::starred() const { Q_D(const AbstractStorage); return d->starred; }

void AbstractStorage::setStarred(int nStarred)
{
    Q_D(AbstractStorage);
    if (nStarred != d->starred) {
        d->starred = nStarred;
        qDebug("Changed starred to %i.", d->starred);
        Q_EMIT starredChanged(starred());
    }
}


QString AbstractStorage::limitBody(const QString &body, int limit) const
{
    if (body.isEmpty() || body.length() < limit) {
        return body;
    }

    QString s = body.left(2*limit);
    s.remove(QRegularExpression(QStringLiteral("<[^>]*>")));
    s = s.simplified();
    return s.left(limit);
}


void AbstractStorage::getArticlesAsync(const QueryArgs &args)
{
    const auto articles = getArticles(args);

    Q_EMIT gotArticlesAsync(articles);
}


bool AbstractStorage::enqueueItem(FuotenEnums::QueueAction action, Article *article)
{
    Q_UNUSED(action)
    Q_UNUSED(article)
    return false;
}


bool AbstractStorage::enqueueMarkFeedRead(qint64 feedId, qint64 newestItemId)
{
    Q_UNUSED(feedId)
    Q_UNUSED(newestItemId)
    return false;
}


bool AbstractStorage::enqueueMarkFolderRead(qint64 folderId, qint64 newestItemId)
{
    Q_UNUSED(folderId)
    Q_UNUSED(newestItemId)
    return false;
}


bool AbstractStorage::enqueueMarkAllItemsRead()
{
    return false;
}


bool AbstractStorage::inOperation() const { Q_D(const AbstractStorage); return d->inOperation; }

void AbstractStorage::setInOperation(bool nInOperation)
{
    Q_D(AbstractStorage);
    if (nInOperation != d->inOperation) {
        d->inOperation = nInOperation;
        qDebug("Changed inOperation to %s.", d->inOperation ? "true" : "false");
        Q_EMIT inOperationChanged(inOperation());
    }
}


AbstractConfiguration *AbstractStorage::configuration() const
{
    Q_D(const AbstractStorage);
    AbstractConfiguration *_config = d->configuration;
    if (!_config) {
        _config = Component::defaultConfiguration();
    }
    return _config;
}

void AbstractStorage::setConfiguration(AbstractConfiguration *nConfiguration)
{
    Q_D(AbstractStorage);
    if (nConfiguration != d->configuration) {
        d->configuration = nConfiguration;
        qDebug("Changed configuration to %p.", d->configuration);
        Q_EMIT configurationChanged(configuration());
    }
}


AbstractNotificator *AbstractStorage::notificator() const
{
    Q_D(const AbstractStorage);
    AbstractNotificator *_notificator = d->notificator;
    if (!_notificator) {
        _notificator = Component::defaultNotificator();
    }
    return _notificator;
}

void AbstractStorage::setNotificator(AbstractNotificator *notificator)
{
    Q_D(AbstractStorage);
    if (notificator != d->notificator) {
        d->notificator = notificator;
        qDebug("Changed notificator to %p.", d->notificator);
        Q_EMIT notificatorChanged(d->notificator);
    }
}


void AbstractStorage::clearQueue()
{

}


void AbstractStorage::clearStorage()
{

}


void AbstractStorage::notify(AbstractNotificator::Type type, QtMsgType severity, const QVariant &data) const
{
    Q_D(const AbstractStorage);
    if (d->notificator) {
        d->notificator->notify(type, severity, data);
    }
}


void AbstractStorage::notify(const Fuoten::Error *e) const
{
    Q_D(const AbstractStorage);
    if (d->notificator) {
        d->notificator->notify(e);
    }
}

qint64 AbstractStorage::getIdFromJson(const QJsonValue &value)
{
    if (value.type() == QJsonValue::String) {
        return value.toString().toLongLong();
    } else if (value.type() == QJsonValue::Double) {
        return std::llround(value.toDouble());
    } else if (value.type() == QJsonValue::Null) {
        return 0;
    } else {
        qCritical("Can not convert JSON value to integer ID that is not of type String, Double or Null. Detected type: %i", value.type());
        Q_ASSERT_X(false, "get id from json", "not supported json type to convert id to qint64");
        return 0;
    }
}

#include "moc_abstractstorage.cpp"
