/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENFOLDERLISTMODEL_H
#define FUOTENFOLDERLISTMODEL_H

#include <QObject>
#include "abstractfoldermodel.h"
#include "../fuoten_global.h"
#include "fuoten_export.h"

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
class FUOTEN_EXPORT FolderListModel : public AbstractFolderModel
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
};

}

#endif // FUOTENFOLDERLISTMODEL_H
