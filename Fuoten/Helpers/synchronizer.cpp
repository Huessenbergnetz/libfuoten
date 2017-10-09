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
#include "../API/component.h"
#include <QPair>

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

void Synchronizer::deferredSync(quint32 miliseconds)
{
    Q_D(Synchronizer);
    if (!d->deferTimer) {
        d->deferTimer = new QTimer(this);
        d->deferTimer->setSingleShot(true);
        d->deferTimer->setTimerType(Qt::VeryCoarseTimer);
        connect(d->deferTimer, &QTimer::timeout, this, &Synchronizer::start);
    }
    d->deferTimer->start(miliseconds);
}


void Synchronizer::start()
{
    Q_D(Synchronizer);

    if (Q_UNLIKELY(d->inOperation)) {
        qWarning("Still in operation. Returning.");
        return;
    }

    d->setInOperation(true);

    if (!d->configuration) {
        setConfiguration(Component::defaultConfiguration());
    }

    Q_ASSERT_X(d->configuration, "start synchronizing", "invalid configuration object");

    if (!d->storage) {
        setStorage(Component::defaultStorage());
    }

    qDebug("%s", "Start synchronizing.");

    setError(nullptr);

    d->totalActions = d->configuration->getLastSync().isValid() ? 4 : 5;

    if (d->storage) {
        QueryArgs qa;
        qa.queuedOnly = true;

        qDebug("%s", "Requesting queued articles from storage.");
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

            if (!d->queuedUnreadArticles.empty()) {
                qDebug("Found %i articles queued as unread.", d->queuedUnreadArticles.size());
                d->totalActions++;
            }

            if (!d->queuedReadArticles.empty()) {
                qDebug("Found %i articles queued as read.", d->queuedReadArticles.size());
                d->totalActions++;
            }

            if (!d->queuedStarredArticles.empty()) {
                qDebug("Found %i articles queued as starred.", d->queuedStarredArticles.size());
                d->totalActions++;
            }

            if (!d->queuedUnstarredArticles.empty()) {
                qDebug("Found %i articles queue as unstarred.", d->queuedUnstarredArticles.size());
                d->totalActions++;
            }

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

        qDebug("Changed error to %p.", d->error);
        Q_EMIT errorChanged(d->error);

        if (old) {
            if (old->parent() == this) {
                delete old;
            }
        }

        if (d->error && (d->error->type() != Error::NoError) && (d->error->severity() == Error::Critical || d->error->severity() == Error::Fatal)) {
            d->setInOperation(false);
            Q_EMIT failed(d->error);
            d->cleanup();
        }
    }
}


void Synchronizer::notifyAboutUnread()
{
    Q_D(Synchronizer);
    if (!d->unreadMultipleItems) {
        setProgress(++d->performedActions/d->totalActions);
        //% "Synchronizing unread articles"
        setCurrentAction(qtTrId("libfuoten-sync-unread-articles"));

        d->unreadMultipleItems = new MarkMultipleItems(this);
        d->unreadMultipleItems->setConfiguration(d->configuration);
        d->unreadMultipleItems->setItemIds(d->queuedUnreadArticles);
        d->unreadMultipleItems->setUnread(true);
        QObject::connect(d->unreadMultipleItems, &Component::failed, this, &Synchronizer::setError);
        if (!d->queuedReadArticles.isEmpty()) {
            QObject::connect(d->unreadMultipleItems, &MarkMultipleItems::succeeded, this, &Synchronizer::notifyAboutRead);
        } else if (!d->queuedStarredArticles.isEmpty()) {
            QObject::connect(d->unreadMultipleItems, &MarkMultipleItems::succeeded, this, &Synchronizer::notifyAboutStarred);
        } else if (!d->queuedUnstarredArticles.isEmpty()) {
            QObject::connect(d->unreadMultipleItems, &MarkMultipleItems::succeeded, this, &Synchronizer::notifyAboutUnstarred);
        } else {
            QObject::connect(d->unreadMultipleItems, &MarkMultipleItems::succeeded, this, &Synchronizer::requestFolders);
        }
        d->unreadMultipleItems->execute();
    }
}


void Synchronizer::notifyAboutRead()
{
    Q_D(Synchronizer);
    if (!d->readMultipleItems) {
        setProgress(++d->performedActions/d->totalActions);
        //% "Synchronizing read articles"
        setCurrentAction(qtTrId("libfuoten-sync-read-articles"));

        d->readMultipleItems = new MarkMultipleItems(this);
        d->readMultipleItems->setConfiguration(d->configuration);
        d->readMultipleItems->setItemIds(d->queuedReadArticles);
        d->readMultipleItems->setUnread(false);
        QObject::connect(d->readMultipleItems, &Component::failed, this, &Synchronizer::setError);
        if (!d->queuedStarredArticles.isEmpty()) {
            QObject::connect(d->readMultipleItems, &MarkMultipleItems::succeeded, this, &Synchronizer::notifyAboutStarred);
        } else if (!d->queuedUnstarredArticles.isEmpty()) {
            QObject::connect(d->readMultipleItems, &MarkMultipleItems::succeeded, this, &Synchronizer::notifyAboutUnstarred);
        } else {
            QObject::connect(d->readMultipleItems, &MarkMultipleItems::succeeded, this, &Synchronizer::requestFolders);
        }
        d->readMultipleItems->execute();
    }
}


