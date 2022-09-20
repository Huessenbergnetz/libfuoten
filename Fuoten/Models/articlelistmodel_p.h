/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENARTICLELISTMODEL_P_H
#define FUOTENARTICLELISTMODEL_P_H

#include "articlelistmodel.h"
#include "abstractarticlemodel_p.h"

namespace Fuoten {


class ArticleListModelPrivate : public AbstractArticleModelPrivate
{
public:
    ArticleListModelPrivate();
    ~ArticleListModelPrivate() override;

private:
    Q_DISABLE_COPY(ArticleListModelPrivate)
};

}

#endif // FUOTENARTICLELISTMODEL_P_H
