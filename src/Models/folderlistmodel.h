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

#ifndef FUOTENFOLDERLISTMODEL_H
#define FUOTENFOLDERLISTMODEL_H

#include <QObject>
#include "abstractfoldermodel.h"
#include "../fuoten_global.h"

namespace Fuoten {

class FolderListModelPrivate;

/*!
 * \brief List model containing Folder objects.
 *
 * \headerfile "" <Fuoten/Models/FolderListModel>
 */
class FUOTENSHARED_EXPORT FolderListModel : public AbstractFolderModel
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs a new empty FolderListModel.
     */
    FolderListModel(QObject *parent = nullptr);

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &child) const Q_DECL_OVERRIDE;
    QModelIndex sibling(int row, int column, const QModelIndex &idx) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

protected:
    FolderListModel(FolderListModelPrivate &dd, QObject *parent = nullptr);

private:
    Q_DECLARE_PRIVATE(FolderListModel)
    Q_DISABLE_COPY(FolderListModel)
};

}

#endif // FUOTENFOLDERLISTMODEL_H
