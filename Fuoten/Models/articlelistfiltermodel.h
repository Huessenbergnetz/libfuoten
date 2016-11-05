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

#ifndef FUOTENARTICLELISTFILTERMODEL_H
#define FUOTENARTICLELISTFILTERMODEL_H

#include <QObject>
#include "basefiltermodel.h"
#include "../fuoten.h"
#include "../fuoten_global.h"

namespace Fuoten {

class ArticleListFilterModelPrivate;

/*!
 * \brief Proxy filter model for ArticleListModel.
 *
 * This proxy model filters and sorts the data of a ArticleListModel that is internally created.
 *
 * \headerfile "" <Fuoten/Models/ArticleListFilterModel>
 */
class FUOTENSHARED_EXPORT ArticleListFilterModel : public BaseFilterModel
{
    Q_OBJECT
    /*!
     * \brief Defines the type of the parentId used in the underlying model.
     *
     * Defaults to FuotenEnums::All
     *
     * \par Access functions:
     * <TABLE><TR><TD>FuotenEnums::Type</TD><TD>parentIdType() const</TD></TR><TR><TD>void</TD><TD>setParentIdType(const FuotenEnums::Type &nParentIdType)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>parentIdTypeChanged(const FuotenEnums::Type &parentIdType)</TD></TR></TABLE>
     */
    Q_PROPERTY(Fuoten::FuotenEnums::Type parentIdType READ parentIdType WRITE setParentIdType NOTIFY parentIdTypeChanged)
public:
    /*!
     * \brief Constructs a new ArticleListFilterModel object with the given \a parent.
     */
    explicit ArticleListFilterModel(QObject *parent = nullptr);


    /*!
     * \brief Returns true while the underlying ArticleListModel is loading.
     */
    bool inOperation() const override;

    /*!
     * \brief Returns the pointer to the local storage handler set in the underlying ArticleListModel.
     */
    AbstractStorage *storage() const override;

    /*!
     * \brief Returns the parent ID that is currently set in the underlying ArticleListModel.
     */
    qint64 parentId() const override;

    /*!
     * \brief Sets the pointer to a local storage object in the underlying ArticleListModel.
     * \param nStorage reimplemented local storage
     */
    void setStorage(AbstractStorage *nStorage) override;

    /*!
     * \brief Sets the parent ID in the underlying ArticleListModel.
     * \param nParentId ID of the parent folder, \c -1 (default) to show all feeds
     */
    void setParentId(qint64 nParentId) override;

    /*!
     * \brief Loads the data in the underlying ArticleListModel.
     */
    Q_INVOKABLE void load(const QString &locale = QString()) override;

    /*!
     * \brief Getter function for the \link ArticleListFilterModel::parentIdType parentIdType \endlink property.
     * \sa ArticleListFilterModel::setParentIdType(), ArticleListFilterModel::parentIdTypeChanged()
     */
    FuotenEnums::Type parentIdType() const;

    /*!
     * \brief Setter function for the \link ArticleListFilterModel::parentIdType parentIdType \endlink property.
     * Emits the parentIdTypeChanged() signal if \a nParentIdType is not equal to the stored value.
     * \sa ArticleListFilterModel::parentIdType(), ArticleListFilterModel::parentIdTypeChanged()
     */
    void setParentIdType(const FuotenEnums::Type &nParentIdType);


Q_SIGNALS:
    /*!
     * \brief This is emitted if the value of the \link ArticleListFilterModel::parentIdType parentIdType \endlink property changes.
     * \sa ArticleListFilterModel::parentIdType(), ArticleListFilterModel::setParentIdType()
     */
    void parentIdTypeChanged(const FuotenEnums::Type &parentIdType);

protected:
    ArticleListFilterModel(ArticleListFilterModelPrivate &&dd, QObject *parent = nullptr);

    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

private:
    Q_DISABLE_COPY(ArticleListFilterModel)
    Q_DECLARE_PRIVATE(ArticleListFilterModel)

};

}


#endif // FUOTENARTICLELISTFILTERMODEL_H
