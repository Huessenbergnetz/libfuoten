/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016-2019 Hüssenbergnetz/Matthias Fehring
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

#ifndef FUOTENFOLDERLISTFILTERMODEL_H
#define FUOTENFOLDERLISTFILTERMODEL_H

#include <QObject>
#include "basefiltermodel.h"
#include "../fuoten_global.h"

namespace Fuoten {

class FolderListFilterModelPrivate;
class AbstractStorage;

/*!
 * \brief Proxy filter model for FolderListModel.
 *
 * This proxy model filters and sorts the data of a FolderListModel that is internally created.
 *
 * \headerfile "" <Fuoten/Models/FolderListFilterModel>
 */
class FUOTENSHARED_EXPORT FolderListFilterModel : public BaseFilterModel
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs a new folder list filter model with the given \a parent.
     */
    explicit FolderListFilterModel(QObject *parent = nullptr);

    /*!
     * \brief Deconstructs the %FeedListModel object.
     */
    ~FolderListFilterModel() override;

    /*!
     * \brief Returns true while the underlying FolderListModel is loading.
     */
    bool inOperation() const override;

    /*!
     * \brief Returns the pointer to the local storage handler set in the underlying FolderListModel.
     */
    AbstractStorage *storage() const override;

    /*!
     * \brief Returns the parent ID that is currently set in the underlying FolderListModel.
     */
    qint64 parentId() const override;

    /*!
     * \brief Sets the pointer to a local storage object in the underlying FolderListModel.
     */
    void setStorage(AbstractStorage *nStorage) override;

    /*!
     * \brief Sets the parent ID in the underlying FolderListModel.
     */
    void setParentId(qint64 nParentId) override;

    /*!
     * \brief Loads the data in the underlying FolderListModel.
     */
    Q_INVOKABLE void load(const QString &locale = QString()) override;

    /*!
     * \brief Reloads the complete underlying FolderListModel.
     */
    Q_INVOKABLE void reload(const QString &locale = QString()) override;

    bool loaded() const override;

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
