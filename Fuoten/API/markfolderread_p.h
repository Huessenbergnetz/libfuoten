/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENMARKFOLDERREAD_P_H
#define FUOTENMARKFOLDERREAD_P_H

#include "markfolderread.h"
#include "component_p.h"

namespace Fuoten {

class MarkFolderReadPrivate : public ComponentPrivate
{
public:
    MarkFolderReadPrivate();
    ~MarkFolderReadPrivate() override;

    qint64 folderId;
    qint64 newestItemId;
};

}

#endif // FUOTENMARKFOLDERREAD_P_H
