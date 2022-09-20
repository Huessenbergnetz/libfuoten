/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENFOLDER_P_H
#define FUOTENFOLDER_P_H

#include "folder.h"
#include "baseitem_p.h"

namespace Fuoten {

class FolderPrivate : public BaseItemPrivate
{
public:
    FolderPrivate();
    FolderPrivate(qint64 _id, const QString &_name, uint _feedCount, uint _unreadCount);
    ~FolderPrivate() override;

    QString name;
    uint feedCount;
    uint unreadCount;

private:
    Q_DISABLE_COPY(FolderPrivate)
};

}

#endif // FUOTENFOLDER_P_H
