/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENABSTRACTARTICLEMODEL_P_H
#define FUOTENABSTRACTARTICLEMODEL_P_H

#include "abstractarticlemodel.h"
#include "basemodel_p.h"
#include "../article.h"

namespace Fuoten {

class AbstractArticleModelPrivate : public BaseModelPrivate
{
public:
    AbstractArticleModelPrivate();
    ~AbstractArticleModelPrivate() override;

    int rowByID(qint64 id);
    int rowByGuidHash(const QString &guidHash);

    QList<Article*> articles;
    int bodyLimit = -1;
    FuotenEnums::Type parentIdType = FuotenEnums::All;
    bool starredOnly = false;

private:
    Q_DISABLE_COPY(AbstractArticleModelPrivate)
};

}

#endif // FUOTENABSTRACTARTICLEMODEL_P_H
