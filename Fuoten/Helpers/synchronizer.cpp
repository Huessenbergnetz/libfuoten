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

#include "synchronizer_p.h"
#include "../article.h"
#include <QPair>
#ifdef QT_DEBUG
#include <QtDebug>
#endif

using namespace Fuoten;


Synchronizer::Synchronizer(QObject *parent) :
    QObject(parent), d_ptr(new SynchronizerPrivate(this))
{
}



Synchronizer::~Synchronizer()
{
}


void Synchronizer::sync()
{
    start();
}


void Synchronizer::start()
{
    Q_D(Synchronizer);

    if (d->inOperation) {
        qWarning("Still in operation. Returning.");
        return;
    }

    if (!d->configuration) {
        qWarning("No configuration set. Returning.");
        return;
    }

#ifdef QT_DEBUG
        qDebug() << "Start synchronizing";
#endif

    d->inOperation = true;
    Q_EMIT inOperationChanged(true);

    if (d->storage) {
        QueryArgs qa;
        qa.queuedOnly = true;

        const ArticleList qas = storage()->getArticles(qa);
        if (!qas.isEmpty()) {
            for (Article *a : qas) {
                if (a->queue().testFlag(FuotenEnums::MarkAsUnread)) {
                    d->queuedUnreadArticles.append(a->id());
                }
                if (a->queue().testFlag(FuotenEnums::MarkAsRead)) {
                    d->queuedReadArticles.append(a->id());
                }
                if (a->queue().testFlag(FuotenEnums::Star)) {
                    d->queuedStarredArticles.append(qMakePair(a->feedId(), a->guidHash()));
                }
                if (a->queue().testFlag(FuotenEnums::Unstar)) {
                    d->queuedUnstarredArticles.append(qMakePair(a->feedId(), a->guidHash()));
                }
            }

            qDeleteAll(qas);

            if (!d->queuedUnreadArticles.isEmpty()) {
                notifyAboutUnread();
            } else if (!d->queuedReadArticles.isEmpty()) {
                notifyAboutRead();
            } else if (!d->queuedStarredArticles.isEmpty()) {
                notifyAboutStarred();
            } else if (!d->queuedUnstarredArticles.isEmpty()) {
                notifyAboutUnstarred();
            } else {
                requestFolders();
            }

        } else {
            requestFolders();
        }
    } else {
        requestFolders();
    }
}





Error *Synchronizer::error() const { Q_D(const Synchronizer); return d->error; }

void Synchronizer::setError(Error *nError)
{
    Q_D(Synchronizer);
    if (nError != d->error) {
        Error *old = d->error;

        if (nError) {
            if (nError->parent() != this) {

                d->error = new Error(nError->type(), nError->severity(), nError->text(), nError->data(), this);

            } else {
                d->error = nError;
            }
        } else {
            d->error = nError;
        }

#ifdef QT_DEBUG
        qDebug() << "Changed error to" << d->error;
#endif
        Q_EMIT errorChanged(d->error);

        if (old) {
            if (old->parent() == this) {
                delete old;
            }
        }

        if (d->error && (d->error->type() != Error::NoError) && (d->error->severity() == Error::Critical || d->error->severity() == Error::Fatal)) {
            Q_EMIT inOperationChanged(false);
            Q_EMIT failed(d->error);
            d->cleanup();
        }
    }
}





