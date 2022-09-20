/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENBASEMODEL_P_H
#define FUOTENBASEMODEL_P_H

#include "basemodel.h"

namespace Fuoten {

class BaseModelPrivate
{
public:
    BaseModelPrivate();
    virtual ~BaseModelPrivate();

    qint64 parentId = -1;
    AbstractStorage *storage = nullptr;
    int limit = 0;
    Qt::SortOrder sortOrder = Qt::AscendingOrder;
    FuotenEnums::SortingRole sortingRole = FuotenEnums::ID;
    bool unreadOnly = false;
    bool inOperation = false;
    bool loaded = false;

private:
    Q_DISABLE_COPY(BaseModelPrivate)
};

}

#endif // FUOTENBASEMODEL_P_H
