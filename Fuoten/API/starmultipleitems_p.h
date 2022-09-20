/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENSTARMULTIPLEITEMS_P_H
#define FUOTENSTARMULTIPLEITEMS_P_H

#include "starmultipleitems.h"
#include "component_p.h"

namespace Fuoten {

class StarMultipleItemsPrivate : public ComponentPrivate
{
public:
    StarMultipleItemsPrivate();
    explicit StarMultipleItemsPrivate(bool nStarred);
    ~StarMultipleItemsPrivate() override;

    QList<QPair<qint64,QString>> itemsToStar;
    bool starred = false;
};

}

#endif // FUOTENSTARMULTIPLEITEMS_P_H
