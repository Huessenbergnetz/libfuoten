/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef ABSTRACTSTORAGE_P_H
#define ABSTRACTSTORAGE_P_H

#include "abstractstorage.h"
#include "../error.h"

namespace Fuoten {


class AbstractStoragePrivate
{
public:
    AbstractStoragePrivate();

    virtual ~AbstractStoragePrivate();

    AbstractConfiguration *configuration = nullptr;
    AbstractNotificator *notificator = nullptr;
    Error *error = nullptr;
    int totalUnread = 0;
    int starred = 0;
    bool ready = false;
    bool inOperation = false;

private:
    Q_DISABLE_COPY(AbstractStoragePrivate)
};

}

#endif // ABSTRACTSTORAGE_P_H