void Synchronizer::notifyAboutUnread()
{
    Q_D(Synchronizer);
    if (!d->unreadMultipleItems) {
#ifdef QT_DEBUG
        qDebug() << "Notify the News App about unread items.";
#endif
        d->unreadMultipleItems = new MarkMultipleItems(this);
        d->unreadMultipleItems->setConfiguration(d->configuration);
        d->unreadMultipleItems->setItemIds(d->queuedUnreadArticles);
        d->unreadMultipleItems->setUnread(true);
        QObject::connect(d->unreadMultipleItems, &Component::failed, this, &Synchronizer::setError);
//        QObject::connect(unreadMultipleItems, &Component::failed, unreadMultipleItems, &QObject::deleteLater);
        if (!d->queuedReadArticles.isEmpty()) {
            QObject::connect(d->unreadMultipleItems, &MarkMultipleItems::succeeded, this, &Synchronizer::notifyAboutRead);
        } else if (!d->queuedStarredArticles.isEmpty()) {
            QObject::connect(d->unreadMultipleItems, &MarkMultipleItems::succeeded, this, &Synchronizer::notifyAboutStarred);
        } else if (!d->queuedUnstarredArticles.isEmpty()) {
            QObject::connect(d->unreadMultipleItems, &MarkMultipleItems::succeeded, this, &Synchronizer::notifyAboutUnstarred);
        } else {
            QObject::connect(d->unreadMultipleItems, &MarkMultipleItems::succeeded, this, &Synchronizer::requestFolders);
        }
//        QObject::connect(unreadMultipleItems, &MarkMultipleItems::succeeded, unreadMultipleItems, &QObject::deleteLater);
        d->unreadMultipleItems->execute();
    }
}


void Synchronizer::notifyAboutRead()
{
    Q_D(Synchronizer);
    if (!d->readMultipleItems) {
#ifdef QT_DEBUG
        qDebug() << "Notify the News App about read items.";
#endif
        d->readMultipleItems = new MarkMultipleItems(this);
        d->readMultipleItems->setConfiguration(d->configuration);
        d->readMultipleItems->setItemIds(d->queuedReadArticles);
        d->readMultipleItems->setUnread(false);
        QObject::connect(d->readMultipleItems, &Component::failed, this, &Synchronizer::setError);
//        QObject::connect(readMultipleItems, &Component::failed, readMultipleItems, &QObject::deleteLater);
        if (!d->queuedStarredArticles.isEmpty()) {
            QObject::connect(d->readMultipleItems, &MarkMultipleItems::succeeded, this, &Synchronizer::notifyAboutStarred);
        } else if (!d->queuedUnstarredArticles.isEmpty()) {
            QObject::connect(d->readMultipleItems, &MarkMultipleItems::succeeded, this, &Synchronizer::notifyAboutUnstarred);
        } else {
            QObject::connect(d->readMultipleItems, &MarkMultipleItems::succeeded, this, &Synchronizer::requestFolders);
        }
//        QObject::connect(readMultipleItems, &MarkMultipleItems::succeeded, readMultipleItems, &QObject::deleteLater);
        d->readMultipleItems->execute();
    }
}


void Synchronizer::notifyAboutStarred()
{
    Q_D(Synchronizer);
    if (!d->starMultipleItems) {
#ifdef QT_DEBUG
        qDebug() << "Notify the News App about starred items.";
#endif
        d->starMultipleItems = new StarMultipleItems(this);
        d->starMultipleItems->setConfiguration(d->configuration);
        d->starMultipleItems->setItemsToStar(d->queuedStarredArticles);
        d->starMultipleItems->setStarred(true);
        QObject::connect(d->starMultipleItems, &Component::failed, this, &Synchronizer::setError);
//        QObject::connect(starMultipleItems, &Component::failed, starMultipleItems, &QObject::deleteLater);
        if (!d->queuedUnstarredArticles.isEmpty()) {
            QObject::connect(d->starMultipleItems, &StarMultipleItems::succeeded, this, &Synchronizer::notifyAboutUnstarred);
        } else {
            QObject::connect(d->starMultipleItems, &StarMultipleItems::succeeded, this, &Synchronizer::requestFolders);
        }
//        QObject::connect(starMultipleItems, &StarMultipleItems::succeeded, starMultipleItems, &QObject::deleteLater);
        d->starMultipleItems->execute();
    }
}


void Synchronizer::notifyAboutUnstarred()
{
    Q_D(Synchronizer);
    if (!d->unstarMultipleItems) {
#ifdef QT_DEBUG
        qDebug() << "Notify the News App about unstarred items.";
#endif
        d->unstarMultipleItems = new StarMultipleItems(this);
        d->unstarMultipleItems->setConfiguration(d->configuration);
        d->unstarMultipleItems->setItemsToStar(d->queuedUnstarredArticles);
        d->unstarMultipleItems->setStarred(false);
        QObject::connect(d->unstarMultipleItems, &Component::failed, this, &Synchronizer::setError);
//        QObject::connect(unstarMultipleItems, &Component::failed, unstarMultipleItems, &QObject::deleteLater);
        QObject::connect(d->unstarMultipleItems, &StarMultipleItems::succeeded, this, &Synchronizer::requestFolders);
//        QObject::connect(unstarMultipleItems, &StarMultipleItems::succeeded, unstarMultipleItems, &QObject::deleteLater);
        d->unstarMultipleItems->execute();
    }
}


