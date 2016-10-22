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

#include "baseitem_p.h"
#include "error.h"
#ifdef QT_DEBUG
#include <QtDebug>
#endif

using namespace Fuoten;


BaseItem::BaseItem(QObject *parent) :
    QObject(parent), d_ptr(new BaseItemPrivate)
{
}


BaseItem::BaseItem(BaseItemPrivate &dd, QObject *parent) :
    QObject(parent), d_ptr(&dd)
{

}



BaseItem::~BaseItem()
{
}


bool BaseItem::inOperation() const
{
    Q_D(const BaseItem);
    if (d->comp) {
        return d->comp->inOperation();
    } else {
        return false;
    }
}





quint64 BaseItem::id() const { Q_D(const BaseItem); return d->id; }

void BaseItem::setId(quint64 nId)
{
    Q_D(BaseItem); 
    if (nId != d->id) {
        d->id = nId;
#ifdef QT_DEBUG
        qDebug() << "Changed id to" << d->id;
#endif
        Q_EMIT idChanged(id());
    }
}




Error *BaseItem::error() const
{
    Q_D(const BaseItem); return d->error;
}

void BaseItem::setError(Error *nError)
{
    Q_D(BaseItem);
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
        Q_EMIT errorChanged(error());

        if (old) {
            if (old->parent() == this) {
                delete old;
            }
        }

        if (d->error && (d->error->type() != Error::NoError) && (d->error->severity() == Error::Critical || d->error->severity() == Error::Fatal)) {
            Q_EMIT inOperationChanged(false);
        }
    }
}




Configuration *BaseItem::configuration() const { Q_D(const BaseItem); return d->configuration; }

void BaseItem::setConfiguration(Configuration *nConfiguration)
{
    Q_D(BaseItem);
    if (nConfiguration != d->configuration) {
        d->configuration = nConfiguration;
#ifdef QT_DEBUG
        qDebug() << "Changed configuration to" << d->configuration;
#endif
        Q_EMIT configurationChanged(configuration());
    }
}



StorageHandler *BaseItem::storageHandler() const { Q_D(const BaseItem); return d->storageHandler; }

void BaseItem::setStorageHandler(StorageHandler *nStorageHandler)
{
    Q_D(BaseItem);
    if (nStorageHandler != d->storageHandler) {
        d->storageHandler = nStorageHandler;
#ifdef QT_DEBUG
        qDebug() << "Changed storageHandler to" << d->storageHandler;
#endif
        Q_EMIT storageHandlerChanged(storageHandler());
    }
}
