/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016-2020 Hüssenbergnetz/Matthias Fehring
 * https://github.com/Huessenbergnetz/libfuoten
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
 * List model with one column that contains a pointer to a Folder object. The Qt::DisplayRole (\a display in QML) returns
 * the pointer. To use this model, you need an AbstractStorage that has to be set to the BaseModel::storage property.
 *
 * \headerfile "" <Fuoten/Models/FolderListModel>
 */
class FUOTENSHARED_EXPORT FolderListModel : public AbstractFolderModel
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs a new empty folder list model with the given \a parent.
     */
    explicit FolderListModel(QObject *parent = nullptr);

    /*!
     * \brief Deconstructs the %FeedListModel object.
     */
    ~FolderListModel() override;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    QModelIndex sibling(int row, int column, const QModelIndex &idx) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

protected:
    FolderListModel(FolderListModelPrivate &dd, QObject *parent = nullptr);

private:
    Q_DECLARE_PRIVATE(FolderListModel)
    Q_DISABLE_COPY(FolderListModel)
    Q_DISABLE_MOVE(FolderListModel)
};

}

#endif // FUOTENFOLDERLISTMODEL_H
