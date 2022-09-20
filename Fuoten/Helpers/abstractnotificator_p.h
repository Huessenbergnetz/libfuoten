/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENABSTRACTNOTIFICATOR_P_H
#define FUOTENABSTRACTNOTIFICATOR_P_H

#include "abstractnotificator.h"

namespace Fuoten {

class AbstractNotificatorPrivate
{
public:
    AbstractNotificatorPrivate() {}

    ~AbstractNotificatorPrivate() {}

    QString appIcon;
    QString appName;
    bool enabled = false;
    bool articlePublishing = false;
};

}

#endif // FUOTENABSTRACTNOTIFICATOR_P_H
