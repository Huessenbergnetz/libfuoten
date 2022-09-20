/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "fuoten.h"

QVersionNumber Fuoten::version()
{
    return QVersionNumber::fromString(QStringLiteral(FUOTEN_VERSION));
}
