/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016 Buschtrommel / Matthias Fehring
 * https://www.buschmann23.de/entwicklung/bibliotheken/libfuoten/
 * https://github.com/Buschtrommel/libfuoten
 *
 * This library is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef FUOTENFOLDERLISTFILTERMODEL_H
#define FUOTENFOLDERLISTFILTERMODEL_H

#include <QObject>
#include "basefiltermodel.h"
#include "../fuoten_global.h"

namespace Fuoten {

class FolderListFilterModelPrivate;
class AbstractStorage;

class FUOTENSHARED_EXPORT FolderListFilterModel : public BaseFilterModel
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs a new FolderListFilterModel.
     */
    FolderListFilterModel(QObject *parent = nullptr);

    bool inOperation() const override;
    AbstractStorage *storage() const override;
    qint64 parentId() const override;

    void setStorage(AbstractStorage *nStorage) override;
    void setParentId(qint64 nParentId) override;

    Q_INVOKABLE void load() override;

protected:
    FolderListFilterModel(FolderListFilterModelPrivate &dd, QObject *parent = nullptr);

    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    Q_DISABLE_COPY(FolderListFilterModel)
    Q_DECLARE_PRIVATE(FolderListFilterModel)

};

}

#endif // FUOTENFOLDERLISTFILTERMODEL_H
