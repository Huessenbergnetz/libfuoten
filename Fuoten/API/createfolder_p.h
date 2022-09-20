/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENCREATEFOLDER_P_H
#define FUOTENCREATEFOLDER_P_H

#include "createfolder.h"
#include "component_p.h"

namespace Fuoten {

class CreateFolderPrivate : public ComponentPrivate
{
public:
    CreateFolderPrivate();
    ~CreateFolderPrivate() override;

    QString name;
};

}

#endif // FUOTENCREATEFOLDER_P_H
