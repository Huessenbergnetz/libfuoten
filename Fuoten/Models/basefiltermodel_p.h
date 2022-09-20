/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENBASEFILTERMODEL_P_H
#define FUOTENBASEFILTERMODEL_P_H

#include "basefiltermodel.h"
#include <QStringMatcher>

namespace Fuoten {

class BaseFilterModelPrivate
{
public:
    BaseFilterModelPrivate();
    virtual ~BaseFilterModelPrivate();

    QStringMatcher search;
    Qt::SortOrder sortOrder = Qt::AscendingOrder;
    FuotenEnums::SortingRole sortingRole = FuotenEnums::ID;
    bool hideRead = false;

private:
    Q_DISABLE_COPY(BaseFilterModelPrivate)
};

}

#endif // FUOTENBASEFILTERMODEL_P_H
