/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENRENAMEFOLDER_P_H
#define FUOTENRENAMEFOLDER_P_H

#include "renamefolder.h"
#include "component_p.h"

namespace Fuoten {

class RenameFolderPrivate : public ComponentPrivate
{
public:
    RenameFolderPrivate();
    ~RenameFolderPrivate() override;

    qint64 folderId;
    QString newName;
};

}

#endif // FUOTENRENAMEFOLDER_P_H
