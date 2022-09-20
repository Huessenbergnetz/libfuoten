/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENABSTRACTFOLDERMODEL_P_H
#define FUOTENABSTRACTFOLDERMODEL_P_H

#include "abstractfoldermodel.h"
#include "basemodel_p.h"
#include "../folder.h"

namespace Fuoten {
class AbstractFolderModelPrivate : public BaseModelPrivate
{
public:
    AbstractFolderModelPrivate();
    ~AbstractFolderModelPrivate() override;

    int rowByID(qint64 id);

    QList<Folder*> folders;

private:
    Q_DISABLE_COPY(AbstractFolderModelPrivate)
};
}

#endif // FUOTENABSTRACTFOLDERMODEL_P_H

