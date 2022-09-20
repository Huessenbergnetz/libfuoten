/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENCONVERTTOAPPPASSWORD_P_H
#define FUOTENCONVERTTOAPPPASSWORD_P_H

#include "converttoapppassword.h"
#include "component_p.h"

namespace Fuoten {

class ConvertToAppPasswordPrivate : public ComponentPrivate
{
public:
    ConvertToAppPasswordPrivate();
    ~ConvertToAppPasswordPrivate() override;
};

}

#endif // FUOTENCONVERTTOAPPPASSWORD_P_H
