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


void SynchronizerPrivate::setError(Error *nError)
{
    Q_Q(Synchronizer);
    if (nError != error) {
        Error *old = error;

        if (nError) {
            if (nError->parent() != q_ptr) {

                error = new Error(nError->type(), nError->severity(), nError->text(), nError->data(), q_ptr);

            } else {
                error = nError;
            }
        } else {
            error = nError;
        }

#ifdef QT_DEBUG
        qDebug() << "Changed error to" << error;
#endif
        Q_EMIT q->errorChanged(error);

        if (old) {
            if (old->parent() == q_ptr) {
                delete old;
            }
        }

        if (error && (error->type() != Error::NoError) && (error->severity() == Error::Critical || error->severity() == Error::Fatal)) {
            if (getFolders) {
                delete getFolders;
                getFolders = nullptr;
            }
            if (getFeeds) {
                delete getFeeds;
                getFeeds = nullptr;
            }
            if (getStarred) {
                delete getStarred;
                getStarred = nullptr;
            }
            if (getUnread) {
                delete getUnread;
                getUnread = nullptr;
            }
            if (getUpdated) {
                delete getUpdated;
                getUpdated = nullptr;
            }
            if (starMultipleItems) {
                delete starMultipleItems;
                starMultipleItems = nullptr;
            }
            if (unstarMultipleItems) {
                delete unstarMultipleItems;
                unstarMultipleItems = nullptr;
            }
            if (readMultipleItems) {
                delete readMultipleItems;
                readMultipleItems = nullptr;
            }
            if (unreadMultipleItems) {
                delete unreadMultipleItems;
                unreadMultipleItems = nullptr;
            }
            queuedUnreadArticles.clear();
            queuedReadArticles.clear();
            queuedStarredArticles.clear();
            queuedUnstarredArticles.clear();
            inOperation = false;
            Q_EMIT q->inOperationChanged(false);
            Q_EMIT q->failed(error);
        }
    }
}



void SynchronizerPrivate::start()
{
    Q_Q(Synchronizer);
    inOperation = true;
    Q_EMIT q->inOperationChanged(true);

    QueryArgs qa;
    qa.queuedOnly = true;

    const ArticleList qas = storage->getArticles(qa);
    if (!qas.isEmpty()) {
        for (Article *a : qas) {
            if (a->queue().testFlag(FuotenEnums::MarkAsUnread)) {
                queuedUnreadArticles.append(a->id());
            }
            if (a->queue().testFlag(FuotenEnums::MarkAsRead)) {
                queuedReadArticles.append(a->id());
            }
            if (a->queue().testFlag(FuotenEnums::Star)) {
                queuedStarredArticles.append(qMakePair(a->feedId(), a->guidHash()));
            }
            if (a->queue().testFlag(FuotenEnums::Unstar)) {
                queuedUnstarredArticles.append(qMakePair(a->feedId(), a->guidHash()));
            }
        }

        if (!queuedUnreadArticles.isEmpty()) {
            notifyAboutUnread();
        } else if (!queuedReadArticles.isEmpty()) {
            notifyAboutRead();
        } else if (!queuedStarredArticles.isEmpty()) {
            notifyAboutStarred();
        } else if (!queuedUnstarredArticles.isEmpty()) {
            notifyAboutUnstarred();
        } else {
            requestFolders();
        }

    } else {
        requestFolders();
    }
}



void SynchronizerPrivate::notifyAboutUnread()
{
    if (!unreadMultipleItems) {
#ifdef QT_DEBUG
        qDebug() << "Notify the News App about unread items.";
#endif
        unreadMultipleItems = new MarkMultipleItems(q_ptr);
        unreadMultipleItems->setConfiguration(configuration);
        unreadMultipleItems->setItemIds(queuedUnreadArticles);
        unreadMultipleItems->setUnread(true);
        QObject::connect(unreadMultipleItems, &Component::failed, [=] (Error *e) {setError(e);});
//        QObject::connect(unreadMultipleItems, &Component::failed, unreadMultipleItems, &QObject::deleteLater);
        QObject::connect(unreadMultipleItems, &MarkMultipleItems::succeeded, [=] () {
            if (!queuedReadArticles.isEmpty()) {
                notifyAboutRead();
            } else if (!queuedStarredArticles.isEmpty()) {
                notifyAboutStarred();
            } else if (!queuedUnstarredArticles.isEmpty()) {
                notifyAboutUnstarred();
            } else {
                requestFolders();
            }
        });
//        QObject::connect(unreadMultipleItems, &MarkMultipleItems::succeeded, unreadMultipleItems, &QObject::deleteLater);
        unreadMultipleItems->execute();
    }
}


