/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENDELETEFEED_P_H
#define FUOTENDELETEFEED_P_H

#include "deletefeed.h"
#include "component_p.h"

namespace Fuoten {

class DeleteFeedPrivate : public ComponentPrivate
{
public:
    DeleteFeedPrivate();
    ~DeleteFeedPrivate() override;

    qint64 feedId = 0;
};

}

#endif // FUOTENDELETEFEED_P_H
