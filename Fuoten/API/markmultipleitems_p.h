/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENMARKMULTIPLEITEMS_P_H
#define FUOTENMARKMULTIPLEITEMS_P_H

#include "markmultipleitems.h"
#include "component_p.h"

namespace Fuoten {


class MarkMultipleItemsPrivate : public ComponentPrivate
{
public:
    MarkMultipleItemsPrivate();
    MarkMultipleItemsPrivate(const IdList &nItemIds, bool nUnread);
    ~MarkMultipleItemsPrivate() override;

    IdList itemIds;
    bool unread;
};

}

#endif // FUOTENMARKMULTIPLEITEMS_P_H