void SynchronizerPrivate::notifyAboutRead()
{
    if (!readMultipleItems) {
#ifdef QT_DEBUG
        qDebug() << "Notify the News App about read items.";
#endif
        readMultipleItems = new MarkMultipleItems(q_ptr);
        readMultipleItems->setConfiguration(configuration);
        readMultipleItems->setItemIds(queuedReadArticles);
        readMultipleItems->setUnread(false);
        QObject::connect(readMultipleItems, &Component::failed, [=] (Error *e) {setError(e);});
//        QObject::connect(readMultipleItems, &Component::failed, readMultipleItems, &QObject::deleteLater);
        QObject::connect(readMultipleItems, &MarkMultipleItems::succeeded, [=] () {
            if (!queuedStarredArticles.isEmpty()) {
                notifyAboutStarred();
            } else if (!queuedUnstarredArticles.isEmpty()) {
                notifyAboutUnstarred();
            } else {
                requestFolders();
            }
        });
//        QObject::connect(readMultipleItems, &MarkMultipleItems::succeeded, readMultipleItems, &QObject::deleteLater);
        readMultipleItems->execute();
    }
}


void SynchronizerPrivate::notifyAboutStarred()
{
    if (!starMultipleItems) {
#ifdef QT_DEBUG
        qDebug() << "Notify the News App about starred items.";
#endif
        starMultipleItems = new StarMultipleItems(q_ptr);
        starMultipleItems->setConfiguration(configuration);
        starMultipleItems->setItemsToStar(queuedStarredArticles);
        starMultipleItems->setStarred(true);
        QObject::connect(starMultipleItems, &Component::failed, [=] (Error *e) {setError(e);});
//        QObject::connect(starMultipleItems, &Component::failed, starMultipleItems, &QObject::deleteLater);
        QObject::connect(starMultipleItems, &StarMultipleItems::succeeded, [=] () {
            if (!queuedUnstarredArticles.isEmpty()) {
                notifyAboutUnstarred();
            } else {
                requestFolders();
            }
        });
//        QObject::connect(starMultipleItems, &StarMultipleItems::succeeded, starMultipleItems, &QObject::deleteLater);
        starMultipleItems->execute();
    }
}


void SynchronizerPrivate::notifyAboutUnstarred()
{
    if (!unstarMultipleItems) {
#ifdef QT_DEBUG
        qDebug() << "Notify the News App about unstarred items.";
#endif
        unstarMultipleItems = new StarMultipleItems(q_ptr);
        unstarMultipleItems->setConfiguration(configuration);
        unstarMultipleItems->setItemsToStar(queuedUnstarredArticles);
        unstarMultipleItems->setStarred(false);
        QObject::connect(unstarMultipleItems, &Component::failed, [=] (Error *e) {setError(e);});
//        QObject::connect(unstarMultipleItems, &Component::failed, unstarMultipleItems, &QObject::deleteLater);
        QObject::connect(unstarMultipleItems, &StarMultipleItems::succeeded, [=] () {
            requestFolders();
        });
//        QObject::connect(unstarMultipleItems, &StarMultipleItems::succeeded, unstarMultipleItems, &QObject::deleteLater);
        unstarMultipleItems->execute();
    }
}


void SynchronizerPrivate::requestFolders()
{
    if (!getFolders) {
#ifdef QT_DEBUG
        qDebug() << "Getting folders";
#endif
        getFolders = new GetFolders(q_ptr);
        getFolders->setConfiguration(configuration);
        getFolders->setStorage(storage);
        QObject::connect(getFolders, &Component::failed, [=] (Error *e) {setError(e);});
//        QObject::connect(getFolders, &Component::failed, getFolders, &QObject::deleteLater);
        if (storage) {
            QObject::connect(storage, &AbstractStorage::requestedFolders, [=] () {requestFeeds();});
//            QObject::connect(storage, &AbstractStorage::requestedFolders, getFolders, &QObject::deleteLater);
        } else {
            QObject::connect(getFolders, &Component::succeeded, [=] () {requestFeeds();});
//            QObject::connect(getFolders, &Component::succeeded, getFolders, &QObject::deleteLater);
        }
        getFolders->execute();
    }
}



void SynchronizerPrivate::requestFeeds()
{
    if (!getFeeds) {
#ifdef QT_DEBUG
        qDebug() << "Getting feeds";
        if (configuration->getLastSync().isValid()) {
            qDebug() << "We have a valid last sync time. Calling GetUpdatedItems after receiving feeds.";
        } else {
            qDebug() << "We have no valid last sync time. Calling GetItems after receiving feeds.";
        }
#endif
        getFeeds = new GetFeeds(q_ptr);
        getFeeds->setConfiguration(configuration);
        getFeeds->setStorage(storage);
        QObject::connect(getFeeds, &Component::failed, [=] (Error *e) {setError(e);});
//        QObject::connect(getFeeds, &Component::failed, getFeeds, &QObject::deleteLater);
        if (storage) {
            if (configuration->getLastSync().isValid()) {
                QObject::connect(storage, &AbstractStorage::requestedFeeds, [=] () {requestUpdated();});
            } else {
                QObject::connect(storage, &AbstractStorage::requestedFeeds, [=] () {requestUnread();});
            }
//            QObject::connect(storage, &AbstractStorage::requestedFeeds, getFeeds, &QObject::deleteLater);
        } else {
            if (configuration->getLastSync().isValid()) {
                QObject::connect(getFeeds, &Component::succeeded, [=] () {requestUpdated();});
            } else {
                QObject::connect(getFeeds, &Component::succeeded, [=] () {requestUnread();});
            }
//            QObject::connect(getFeeds, &Component::succeeded, getFeeds, &QObject::deleteLater);
        }
        getFeeds->execute();
    }
}



