/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENGETWIPESTATUS_P_H
#define FUOTENGETWIPESTATUS_P_H

#include "getwipestatus.h"
#include "component_p.h"

namespace Fuoten {

class GetWipeStatusPrivate : public ComponentPrivate
{
public:
    GetWipeStatusPrivate();
    ~GetWipeStatusPrivate() override;
};

}

#endif // FUOTENGETWIPESTATUS_P_H
