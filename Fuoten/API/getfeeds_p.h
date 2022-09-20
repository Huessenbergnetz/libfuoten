/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENGETFEEDS_P_H
#define FUOTENGETFEEDS_P_H

#include "getfeeds.h"
#include "component_p.h"

namespace Fuoten {

class GetFeedsPrivate : public ComponentPrivate {
public:
    GetFeedsPrivate();
    ~GetFeedsPrivate() override;
};

}

#endif // FUOTENGETFEEDS_P_H
