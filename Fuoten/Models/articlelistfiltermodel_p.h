/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENARTICLELISTFILTERMODEL_P_H
#define FUOTENARTICLELISTFILTERMODEL_P_H

#include "articlelistfiltermodel.h"
#include "basefiltermodel_p.h"
#include "articlelistmodel.h"

namespace Fuoten {

class ArticleListFilterModelPrivate : public BaseFilterModelPrivate
{
public:
    ArticleListFilterModelPrivate();
    ~ArticleListFilterModelPrivate() override;

    const QScopedPointer<ArticleListModel> alm;
};

}

#endif // FUOTENARTICLELISTFILTERMODEL_P_H
