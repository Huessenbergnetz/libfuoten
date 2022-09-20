/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENARTICLELISTMODEL_H
#define FUOTENARTICLELISTMODEL_H

#include <QObject>
#include "abstractarticlemodel.h"
#include "../fuoten_global.h"

namespace Fuoten {

class ArticleListModelPrivate;

/*!
 * \brief List model containing Article objects.
 *
 * List model with one column that contains a pointer to an Article object. The Qt::DisplayRole (\a display in QML) returns
 * the pointer. To use this model, you need an AbstractStorage derived class that has to be set to the BaseModel::storage property.
 *
 * \headerfile "" <Fuoten/Models/ArticleListModel>
 */
class FUOTENSHARED_EXPORT ArticleListModel : public AbstractArticleModel
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs a new empty ArticleListModel object with the given \a parent.
     */
    explicit ArticleListModel(QObject *parent = nullptr);

    /*!
     * \brief Destroys the %ArticleListModel object.
     */
    ~ArticleListModel() override;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    QModelIndex sibling(int row, int column, const QModelIndex &idx) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

protected:
    ArticleListModel(ArticleListModelPrivate &dd, QObject *parent = nullptr);

private:
    Q_DISABLE_COPY(ArticleListModel)
    Q_DISABLE_MOVE(ArticleListModel)
    Q_DECLARE_PRIVATE(ArticleListModel)
};

}


#endif // FUOTENARTICLELISTMODEL_H
