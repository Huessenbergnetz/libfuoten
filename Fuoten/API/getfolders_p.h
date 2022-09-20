/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENGETFOLDERS_P_H
#define FUOTENGETFOLDERS_P_H

#include "getfolders.h"
#include "component_p.h"


namespace Fuoten {

class GetFoldersPrivate : public ComponentPrivate {
public:
    GetFoldersPrivate();
    ~GetFoldersPrivate() override;
};

}

#endif // FUOTENGETFOLDERS_P_H

