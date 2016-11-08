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

#include "basemodel_p.h"
#ifdef QT_DEBUG
#include <QtDebug>
#endif

using namespace Fuoten;


BaseModel::BaseModel(QObject *parent) :
    QAbstractItemModel(parent), d_ptr(new BaseModelPrivate)
{
}


BaseModel::BaseModel(BaseModelPrivate &dd, QObject *parent) :
    QAbstractItemModel(parent), d_ptr(&dd)
{
}



BaseModel::~BaseModel()
{
}


bool BaseModel::inOperation() const { Q_D(const BaseModel); return d->inOperation; }

void BaseModel::setInOperation(bool nInOperation)
{
    Q_D(BaseModel); 
    if (nInOperation != d->inOperation) {
        d->inOperation = nInOperation;
#ifdef QT_DEBUG
        qDebug() << "Changed inOperation to" << d->inOperation;
#endif
        Q_EMIT inOperationChanged(inOperation());
    }
}




AbstractStorage *BaseModel::storage() const { Q_D(const BaseModel); return d->storage; }

void BaseModel::setStorage(AbstractStorage *nStorage)
{
    Q_D(BaseModel);
    if (nStorage != d->storage) {
        d->storage = nStorage;
#ifdef QT_DEBUG
        qDebug() << "Changed storage to" << d->storage;
#endif
        Q_EMIT storageChanged(storage());

        handleStorageChanged();
    }
}



void BaseModel::handleStorageChanged()
{

}


QModelIndex BaseModel::findByID(qint64 id) const
{
    Q_UNUSED(id)
    return QModelIndex();
}


QHash<qint64, QModelIndex> BaseModel::findByIDs(const IdList &ids) const
{
    Q_UNUSED(ids)
    return QHash<qint64, QModelIndex>();
}



bool BaseModel::loaded() const
{
    Q_D(const BaseModel);
    return d->loaded;
}


void BaseModel::setLoaded(bool loaded)
{
    Q_D(BaseModel);
    d->loaded = loaded;
}


qint64 BaseModel::parentId() const { Q_D(const BaseModel); return d->parentId; }

void BaseModel::setParentId(qint64 nParentId)
{
    Q_D(BaseModel);
    if (nParentId != d->parentId) {
        d->parentId = nParentId;
#ifdef QT_DEBUG
        qDebug() << "Changed parentId to" << d->parentId;
#endif
        Q_EMIT parentIdChanged(parentId());
        Q_EMIT doubleParentIdChanged(doubleParentId());
    }
}



double BaseModel::doubleParentId() const { Q_D(const BaseModel); return static_cast<double>(d->parentId); }

void BaseModel::setDoubleParentId(double nDoubleParentId)
{
    setParentId(static_cast<qint64>(nDoubleParentId));
}


FuotenEnums::SortingRole BaseModel::sortingRole() const { Q_D(const BaseModel); return d->sortingRole; }

void BaseModel::setSortingRole(FuotenEnums::SortingRole nSortingRole)
{
    Q_D(BaseModel);
    if (nSortingRole != d->sortingRole) {
        d->sortingRole = nSortingRole;
#ifdef QT_DEBUG
        qDebug() << "Changed sortingRole to" << d->sortingRole;
#endif
        Q_EMIT sortingRoleChanged(sortingRole());
    }
}




Qt::SortOrder BaseModel::sortOrder() const { Q_D(const BaseModel); return d->sortOrder; }

void BaseModel::setSortOrder(Qt::SortOrder nSortOrder)
{
    Q_D(BaseModel);
    if (nSortOrder != d->sortOrder) {
        d->sortOrder = nSortOrder;
#ifdef QT_DEBUG
        qDebug() << "Changed sortOrder to" << d->sortOrder;
#endif
        Q_EMIT sortOrderChanged(sortOrder());
    }
}




bool BaseModel::unreadOnly() const { Q_D(const BaseModel); return d->unreadOnly; }

void BaseModel::setUnreadOnly(bool nUnreadOnly)
{
    Q_D(BaseModel);
    if (nUnreadOnly != d->unreadOnly) {
        d->unreadOnly = nUnreadOnly;
#ifdef QT_DEBUG
        qDebug() << "Changed unreadOnly to" << d->unreadOnly;
#endif
        Q_EMIT unreadOnlyChanged(unreadOnly());
    }
}




int BaseModel::limit() const { Q_D(const BaseModel); return d->limit; }

void BaseModel::setLimit(int nLimit)
{
    Q_D(BaseModel);
    if (nLimit != d->limit) {
        d->limit = nLimit;
#ifdef QT_DEBUG
        qDebug() << "Changed limit to" << d->limit;
#endif
        Q_EMIT limitChanged(limit());
    }
}




void BaseModel::reload()
{
    clear();

    setLoaded(false);

    load();
}
