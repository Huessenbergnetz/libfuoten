/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENFOLDERLISTFILTERMODEL_P_H
#define FUOTENFOLDERLISTFILTERMODEL_P_H

#include "folderlistfiltermodel.h"
#include "basefiltermodel_p.h"
#include "folderlistmodel.h"

namespace Fuoten {
class FolderListFilterModelPrivate : public BaseFilterModelPrivate
{
public:
    FolderListFilterModelPrivate();
    ~FolderListFilterModelPrivate() override;

    const QScopedPointer<FolderListModel> flm;
//    FolderListModel *flm;
};
}

#endif // FUOTENFOLDERLISTFILTERMODEL_P_H

