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
            inOperation = false;
            Q_EMIT q->inOperationChanged(false);
            Q_EMIT q->failed(error);
        }
    }
}



void SynchronizerPrivate::start()
{
    getFolders = new GetFolders(q_ptr);
    getFolders->setConfiguration(configuration);
    getFolders->setStorage(storage);
    QObject::connect(getFolders, &Component::failed, [=] (Error *e) {setError(e);});
    if (storage) {
        QObject::connect(storage, &AbstractStorage::requestedFolders, [=] () {finished();});
    } else {
        QObject::connect(getFolders, &Component::succeeded, [=] () {finished();});
    }
    getFolders->execute();
}




void SynchronizerPrivate::finished()
{
    Q_Q(Synchronizer);
    inOperation = false;
    Q_EMIT q->inOperationChanged(false);
    Q_EMIT q->succeeded();
    getFolders->deleteLater();
    getFolders = nullptr;
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
