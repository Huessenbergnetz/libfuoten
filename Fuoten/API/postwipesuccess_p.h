/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENPOSTWIPESUCCESS_P_H
#define FUOTENPOSTWIPESUCCESS_P_H

#include "postwipesuccess.h"
#include "component_p.h"

namespace Fuoten {

class PostWipeSuccessPrivate : public ComponentPrivate
{
public:
    PostWipeSuccessPrivate();
    ~PostWipeSuccessPrivate();

    QString appToken;
};

}

#endif // FUOTENPOSTWIPESUCCESS_P_H