void Synchronizer::requestFolders()
{
    Q_D(Synchronizer);
    if (!d->getFolders) {
#ifdef QT_DEBUG
        qDebug() << "Getting folders";
#endif
        d->getFolders = new GetFolders(this);
        d->getFolders->setConfiguration(d->configuration);
        d->getFolders->setStorage(d->storage);
        QObject::connect(d->getFolders, &Component::failed, this, &Synchronizer::setError);
//        QObject::connect(getFolders, &Component::failed, getFolders, &QObject::deleteLater);
        if (d->storage) {
            QObject::connect(d->storage, &AbstractStorage::requestedFolders, this, &Synchronizer::requestFeeds);
//            QObject::connect(storage, &AbstractStorage::requestedFolders, getFolders, &QObject::deleteLater);
        } else {
            QObject::connect(d->getFolders, &Component::succeeded, this, &Synchronizer::requestFeeds);
//            QObject::connect(getFolders, &Component::succeeded, getFolders, &QObject::deleteLater);
        }
        d->getFolders->execute();
    }
}



void Synchronizer::requestFeeds()
{
    Q_D(Synchronizer);
    if (!d->getFeeds) {
#ifdef QT_DEBUG
        qDebug() << "Getting feeds";
        if (d->configuration->getLastSync().isValid()) {
            qDebug() << "We have a valid last sync time. Calling GetUpdatedItems after receiving feeds.";
        } else {
            qDebug() << "We have no valid last sync time. Calling GetItems after receiving feeds.";
        }
#endif
        d->getFeeds = new GetFeeds(this);
        d->getFeeds->setConfiguration(d->configuration);
        d->getFeeds->setStorage(d->storage);
        QObject::connect(d->getFeeds, &Component::failed, this, &Synchronizer::setError);
//        QObject::connect(getFeeds, &Component::failed, getFeeds, &QObject::deleteLater);
        if (d->storage) {
            if (d->configuration->getLastSync().isValid()) {
                QObject::connect(d->storage, &AbstractStorage::requestedFeeds, this, &Synchronizer::requestUpdated);
            } else {
                QObject::connect(d->storage, &AbstractStorage::requestedFeeds, this, &Synchronizer::requestUnread);
            }
//            QObject::connect(storage, &AbstractStorage::requestedFeeds, getFeeds, &QObject::deleteLater);
        } else {
            if (d->configuration->getLastSync().isValid()) {
                QObject::connect(d->getFeeds, &Component::succeeded, this, &Synchronizer::requestUpdated);
            } else {
                QObject::connect(d->getFeeds, &Component::succeeded, this, &Synchronizer::requestUnread);
            }
//            QObject::connect(getFeeds, &Component::succeeded, getFeeds, &QObject::deleteLater);
        }
        d->getFeeds->execute();
    }
}



void Synchronizer::requestUnread()
{
    Q_D(Synchronizer);
    if (!d->getUnread) {
#ifdef QT_DEBUG
        qDebug() << "Getting unread items";
#endif
        d->getUnread = new GetItems(this);
        d->getUnread->setConfiguration(d->configuration);
        d->getUnread->setStorage(d->storage);
        d->getUnread->setType(FuotenEnums::All);
        d->getUnread->setGetRead(false);
        d->getUnread->setBatchSize(-1);
        d->getUnread->setRequestTimeout(150);
        QObject::connect(d->getUnread, &Component::failed, this, &Synchronizer::setError);
//        QObject::connect(getUnread, &Component::failed, getUnread, &QObject::deleteLater);
        if (d->storage) {
            QObject::connect(d->storage, &AbstractStorage::requestedItems, this, &Synchronizer::requestStarred);
//            QObject::connect(storage, &AbstractStorage::requestedItems, getUnread, &QObject::deleteLater);
        } else {
            QObject::connect(d->getUnread, &Component::succeeded, this, &Synchronizer::requestStarred);
//            QObject::connect(getUnread, &Component::succeeded, getUnread, &QObject::deleteLater);
        }
        d->getUnread->execute();
    }
}


