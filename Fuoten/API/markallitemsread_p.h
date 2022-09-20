/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENMARKALLITEMSREAD_P_H
#define FUOTENMARKALLITEMSREAD_P_H

#include "markallitemsread.h"
#include "component_p.h"

namespace Fuoten {

class MarkAllItemsReadPrivate : public ComponentPrivate
{
public:
    MarkAllItemsReadPrivate();
    explicit MarkAllItemsReadPrivate(qint64 nNewestItemId);
    ~MarkAllItemsReadPrivate() override;

    qint64 newestItemId;
};

}

#endif // FUOTENMARKALLITEMSREAD_P_H
