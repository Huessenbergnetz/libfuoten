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

#include "abstractstorage_p.h"
#include "../article.h"
#include "../Helpers/abstractconfiguration.h"
#include "../API/component.h"
#include <QRegularExpression>

using namespace Fuoten;

AbstractStorage::AbstractStorage(QObject *parent) :
    QObject(parent), d_ptr(new AbstractStoragePrivate)
{
    qRegisterMetaType<Fuoten::IdList>("IdList");
    qRegisterMetaType<Fuoten::ArticleList>("ArticleList");
}

AbstractStorage::AbstractStorage(AbstractStoragePrivate &dd, QObject *parent) :
    QObject(parent), d_ptr(&dd)
{
    qRegisterMetaType<Fuoten::IdList>("IdList");
    qRegisterMetaType<Fuoten::ArticleList>("ArticleList");
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



quint16 AbstractStorage::totalUnread() const { Q_D(const AbstractStorage); return d->totalUnread; }

void AbstractStorage::setTotalUnread(quint16 nTotalUnread)
{
    Q_D(AbstractStorage);
    if (nTotalUnread != d->totalUnread) {
        d->totalUnread = nTotalUnread;
        qDebug("Changed totalUnread to %i.", d->totalUnread);
        Q_EMIT totalUnreadChanged(totalUnread());
    }
}




quint16 AbstractStorage::starred() const { Q_D(const AbstractStorage); return d->starred; }

void AbstractStorage::setStarred(quint16 nStarred)
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
    const ArticleList articles = getArticles(args);

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



void AbstractStorage::clearQueue()
{

}