void SynchronizerPrivate::requestUnread()
{
    if (!getUnread) {
#ifdef QT_DEBUG
        qDebug() << "Getting unread items";
#endif
        getUnread = new GetItems(q_ptr);
        getUnread->setConfiguration(configuration);
        getUnread->setStorage(storage);
        getUnread->setType(FuotenEnums::All);
        getUnread->setGetRead(false);
        getUnread->setBatchSize(-1);
        QObject::connect(getUnread, &Component::failed, [=] (Error *e) {setError(e);});
//        QObject::connect(getUnread, &Component::failed, getUnread, &QObject::deleteLater);
        if (storage) {
            QObject::connect(storage, &AbstractStorage::requestedItems, [=] () {requestStarred();});
//            QObject::connect(storage, &AbstractStorage::requestedItems, getUnread, &QObject::deleteLater);
        } else {
            QObject::connect(getUnread, &Component::succeeded, [=] () {requestStarred();});
//            QObject::connect(getUnread, &Component::succeeded, getUnread, &QObject::deleteLater);
        }
        getUnread->execute();
    }
}


void SynchronizerPrivate::requestStarred()
{
    if (!getStarred) {
#ifdef QT_DEBUG
        qDebug() << "Getting starred items";
#endif
        getStarred = new GetItems(q_ptr);
        getStarred->setConfiguration(configuration);
        getStarred->setStorage(storage);
        getStarred->setType(FuotenEnums::Starred);
        getStarred->setGetRead(true);
        getStarred->setBatchSize(-1);
        QObject::connect(getStarred, &Component::failed, [=] (Error *e) {setError(e);});
//        QObject::connect(getStarred, &Component::failed, getStarred, &QObject::deleteLater);
        if (storage) {
            QObject::connect(storage, &AbstractStorage::requestedItems, [=] () {finished();});
//            QObject::connect(storage, &AbstractStorage::requestedItems, getStarred, &QObject::deleteLater);
        } else {
            QObject::connect(getStarred, &Component::succeeded, [=] () {finished();});
//            QObject::connect(getStarred, &Component::succeeded, getStarred, &QObject::deleteLater);
        }
        getStarred->execute();
    }
}



void SynchronizerPrivate::requestUpdated()
{
    if (!getUpdated) {
#ifdef QT_DEBUG
        qDebug() << "Getting updated items";
#endif
        getUpdated = new GetUpdatedItems(q_ptr);
        getUpdated->setConfiguration(configuration);
        getUpdated->setStorage(storage);
        getUpdated->setLastModified(configuration->getLastSync());
        getUpdated->setType(FuotenEnums::All);
        getUpdated->setParentId(0);
        QObject::connect(getUpdated, &Component::failed, [=] (Error *e) {setError(e);});
//        QObject::connect(getUpdated, &Component::failed, getUpdated, &QObject::deleteLater);
        if (storage) {
            QObject::connect(storage, &AbstractStorage::requestedItems, [=] () {finished();});
//            QObject::connect(storage, &AbstractStorage::requestedItems, getUpdated, &QObject::deleteLater);
        } else {
            QObject::connect(getUpdated, &Component::succeeded, [=] () {finished();});
//            QObject::connect(getUpdated, &Component::succeeded, getUpdated, &QObject::deleteLater);
        }
        getUpdated->execute();
    }
}



void SynchronizerPrivate::finished()
{
    Q_Q(Synchronizer);
    inOperation = false;
    configuration->setLastSync(QDateTime::currentDateTimeUtc());
    Q_EMIT q->inOperationChanged(false);
    Q_EMIT q->succeeded();
    if (getFolders) {
        delete getFolders;
        getFolders = nullptr;
    }
    if (getFeeds) {
        delete getFeeds;
        getFeeds = nullptr;
    }
    if (getStarred) {
        delete getStarred;
        getStarred = nullptr;
    }
    if (getUnread) {
        delete getUnread;
        getUnread = nullptr;
    }
    if (getUpdated) {
        delete getUpdated;
        getUpdated = nullptr;
    }
    if (starMultipleItems) {
        delete starMultipleItems;
        starMultipleItems = nullptr;
    }
    if (unstarMultipleItems) {
        delete unstarMultipleItems;
        unstarMultipleItems = nullptr;
    }
    if (readMultipleItems) {
        delete readMultipleItems;
        readMultipleItems = nullptr;
    }
    if (unreadMultipleItems) {
        delete unreadMultipleItems;
        unreadMultipleItems = nullptr;
    }
    queuedUnreadArticles.clear();
    queuedReadArticles.clear();
    queuedStarredArticles.clear();
    queuedUnstarredArticles.clear();

#ifdef QT_DEBUG
        qDebug() << "Finished synchronizing";
#endif
}






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

#ifdef QT_DEBUG
        qDebug() << "Start synchronizing";
#endif


    d->start();
}


Error *Synchronizer::error() const { Q_D(const Synchronizer); return d->error; }



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
