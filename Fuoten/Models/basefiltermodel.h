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
 * \brief Abstract base filter model for other filter models.
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
     * <TABLE><TR><TD>qint64</TD><TD>parentId() const</TD></TR><TR><TD>void</TD><TD>setParentId(qint64 nParentId)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>parentIdChanged(qint64 parentId)</TD></TR></TABLE>
     */
    Q_PROPERTY(qint64 parentId READ parentId WRITE setParentId NOTIFY parentIdChanged)
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
     * \brief When \c true, items with no unread items will be hidden.
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>hideRead() const</TD></TR><TR><TD>void</TD><TD>setHideRead(bool nHideRead)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>hideReadChanged(bool hideRead)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool hideRead READ hideRead WRITE setHideRead NOTIFY hideReadChanged)
    /*!
     * \brief Stores the parent ID as double to make it accesseable from QML.
     *
     * \par Access functions:
     * <TABLE><TR><TD>double</TD><TD>doubleParentId() const</TD></TR><TR><TD>void</TD><TD>setDoubleParentId(double nDoubleParentId)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>doubleParentIdChanged(double doubleParentId)</TD></TR></TABLE>
     */
    Q_PROPERTY(double doubleParentId READ doubleParentId WRITE setDoubleParentId NOTIFY doubleParentIdChanged)
    /*!
     * \brief This property holds \c true after the source model has initially loaded its data.
     *
     * \par Access functions:
     * bool loaded() const
     *
     * \par Notifier signal:
     * void loadedChanged(bool loaded)
     */
    Q_PROPERTY(bool loaded READ loaded NOTIFY loadedChanged)
public:
    /*!
     * \brief Constructs a new base filter model with the given \a parent.
     *
     * QSortFilterProxyModel::isSortLocaleAware will be set to true.
     */
    explicit BaseFilterModel(QObject *parent = nullptr);

    /*!
     * \brief Deconstructs the %BaseFilterModel object.
     */
    ~BaseFilterModel() override;

    /*!
     * \brief Returns true while the underlying model is in operational mode.
     *
     * Reimplement this in a derived class.
     * \sa inOperation
     */
    virtual bool inOperation() const = 0;

    /*!
     * \brief Returns the local storage that is currently set to the underlying model.
     *
     * Reimplement this in a derived class.
     * \sa storage
     */
    virtual AbstractStorage *storage() const = 0;

    /*!
     * \brief Returns the parent ID that is currently set to the underlyiing model.
     * Reimplement this in a derived class.
     * \sa parentId
     */
    virtual qint64 parentId() const = 0;

    /*!
     * \brief Returns the currently set sorting role.
     * \sa sortingRole
     */
    virtual FuotenEnums::SortingRole sortingRole() const;

    /*!
     * \brief Returns the currently set sort order.
     * \sa sortOrder
     */
    virtual Qt::SortOrder sortOrder() const;

    /*!
     * \brief Returns the currently set serach string.
     * \sa setSearch
     */
    QString search() const;

    /*!
     * \brief Returns true if model items with no unread items or unread articles should be hidden.
     */
    bool hideRead() const;

    /*!
     * \brief Returns \c true if the source model has initially loaded its data.
     */
    virtual bool loaded() const = 0;

    /*!
     * \brief Sets the local storage handler for the underlying model.
     * \sa storage
     */
    virtual void setStorage(AbstractStorage *nStorage) = 0;

    /*!
     * \brief Sets the parent ID for the underlying model.
     * \sa parentId
     */
    virtual void setParentId(qint64 nParentId) = 0;

    /*!
     * \brief Sets the sorting role.
     * \sa sortingRole
     */
    virtual void setSortingRole(FuotenEnums::SortingRole nSortingRole);

    /*!
     * \brief Sets the sort order.
     * \sa sortOrder
     */
    virtual void setSortOrder(Qt::SortOrder nSortOrder);

    /*!
     * \brief Sets the search string.
     * \sa search find
     */
    void setSearch(const QString &nSearch);

    /*!
     * \brief Set this to true to hide items with no unread content or read articles.
     * \sa hideRead
     */
    void setHideRead(bool nHideRead);

    /*!
     * \brief Loads the data in the underlying model.
     *
     * Reimplement this in a subclass and call the underlying model's BaseModel::load() function.
     * If \a locale is not an empty string, the default locale will be set to the defined locale.
     * \sa QLocale::setDefault()
     */
    Q_INVOKABLE virtual void load(const QString &locale = QString()) = 0;

    /*!
     * \brief Reloads the complete underlying model.
     *
     * Reimplement this in a subclass and call the underlying model's BaseModel::reload() function.
     * If \a localse is not an empty string, the default local will be set to the defined locale.
     * \sa QLocale::setDefault()
     */
    Q_INVOKABLE virtual void reload(const QString &locale = QString()) = 0;

    /*!
     * \brief Getter function for the \link BaseModel::doubleParentId doubleParentId \endlink property.
     * \sa setDoubleParentId(), doubleParentIdChanged()
     */
    double doubleParentId() const;

    /*!
     * \brief Setter function for the \link BaseModel::doubleParentId doubleParentId \endlink property.
     * \sa doubleParentId(), doubleParentIdChanged()
     */
    void setDoubleParentId(double nDoubleParentId);

Q_SIGNALS:
    /*!
     * \brief This signal is emitted when the operational state of the underlying model changes.
     * \sa inOperation
     */
    void inOperationChanged(bool inOperation);

    /*!
     * \brief This signal is emitted when the pointer to the local storage handler changes in the underlying model.
     * \sa storage
     */
    void storageChanged(Fuoten::AbstractStorage *storage);

    /*!
     * \brief This signal is emitted when the parent ID property changed in the underlying model.
     * \sa parentId
     */
    void parentIdChanged(qint64 parentId);

    /*!
     * \brief This signal is emitted if the sorting role changes.
     * \sa sortingRole
     */
    void sortingRoleChanged(FuotenEnums::SortingRole sortingRole);

    /*!
     * \brief This signal is emitted if the sort order changes.
     * \sa sortOrder
     */
    void sortOrderChanged(Qt::SortOrder sortOrder);

    /*!
     * \brief This signal is emitted if the search string changes.
     * \sa search
     */
    void searchChanged(const QString &search);

    /*!
     * \brief This signal is emitted if the hideRead property changes.
     * \sa hideRead
     */
    void hideReadChanged(bool hideRead);

    /*!
     * \brief This signal is emitted if the loaded property changes.
     * \sa loaded
     */
    void loadedChanged(bool loaded);

    /*!
     * \brief This signal is emitted if the doubleParentId property changes.
     * \sa doubleParentId
     */
    void doubleParentIdChanged(double doubleParentId);

protected:
    const QScopedPointer<BaseFilterModelPrivate> d_ptr;
    BaseFilterModel(BaseFilterModelPrivate &dd, QObject *parent = nullptr);

    /*!
     * \brief Returns \c true if the string set by setSearch() is part of \a str.
     * \sa setSearch
     */
    bool find(const QString &str) const;

private:
    Q_DISABLE_COPY(BaseFilterModel)
    Q_DISABLE_MOVE(BaseFilterModel)
    Q_DECLARE_PRIVATE(BaseFilterModel)
};

}


#endif // FUOTENBASEFILTERMODEL_H
