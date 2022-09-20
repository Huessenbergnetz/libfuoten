/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENMARKITEM_P_H
#define FUOTENMARKITEM_P_H

#include "markitem.h"
#include "component_p.h"

namespace Fuoten {


class MarkItemPrivate : public ComponentPrivate
{
public:
    MarkItemPrivate();
    MarkItemPrivate(qint64 nItemId, bool nUnread);
    ~MarkItemPrivate() override;


    qint64 itemId;
    bool unread;
};

}

#endif // FUOTENMARKITEM_P_H
