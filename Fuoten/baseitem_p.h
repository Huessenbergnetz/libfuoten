/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENBASEITEM_P_H
#define FUOTENBASEITEM_P_H

#include "baseitem.h"
#include "API/component.h"
#include "Helpers/abstractconfiguration.h"
#include "Storage/abstractstorage.h"

namespace Fuoten {

class BaseItemPrivate
{
public:
    BaseItemPrivate() {}

    explicit BaseItemPrivate(qint64 _id);
    virtual ~BaseItemPrivate();

    qint64 id = 0;
    Error *error = nullptr;
    Component *comp = nullptr;

private:
    Q_DISABLE_COPY(BaseItemPrivate)
};

}

#endif // FUOTENBASEITEM_P_H
