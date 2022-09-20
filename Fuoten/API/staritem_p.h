/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENSTARITEM_P_H
#define FUOTENSTARITEM_P_H

#include "staritem.h"
#include "component_p.h"

namespace Fuoten {

class StarItemPrivate : public ComponentPrivate
{
public:
    StarItemPrivate();
    StarItemPrivate(qint64 nFeedId, const QString &nGuidHash, bool nStarred);
    ~StarItemPrivate() override;

    qint64 feedId;
    QString guidHash;
    bool starred;
};

}

#endif // FUOTENSTARITEM_P_H
