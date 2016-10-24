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

#ifndef FUOTENBASEFILTERMODEL_H
#define FUOTENBASEFILTERMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>
#include "../fuoten.h"
#include "../fuoten_global.h"

namespace Fuoten {

class BaseFilterModelPrivate;
class AbstractStorage;

/*!
 * \brief Base filter model for all other filter models.
 *
 * \headerfile "" <Fuoten/Model/BaseFilterModel>
 */
class FUOTENSHARED_EXPORT BaseFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
    /*!
     * \brief Returns true while the underlying BaseModel is initially loading data.
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>inOperation() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>inOperationChanged(bool inOperation)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool inOperation READ inOperation NOTIFY inOperationChanged)
    /*!
     * \brief Pointer to the AbstractStorage object of the underlying BaseModel.
     *
     * \par Access functions:
     * <TABLE><TR><TD>AbstractStorage*</TD><TD>storage() const</TD></TR><TR><TD>void</TD><TD>setStorage(AbstractStorage *nStorage)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>storageChanged(AbstractStorage *storage)</TD></TR></TABLE>
     */
    Q_PROPERTY(Fuoten::AbstractStorage *storage READ storage WRITE setStorage NOTIFY storageChanged)
    /*!
     * \brief Sets the parentId property of the underlying BaseModel.
     *
     * \par Access functions:
     * <TABLE><TR><TD>quint64</TD><TD>parentId() const</TD></TR><TR><TD>void</TD><TD>setParentId(quint64 nParentId)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>parentIdChanged(quint64 parentId)</TD></TR></TABLE>
     */
    Q_PROPERTY(quint64 parentId READ parentId WRITE setParentId NOTIFY parentIdChanged)
    /*!
     * \brief The property the model items should be sorted by.
     *
     * \par Access functions:
     * <TABLE><TR><TD>FuotenEnums::SortingRole</TD><TD>sortingRole() const</TD></TR><TR><TD>void</TD><TD>setSortingRole(FuotenEnums::SortingRole nSortingRole)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>sortingRoleChanged(FuotenEnums::SortingRole sortingRole)</TD></TR></TABLE>
     */
    Q_PROPERTY(Fuoten::FuotenEnums::SortingRole sortingRole READ sortingRole WRITE setSortingRole NOTIFY sortingRoleChanged)
    /*!
     * \brief Defines the sort order.
     *
     * \par Access functions:
     * <TABLE><TR><TD>Qt::SortOrder</TD><TD>sortOrder() const</TD></TR><TR><TD>void</TD><TD>setSortOrder(Qt::SortOrder nSortOrder)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>sortOrderChanged(Qt::SortOrder sortOrder)</TD></TR></TABLE>
     */
    Q_PROPERTY(Qt::SortOrder sortOrder READ sortOrder WRITE setSortOrder NOTIFY sortOrderChanged)
    /*!
     * \brief Search string to filter the model for.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>search() const</TD></TR><TR><TD>void</TD><TD>setSearch(const QString &nSearch)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>searchChanged(const QString &search)</TD></TR></TABLE>
     */
    Q_PROPERTY(QString search READ search WRITE setSearch NOTIFY searchChanged)
    /*!
     * \brief When true, items with no unread items will be hidden.
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>hideRead() const</TD></TR><TR><TD>void</TD><TD>setHideRead(bool nHideRead)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>hideReadChanged(bool hideRead)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool hideRead READ hideRead WRITE setHideRead NOTIFY hideReadChanged)
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
    Q_ENUM(Fuoten::FuotenEnums::SortingRole)
#else
    Q_ENUMS(Fuoten::FuotenEnums::SortingRole)
#endif
public:
    /*!
     * \brief Constructs a new BaseFilterModel object.
     *
     * QSortFilterProxyModel::isSortLocaleAware will be set to true.
     */
    explicit BaseFilterModel(QObject *parent = nullptr);

    /*!
     * \brief Deconstructs the BaseFilterModel object.
     */
    ~BaseFilterModel();

    virtual bool inOperation() const = 0;
    virtual AbstractStorage *storage() const = 0;
    virtual quint64 parentId() const = 0;
    FuotenEnums::SortingRole sortingRole() const;
    Qt::SortOrder sortOrder() const;
    QString search() const;
    bool hideRead() const;


    virtual void setStorage(AbstractStorage *nStorage) = 0;
    virtual void setParentId(quint64 nParentId) = 0;
    void setSortingRole(FuotenEnums::SortingRole nSortingRole);
    void setSortOrder(Qt::SortOrder nSortOrder);
    void setSearch(const QString &nSearch);
    void setHideRead(bool nHideRead);

    Q_INVOKABLE virtual void load() = 0;

Q_SIGNALS:
    void inOperationChanged(bool inOperation);
    void storageChanged(AbstractStorage *storage);
    void parentIdChanged(quint64 parentId);
    void sortingRoleChanged(FuotenEnums::SortingRole sortingRole);
    void sortOrderChanged(Qt::SortOrder sortOrder);
    void searchChanged(const QString &search);
    void hideReadChanged(bool hideRead);

protected:
    const QScopedPointer<BaseFilterModelPrivate> d_ptr;
    BaseFilterModel(BaseFilterModelPrivate &dd, QObject *parent = nullptr);

private:
    Q_DISABLE_COPY(BaseFilterModel)
    Q_DECLARE_PRIVATE(BaseFilterModel)

};

}


#endif // FUOTENBASEFILTERMODEL_H
