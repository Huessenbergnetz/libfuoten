/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENFOLDERLISTMODEL_P_H
#define FUOTENFOLDERLISTMODEL_P_H

#include "folderlistmodel.h"
#include "abstractfoldermodel_p.h"

namespace Fuoten {

class FolderListModelPrivate : public AbstractFolderModelPrivate
{
public:
    FolderListModelPrivate();
    ~FolderListModelPrivate() override;

private:
    Q_DISABLE_COPY(FolderListModelPrivate)
};

}

#endif // FUOTENFOLDERLISTMODEL_P_H

