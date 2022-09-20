/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENMARKFEEDREAD_P_H
#define FUOTENMARKFEEDREAD_P_H

#include "markfeedread.h"
#include "component_p.h"

namespace Fuoten {


class MarkFeedReadPrivate : public ComponentPrivate
{
public:
    MarkFeedReadPrivate();
    ~MarkFeedReadPrivate() override;

    qint64 feedId;
    qint64 newestItemId;
};

}

#endif // FUOTENMARKFEEDREAD_P_H
