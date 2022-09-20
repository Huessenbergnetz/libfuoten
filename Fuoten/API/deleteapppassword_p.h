/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENDELETEAPPPASSWORD_P_H
#define FUOTENDELETEAPPPASSWORD_P_H

#include "deleteapppassword.h"
#include "component_p.h"

namespace Fuoten {

class DeleteAppPasswordPrivate : public ComponentPrivate
{
public:
    DeleteAppPasswordPrivate();
    ~DeleteAppPasswordPrivate() override;
};

}

#endif // FUOTENDELETEAPPPASSWORD_P_H