void Synchronizer::requestStarred()
{
    Q_D(Synchronizer);
    if (!d->getStarred) {
#ifdef QT_DEBUG
        qDebug() << "Getting starred items";
#endif
        d->getStarred = new GetItems(this);
        d->getStarred->setConfiguration(d->configuration);
        d->getStarred->setStorage(d->storage);
        d->getStarred->setType(FuotenEnums::Starred);
        d->getStarred->setGetRead(true);
        d->getStarred->setBatchSize(-1);
        QObject::connect(d->getStarred, &Component::failed, this, &Synchronizer::setError);
//        QObject::connect(getStarred, &Component::failed, getStarred, &QObject::deleteLater);
        if (d->storage) {
            QObject::connect(d->storage, &AbstractStorage::requestedItems, this, &Synchronizer::finished);
//            QObject::connect(storage, &AbstractStorage::requestedItems, getStarred, &QObject::deleteLater);
        } else {
            QObject::connect(d->getStarred, &Component::succeeded, this, &Synchronizer::finished);
//            QObject::connect(getStarred, &Component::succeeded, getStarred, &QObject::deleteLater);
        }
        d->getStarred->execute();
    }
}



void Synchronizer::requestUpdated()
{
    Q_D(Synchronizer);
    if (!d->getUpdated) {
#ifdef QT_DEBUG
        qDebug() << "Getting updated items";
#endif
        d->getUpdated = new GetUpdatedItems(this);
        d->getUpdated->setConfiguration(d->configuration);
        d->getUpdated->setStorage(d->storage);
        d->getUpdated->setLastModified(d->configuration->getLastSync());
        d->getUpdated->setType(FuotenEnums::All);
        d->getUpdated->setParentId(0);
        QObject::connect(d->getUpdated, &Component::failed, this, &Synchronizer::setError);
//        QObject::connect(getUpdated, &Component::failed, getUpdated, &QObject::deleteLater);
        if (d->storage) {
            QObject::connect(d->storage, &AbstractStorage::requestedItems, this, &Synchronizer::finished);
//            QObject::connect(storage, &AbstractStorage::requestedItems, getUpdated, &QObject::deleteLater);
        } else {
            QObject::connect(d->getUpdated, &Component::succeeded, this, &Synchronizer::finished);
//            QObject::connect(getUpdated, &Component::succeeded, getUpdated, &QObject::deleteLater);
        }
        d->getUpdated->execute();
    }
}



void Synchronizer::finished()
{
    Q_D(Synchronizer);
    d->inOperation = false;
    d->configuration->setLastSync(QDateTime::currentDateTimeUtc());
    Q_EMIT inOperationChanged(false);
    Q_EMIT succeeded();
    d->cleanup();

#ifdef QT_DEBUG
        qDebug() << "Finished synchronizing";
#endif
}



AbstractConfiguration *Synchronizer::configuration() const { Q_D(const Synchronizer); return d->configuration; }

void Synchronizer::setConfiguration(AbstractConfiguration *nAbstractConfiguration)
{
    Q_D(Synchronizer);
    if (nAbstractConfiguration != d->configuration) {
        d->configuration = nAbstractConfiguration;
#ifdef QT_DEBUG
        qDebug() << "Changed configuration to" << d->configuration;
#endif
        Q_EMIT configurationChanged(configuration());
    }
}




AbstractStorage *Synchronizer::storage() const { Q_D(const Synchronizer); return d->storage; }

void Synchronizer::setStorage(AbstractStorage *nStorageHandler)
{
    Q_D(Synchronizer);
    if (nStorageHandler != d->storage) {
        d->storage = nStorageHandler;
#ifdef QT_DEBUG
        qDebug() << "Changed storage to" << d->storage;
#endif
        Q_EMIT storageChanged(storage());
    }
}



bool Synchronizer::inOperation() const
{
    Q_D(const Synchronizer);
    return d->inOperation;
}
