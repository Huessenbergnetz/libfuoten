/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "baseitem_p.h"
#include "error.h"

using namespace Fuoten;

BaseItemPrivate::BaseItemPrivate(qint64 _id) :
    id(_id)
{

}


BaseItemPrivate::~BaseItemPrivate()
{

}


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


qint64 BaseItem::id() const { Q_D(const BaseItem); return d->id; }

void BaseItem::setId(qint64 nId)
{
    Q_D(BaseItem);
    if (nId != d->id) {
        d->id = nId;
        qDebug("Changed id to %lli.", d->id);
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

        qDebug("Changed error to %p.", d->error);
        Q_EMIT errorChanged(error());

        if (old) {
            if (old->parent() == this) {
                delete old;
            }
        }
    }
}


Component *BaseItem::component() const { Q_D(const BaseItem); return d->comp; }

void BaseItem::setComponent(Component *nComp)
{
    Q_D(BaseItem);

    Component *old = d->comp;
    d->comp = nComp;

    if (d->comp) {
        connect(d->comp, &Component::failed, this, [=] (Error *nError) {setError(nError);});
        connect(d->comp, &Component::failed, this, [=] () {setComponent(nullptr);});
        connect(d->comp, &Component::failed, d->comp, &QObject::deleteLater);
    }

    if (old) {
        old->deleteLater();
    }

    Q_EMIT inOperationChanged(inOperation());
}


void BaseItem::clearError()
{
    setError(nullptr);
}


void BaseItem::copy(BaseItem *other)
{
    Q_UNUSED(other);
}

#include "moc_baseitem.cpp"
