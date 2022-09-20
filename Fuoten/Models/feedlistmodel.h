/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENFEEDLISTMODEL_H
#define FUOTENFEEDLISTMODEL_H

#include <QObject>
#include "abstractfeedmodel.h"
#include "../fuoten_global.h"

namespace Fuoten {

class FeedListModelPrivate;

/*!
 * \brief List model containing Feed objects.
 *
 * List model with one column that contains a pointer to a Feed object. The Qt::DisplayRole (\a display in QML) returns
 * the pointer. To use this model, you need an AbstractStorage derived class that has to be set to the BaseModel::storage property.
 *
 * \headerfile "" <Fuoten/Models/FeedListModel>
 */
class FUOTENSHARED_EXPORT FeedListModel : public AbstractFeedModel
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs a new empty feed list model with the given \a parent.
     */
    explicit FeedListModel(QObject *parent = nullptr);

    /*!
     * \brief Deconstructs the %FeedListModel object.
     */
    ~FeedListModel() override;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    QModelIndex sibling(int row, int column, const QModelIndex &idx) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

protected:
    FeedListModel(FeedListModelPrivate &dd, QObject *parent = nullptr);

private:
    Q_DISABLE_COPY(FeedListModel)
    Q_DISABLE_MOVE(FeedListModel)
    Q_DECLARE_PRIVATE(FeedListModel)
};

}

#endif // FUOTENFEEDLISTMODEL_H
