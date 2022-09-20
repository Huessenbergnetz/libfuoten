/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENDELETEFOLDER_P_H
#define FUOTENDELETEFOLDER_P_H

#include "deletefolder.h"
#include "component_p.h"

namespace Fuoten {

class DeleteFolderPrivate : public ComponentPrivate
{
public:
    DeleteFolderPrivate();
    ~DeleteFolderPrivate() override;

    qint64 folderId = 0;
};

}

#endif // FUOTENDELETEFOLDER_P_H
