/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENABSTRACTFEEDMODEL_P_H
#define FUOTENABSTRACTFEEDMODEL_P_H

#include "abstractfeedmodel.h"
#include "basemodel_p.h"
#include "../feed.h"

namespace Fuoten {

class AbstractFeedModelPrivate : public BaseModelPrivate
{
public:
    AbstractFeedModelPrivate();
    ~AbstractFeedModelPrivate() override;

    int rowByID(qint64 id);

    QList<Feed*> feeds;

private:
    Q_DISABLE_COPY(AbstractFeedModelPrivate)
};

}

#endif // FUOTENABSTRACTFEEDMODEL_P_H
