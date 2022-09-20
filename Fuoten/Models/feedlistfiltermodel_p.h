/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENFEEDLISTFILTERMODEL_P_H
#define FUOTENFEEDLISTFILTERMODEL_P_H

#include "feedlistfiltermodel.h"
#include "basefiltermodel_p.h"
#include "feedlistmodel.h"

namespace Fuoten {

class FeedListFilterModelPrivate : public BaseFilterModelPrivate
{
public:
    FeedListFilterModelPrivate();
    ~FeedListFilterModelPrivate() override;

    const QScopedPointer<FeedListModel> flm;
    bool respectPinned;
    bool sortByFolder;
};

}

#endif // FUOTENFEEDLISTFILTERMODEL_P_H
