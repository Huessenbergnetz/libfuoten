/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENRENAMEFEED_P_H
#define FUOTENRENAMEFEED_P_H

#include "renamefeed.h"
#include "component_p.h"

namespace Fuoten {

class RenameFeedPrivate : public ComponentPrivate
{
public:
    RenameFeedPrivate();
    ~RenameFeedPrivate() override;

    qint64 feedId;
    QString newName;
};

}

#endif // FUOTENRENAMEFEED_P_H
