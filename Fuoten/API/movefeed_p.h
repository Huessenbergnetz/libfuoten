/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENMOVEFEED_P_H
#define FUOTENMOVEFEED_P_H

#include "movefeed.h"
#include "component_p.h"

namespace Fuoten {

class MoveFeedPrivate : public ComponentPrivate
{
public:
    MoveFeedPrivate();
    ~MoveFeedPrivate() override;

    qint64 feedId;
    qint64 folderId;
};

}

#endif // FUOTENMOVEFEED_P_H
