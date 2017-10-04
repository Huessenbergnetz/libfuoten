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
#include "../API/component.h"
#include <QMetaEnum>

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
        qDebug("Changed inOperation to %s.", d->inOperation ? "true" : "false");
        Q_EMIT inOperationChanged(inOperation());
    }
}




AbstractStorage *BaseModel::storage() const
{
    Q_D(const BaseModel);
    AbstractStorage *_storage = d->storage;
    if (!_storage) {
        _storage = Component::defaultStorage();
    }
    return _storage;
}

void BaseModel::setStorage(AbstractStorage *nStorage)
{
    Q_D(BaseModel);
    if (nStorage != d->storage) {
        d->storage = nStorage;
        qDebug("Changed storage to %p.", d->storage);
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
        qDebug("Changed parentId to %lli.", d->parentId);
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
        qDebug("Changed sortinRole to %s.", FuotenEnums::staticMetaObject.enumerator(FuotenEnums::staticMetaObject.indexOfEnumerator("SortingRole")).valueToKey(d->sortingRole));
        Q_EMIT sortingRoleChanged(sortingRole());
    }
}




Qt::SortOrder BaseModel::sortOrder() const { Q_D(const BaseModel); return d->sortOrder; }

void BaseModel::setSortOrder(Qt::SortOrder nSortOrder)
{
    Q_D(BaseModel);
    if (nSortOrder != d->sortOrder) {
        d->sortOrder = nSortOrder;
        qDebug("Changed sortOrder to %i.", d->sortOrder);
        Q_EMIT sortOrderChanged(sortOrder());
    }
}




bool BaseModel::unreadOnly() const { Q_D(const BaseModel); return d->unreadOnly; }

void BaseModel::setUnreadOnly(bool nUnreadOnly)
{
    Q_D(BaseModel);
    if (nUnreadOnly != d->unreadOnly) {
        d->unreadOnly = nUnreadOnly;
        qDebug("Changed unreadOnly to %s.", d->unreadOnly ? "true" : "false");
        Q_EMIT unreadOnlyChanged(unreadOnly());
    }
}




int BaseModel::limit() const { Q_D(const BaseModel); return d->limit; }

void BaseModel::setLimit(int nLimit)
{
    Q_D(BaseModel);
    if (nLimit != d->limit) {
        d->limit = nLimit;
        qDebug("Changed limit to %i.", d->limit);
        Q_EMIT limitChanged(limit());
    }
}




void BaseModel::reload()
{
    clear();

    setLoaded(false);

    load();
}
