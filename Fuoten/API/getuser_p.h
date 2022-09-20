/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENGETUSER_P_H
#define FUOTENGETUSER_P_H

#include "getuser.h"
#include "component_p.h"

namespace Fuoten {

class GetUserPrivate : public ComponentPrivate {
public:
    GetUserPrivate();
    ~GetUserPrivate() override;
};

}

#endif // FUOTENGETUSER_P_H

