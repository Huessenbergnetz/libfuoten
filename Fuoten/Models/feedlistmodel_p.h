/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENFEEDLISTMODEL_P_H
#define FUOTENFEEDLISTMODEL_P_H

#include "feedlistmodel.h"
#include "abstractfeedmodel_p.h"

namespace Fuoten {

class FeedListModelPrivate : public AbstractFeedModelPrivate
{
public:
    FeedListModelPrivate();
    ~FeedListModelPrivate() override;

private:
    Q_DISABLE_COPY(FeedListModelPrivate)
};

}

#endif // FUOTENFEEDLISTMODEL_P_H