void Synchronizer::notifyAboutStarred()
{
    Q_D(Synchronizer);
    if (!d->starMultipleItems) {
        setProgress(++d->performedActions/d->totalActions);
        //% "Synchronizing starred articles"
        setCurrentAction(qtTrId("libfuoten-sync-starred-articles"));

        d->starMultipleItems = new StarMultipleItems(this);
        d->starMultipleItems->setConfiguration(d->configuration);
        d->starMultipleItems->setItemsToStar(d->queuedStarredArticles);
        d->starMultipleItems->setStarred(true);
        QObject::connect(d->starMultipleItems, &Component::failed, this, &Synchronizer::setError);
        if (!d->queuedUnstarredArticles.isEmpty()) {
            QObject::connect(d->starMultipleItems, &StarMultipleItems::succeeded, this, &Synchronizer::notifyAboutUnstarred);
        } else {
            QObject::connect(d->starMultipleItems, &StarMultipleItems::succeeded, this, &Synchronizer::requestFolders);
        }
        d->starMultipleItems->execute();
    }
}


void Synchronizer::notifyAboutUnstarred()
{
    Q_D(Synchronizer);
    if (!d->unstarMultipleItems) {
        setProgress(++d->performedActions/d->totalActions);
        //% "Synchronizing unstarred articles"
        setCurrentAction(qtTrId("libfuoten-sync-unstarred-articles"));

        d->unstarMultipleItems = new StarMultipleItems(this);
        d->unstarMultipleItems->setConfiguration(d->configuration);
        d->unstarMultipleItems->setItemsToStar(d->queuedUnstarredArticles);
        d->unstarMultipleItems->setStarred(false);
        QObject::connect(d->unstarMultipleItems, &Component::failed, this, &Synchronizer::setError);
        QObject::connect(d->unstarMultipleItems, &StarMultipleItems::succeeded, this, &Synchronizer::requestFolders);
        d->unstarMultipleItems->execute();
    }
}


void Synchronizer::requestFolders()
{
    Q_D(Synchronizer);
    if (!d->getFolders) {
        setProgress(++d->performedActions/d->totalActions);
        //% "Requesting folders"
        setCurrentAction(qtTrId("libfuoten-sync-folders"));

        d->getFolders = new GetFolders(this);
        d->getFolders->setConfiguration(d->configuration);
        d->getFolders->setStorage(d->storage);
        QObject::connect(d->getFolders, &Component::failed, this, &Synchronizer::setError);
        if (d->storage) {
            QObject::connect(d->storage, &AbstractStorage::requestedFolders, this, &Synchronizer::requestFeeds);
        } else {
            QObject::connect(d->getFolders, &Component::succeeded, this, &Synchronizer::requestFeeds);
        }
        d->getFolders->execute();
    }
}


void Synchronizer::requestFeeds()
{
    Q_D(Synchronizer);
    if (!d->getFeeds) {
        setProgress(++d->performedActions/d->totalActions);
        //% "Requesting feeds"
        setCurrentAction(qtTrId("libfuoten-sync-feeds"));

#ifndef QT_NO_DEBUG_OUTPUT
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
        if (d->storage) {
            if (d->configuration->getLastSync().isValid()) {
                QObject::connect(d->storage, &AbstractStorage::requestedFeeds, this, &Synchronizer::requestUpdated);
            } else {
                QObject::connect(d->storage, &AbstractStorage::requestedFeeds, this, &Synchronizer::requestUnread);
            }
        } else {
            if (d->configuration->getLastSync().isValid()) {
                QObject::connect(d->getFeeds, &Component::succeeded, this, &Synchronizer::requestUpdated);
            } else {
                QObject::connect(d->getFeeds, &Component::succeeded, this, &Synchronizer::requestUnread);
            }
        }
        d->getFeeds->execute();
    }
}


void Synchronizer::requestUnread()
{
    Q_D(Synchronizer);
    if (!d->getUnread) {
        setProgress(++d->performedActions/d->totalActions);
        //% "Requesting unread articles"
        setCurrentAction(qtTrId("libfuoten-sync-req-articles"));

        d->getUnread = new GetItems(this);
        d->getUnread->setConfiguration(d->configuration);
        d->getUnread->setStorage(d->storage);
        d->getUnread->setType(FuotenEnums::All);
        d->getUnread->setGetRead(false);
        d->getUnread->setBatchSize(-1);
        d->getUnread->setRequestTimeout(150);
        QObject::connect(d->getUnread, &Component::failed, this, &Synchronizer::setError);
        if (d->storage) {
            QObject::connect(d->storage, &AbstractStorage::requestedItems, this, &Synchronizer::requestStarred);
        } else {
            QObject::connect(d->getUnread, &Component::succeeded, this, &Synchronizer::requestStarred);
        }
        d->getUnread->execute();
    }
}


