/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENGETVERSION_P_H
#define FUOTENGETVERSION_P_H

#include "getversion.h"
#include "component_p.h"

namespace Fuoten {

class GetVersionPrivate : public ComponentPrivate {
public:
    GetVersionPrivate();
    ~GetVersionPrivate();

    QVersionNumber version;
};

}

#endif // FUOTENGETVERSION_P_H

