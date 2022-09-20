/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENCREATEFEED_P_H
#define FUOTENCREATEFEED_P_H

#include "createfeed.h"
#include "component_p.h"

namespace Fuoten {

class CreateFeedPrivate : public ComponentPrivate
{
public:
    CreateFeedPrivate();
    ~CreateFeedPrivate() override;

    qint64 folderId = 0;
    QUrl url;
};

}

#endif // FUOTENCREATEFEED_P_H