void Synchronizer::requestStarred()
{
    Q_D(Synchronizer);
    if (!d->getStarred) {
        setProgress(++d->performedActions/d->totalActions);
        //% "Requesting starred articles"
        setCurrentAction(qtTrId("libfuoten-sync-req-starred-articles"));

        d->getStarred = new GetItems(this);
        d->getStarred->setConfiguration(d->configuration);
        d->getStarred->setStorage(d->storage);
        d->getStarred->setType(FuotenEnums::Starred);
        d->getStarred->setGetRead(true);
        d->getStarred->setBatchSize(-1);
        QObject::connect(d->getStarred, &Component::failed, this, &Synchronizer::setError);
        if (d->storage) {
            QObject::connect(d->storage, &AbstractStorage::requestedItems, this, &Synchronizer::finished);
        } else {
            QObject::connect(d->getStarred, &Component::succeeded, this, &Synchronizer::finished);
        }
        d->getStarred->execute();
    }
}


void Synchronizer::requestUpdated()
{
    Q_D(Synchronizer);
    if (!d->getUpdated) {
        setProgress(++d->performedActions/d->totalActions);
        //% "Requesting updated and new articles"
        setCurrentAction(qtTrId("libfuoten-sync-req-updated-articles"));

        d->getUpdated = new GetUpdatedItems(this);
        d->getUpdated->setConfiguration(d->configuration);
        d->getUpdated->setStorage(d->storage);
        d->getUpdated->setLastModified(d->configuration->getLastSync());
        d->getUpdated->setType(FuotenEnums::All);
        d->getUpdated->setParentId(0);
        QObject::connect(d->getUpdated, &Component::failed, this, &Synchronizer::setError);
        if (d->storage) {
            QObject::connect(d->storage, &AbstractStorage::requestedItems, this, &Synchronizer::finished);
        } else {
            QObject::connect(d->getUpdated, &Component::succeeded, this, &Synchronizer::finished);
        }
        d->getUpdated->execute();
    }
}


void Synchronizer::finished()
{
    Q_D(Synchronizer);
    if (d->storage) {
        d->storage->clearQueue();
    }
    setProgress(++d->performedActions/d->totalActions);
    d->configuration->setLastSync(QDateTime::currentDateTimeUtc());
    d->setInOperation(false);
    Q_EMIT succeeded();
    d->cleanup();

    qDebug("%s", "Finished synchronizing");
}


AbstractConfiguration *Synchronizer::configuration() const
{
    Q_D(const Synchronizer);
    AbstractConfiguration *c = d->configuration;
    if (!c) {
        c = Component::defaultConfiguration();
    }
    return c;
}


void Synchronizer::setConfiguration(AbstractConfiguration *nAbstractConfiguration)
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Can not change property %s, still in operation.", "configuration");
        return;
    }

    Q_D(Synchronizer);
    if (nAbstractConfiguration != d->configuration) {
        d->configuration = nAbstractConfiguration;
        qDebug("Changed configuration to %p.", d->configuration);
        Q_EMIT configurationChanged(configuration());
    }
}


AbstractStorage *Synchronizer::storage() const
{
    Q_D(const Synchronizer);
    AbstractStorage *s = d->storage;
    if (!s) {
        s = Component::defaultStorage();
    }
    return s;
}


void Synchronizer::setStorage(AbstractStorage *nStorageHandler)
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Can not change property %s, still in operation.", "storage");
        return;
    }

    Q_D(Synchronizer);
    if (nStorageHandler != d->storage) {
        d->storage = nStorageHandler;
        qDebug("Changed storage to %p.", d->storage);
        Q_EMIT storageChanged(storage());
    }
}


bool Synchronizer::inOperation() const
{
    Q_D(const Synchronizer);
    return d->inOperation;
}


qreal Synchronizer::progress() const { Q_D(const Synchronizer); return d->progress; }

void Synchronizer::setProgress(qreal nProgress)
{
    Q_D(Synchronizer);
    if (nProgress != d->progress) {
        d->progress = nProgress;
        qDebug("Changed progress to %f.", d->progress);
        Q_EMIT progressChanged(progress());
    }
}


QString Synchronizer::currentAction() const { Q_D(const Synchronizer); return d->currentAction; }

void Synchronizer::setCurrentAction(const QString &nCurrentAction)
{
    Q_D(Synchronizer);
    if (nCurrentAction != d->currentAction) {
        d->currentAction = nCurrentAction;
        qDebug("Changed currentAction to %s.", qUtf8Printable(d->currentAction));
        Q_EMIT currentActionChanged(currentAction());
    }
}


void Synchronizer::clearError()
{
    setError(nullptr);
}

#include "moc_synchronizer.cpp"
