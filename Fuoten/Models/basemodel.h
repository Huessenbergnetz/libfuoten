/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENBASEMODEL_H
#define FUOTENBASEMODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include "../fuoten_global.h"
#include "../fuoten.h"
#include "fuoten_export.h"

namespace Fuoten {

class BaseModelPrivate;
class AbstractStorage;

/*!
 * \brief Abstract base class for all data models.
 *
 * \headerfile "" <Fuoten/Models/BaseModel>
 */
class FUOTEN_EXPORT BaseModel : public QAbstractItemModel
{
    Q_OBJECT
    /*!
     * \brief Returns true while the model is loading data.
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>inOperation() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>inOperationChanged(bool inOperation)</TD></TR></TABLE>
     *
     * \sa setInOperation()
     */
    Q_PROPERTY(bool inOperation READ inOperation WRITE setInOperation NOTIFY inOperationChanged)
    /*!
     * \brief Pointer to a class derived from AbstractStorage.
     *
     * \par Access functions:
     * <TABLE><TR><TD>AbstractStorage*</TD><TD>storage() const</TD></TR><TR><TD>void</TD><TD>setStorage(AbstractStorage *nStorage)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>storageChanged(AbstractStorage *storage)</TD></TR></TABLE>
     */
    Q_PROPERTY(Fuoten::AbstractStorage *storage READ storage WRITE setStorage NOTIFY storageChanged)
    /*!
     * \brief Sets the parent database ID to load feeds/items for.
     *
     * If used on a model presenting feeds, this defines the folder, the feed belongs to. If used on a model
     * presenting items, this defines the feed the items belong to.
     *
     * \par Access functions:
     * <TABLE><TR><TD>qint64</TD><TD>parentId() const</TD></TR><TR><TD>void</TD><TD>setParentId(qint64 nParentId)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>parentIdChanged(qint64 parentId)</TD></TR></TABLE>
     */
    Q_PROPERTY(qint64 parentId READ parentId WRITE setParentId NOTIFY parentIdChanged)
    /*!
     * \brief Stores the parent ID as double to make it accesseable from QML.
     *
     * This uses the same data as parentId, but converts it between double and qint64. This is a convenience property for use in QML
     * that does not support 64bit integers. As JavaScript and JSON are storing number values according to IEEE 754, there might happen
     * an overflow, if setting values greater than the limit of qint64. Under normal circumstances we simply assume, that there will hardly
     * be greater database IDs than what fits in qint64.
     *
     * \par Access functions:
     * <TABLE><TR><TD>double</TD><TD>doubleParentId() const</TD></TR><TR><TD>void</TD><TD>setDoubleParentId(double nDoubleParentId)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>doubleParentIdChanged(double doubleParentId)</TD></TR></TABLE>
     */
    Q_PROPERTY(double doubleParentId READ doubleParentId WRITE setDoubleParentId NOTIFY doubleParentIdChanged)
    /*!
     * \brief The role/value used to sort the result.
     *
     * \par Access functions:
     * <TABLE><TR><TD>FuotenEnums::SortingRole</TD><TD>sortingRole() const</TD></TR><TR><TD>void</TD><TD>setSortingRole(FuotenEnums::SortingRole nSortingRole)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>sortingRoleChanged(FuotenEnums::SortingRole sortingRole)</TD></TR></TABLE>
     */
    Q_PROPERTY(Fuoten::FuotenEnums::SortingRole sortingRole READ sortingRole WRITE setSortingRole NOTIFY sortingRoleChanged)
    /*!
     * \brief The sorting order.
     *
     * \par Access functions:
     * <TABLE><TR><TD>Qt::SortOrder</TD><TD>sortOrder() const</TD></TR><TR><TD>void</TD><TD>setSortOrder(Qt::SortOrder nSortOrder)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>sortOrderChanged(Qt::SortOrder sortOrder)</TD></TR></TABLE>
     */
    Q_PROPERTY(Qt::SortOrder sortOrder READ sortOrder WRITE setSortOrder NOTIFY sortOrderChanged)
    /*!
     * \brief If true, only unread articles, feeds or folders are returned.
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>unreadOnly() const</TD></TR><TR><TD>void</TD><TD>setUnreadOnly(bool nUnreadOnly)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>unreadOnlyChanged(bool unreadOnly)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool unreadOnly READ unreadOnly WRITE setUnreadOnly NOTIFY unreadOnlyChanged)
    /*!
     * \brief Limits the result to the specified number of objects.
     *
     * \par Access functions:
     * <TABLE><TR><TD>int</TD><TD>limit() const</TD></TR><TR><TD>void</TD><TD>setLimit(int nLimit)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>limitChanged(int limit)</TD></TR></TABLE>
     */
    Q_PROPERTY(int limit READ limit WRITE setLimit NOTIFY limitChanged)
    /*!
     * \brief This property holds \c true if the model has initially loaded the data.
     *
     * While it is \c true, load() will return immediately without loading data. Use reload() instead.
     * When creating a derived class, you should use setLoaded() after your model has initially loaded
     * the data.
     *
     * \par Access functions:
     * void setLoaded(bool loaded)
     * bool loaded() const
     *
     * \par Notifier signal:
     * void loadedChanged(bool loaded)
     */
    Q_PROPERTY(bool loaded READ loaded NOTIFY loadedChanged)
public:
    /*!
     * \brief Constructs a new BaseModel object.
     */
    explicit BaseModel(QObject *parent = nullptr);

    /*!
     * \brief Deconstructs the %BaseModel object.
     */
    ~BaseModel() override;

    /*!
     * \brief Returns the model index of the item identified by database ID.
     *
     * The default implementation returns an invalid QModelIndex. Reimplement this function
     * in a subclass.
     */
    virtual QModelIndex findByID(qint64 id) const;

    /*!
     * \brief Returns a hash-table containing IDs and their model index fount in the model.
     *
     * The default implementation return an empty QHash<qint64, QModelIndex>. Reimplement this
     * function in a subclass.
     */
    virtual QHash<qint64, QModelIndex> findByIDs(const IdList &ids) const;


    /*!
     * \brief Returns true while the model is loading data.
     * \sa inOperation
     */
    bool inOperation() const;
    /*!
     * \brief Returns the pointer to the currently set local storage.
     * \sa storage
     */
    AbstractStorage *storage() const;
    /*!
     * \brief Returns the currently set parent ID.
     * \sa parentId
     */
    qint64 parentId() const;
    /*!
     * \brief Returns the currently set parent ID as double.
     * This function is for use in QML, that does not support 64bit integers. It performs a static cast from
     * the saved qint64 parent ID to double.
     * \sa doubleParentId
     */
    double doubleParentId() const;
    /*!
     * \brief Getter function for the \link BaseModel::sortingRole sortingRole \endlink property.
     * \sa BaseModel::setSortingRole(), BaseModel::sortingRoleChanged()
     */
    FuotenEnums::SortingRole sortingRole() const;
    /*!
     * \brief Getter function for the \link BaseModel::sortOrder sortOrder \endlink property.
     * \sa BaseModel::setSortOrder(), BaseModel::sortOrderChanged()
     */
    Qt::SortOrder sortOrder() const;
    /*!
     * \brief Getter function for the \link BaseModel::unreadOnly unreadOnly \endlink property.
     * \sa BaseModel::setUnreadOnly(), BaseModel::unreadOnlyChanged()
     */
    bool unreadOnly() const;
    /*!
     * \brief Getter function for the \link BaseModel::limit limit \endlink property.
     * \sa BaseModel::setLimit(), BaseModel::limitChanged()
     */
    int limit() const;
    /*!
     * \brief Getter function for the \link BaseModel::loaded loaded \endlink property.     *
     * \sa setLoaded(), loadedChanged()
     */
    bool loaded() const;



    /*!
     * \brief Sets the pointer to the local storage handler.
     * \sa storage
     */
    void setStorage(AbstractStorage *nStorage);
    /*!
     * \brief Set the parent ID.
     * \sa parentId
     */
    void setParentId(qint64 nParentId);
    /*!
     * \brief Sets the parent ID via a double.
     * This function is for use in QML, that does not support 64bit integers. It performs a static cast from
     * the double to a qint64 to save it.
     * \sa doubleParentId
     */
    void setDoubleParentId(double nDoubleParentId);
    /*!
     * \brief Setter function for the \link BaseModel::sortingRole sortingRole \endlink property.
     * Emits the sortingRoleChanged() signal if \a nSortingRole is not equal to the stored value.
     * \sa BaseModel::sortingRole(), BaseModel::sortingRoleChanged()
     */
    void setSortingRole(FuotenEnums::SortingRole nSortingRole);
    /*!
     * \brief Setter function for the \link BaseModel::sortOrder sortOrder \endlink property.
     * Emits the sortOrderChanged() signal if \a nSortOrder is not equal to the stored value.
     * \sa BaseModel::sortOrder(), BaseModel::sortOrderChanged()
     */
    void setSortOrder(Qt::SortOrder nSortOrder);
    /*!
     * \brief Setter function for the \link BaseModel::unreadOnly unreadOnly \endlink property.
     * Emits the unreadOnlyChanged() signal if \a nUnreadOnly is not equal to the stored value.
     * \sa BaseModel::unreadOnly(), BaseModel::unreadOnlyChanged()
     */
    void setUnreadOnly(bool nUnreadOnly);
    /*!
     * \brief Setter function for the \link BaseModel::limit limit \endlink property.
     * Emits the limitChanged() signal if \a nLimit is not equal to the stored value.
     * \sa BaseModel::limit(), BaseModel::limitChanged()
     */
    void setLimit(int nLimit);

public Q_SLOTS:
    /*!
     * \brief Loads the model data.
     *
     * Reimplement this in a subclass. Do only load the data, if loaded() returns false and
     * set setLoaded() to true after the data has been loaded. All later changes, after the model
     * has loaded the initial data, should be done by signals and slots without reloading the full
     * model.
     *
     * Additionally you could use the \link BaseModel::inOperation inOperation \endlink property in this function to indicate, that the model
     * is loading its data.
     *
     * \par Example implementation
     *
     * \code{.cpp}
     *
     * void AbstractFolderModel::load()
     * {
     *     if (!storage()) {
     *         return;
     *     }
     *
     *     if (!storage()->ready() || loaded()) {
     *         return;
     *     }
     *
     *     setInOperation(true);
     *
     *     Q_D(AbstractFolderModel);
     *
     *     const QList<Folder*> fs = storage()->getFolders(FuotenEnums::Name, Qt::AscendingOrder);
     *     if (!fs.isEmpty()) {
     *         beginInsertRows(QModelIndex(), 0, fs.count() - 1);
     *         d->folders = fs;
     *         endInsertRows();
     *     }
     *
     *     setLoaded(true);
     *     setInOperation(false);
     * }
     * \endcode
     */
    virtual void load() = 0;

    /*!
     * \brief Reloads the complete model.
     *
     * Will call clear(), will than set \link BaseModel::loaded loaded \endlink to false and will then call load().
     */
    virtual void reload();

Q_SIGNALS:
    /*!
     * \brief This signal is emitted if the operational state of the model changes.
     * \sa inOperatoin
     */
    void inOperationChanged(bool inOperation);
    /*!
     * \brief This signal is emitted if the poiner to the local storage changes.
     * \sa storage
     */
    void storageChanged(Fuoten::AbstractStorage *storage);
    /*!
     * \brief This signal is emitted if the parent ID changes.
     * \sa parentId
     */
    void parentIdChanged(qint64 parentId);
    /*!
     * \brief This signal is emitted if the parent ID changes.
     * \sa doubleParentId
     */
    void doubleParentIdChanged(double doubleParentId);
    /*!
     * \brief This is emitted if the value of the \link BaseModel::sortingRole sortingRole \endlink property changes.
     * \sa BaseModel::sortingRole(), BaseModel::setSortingRole()
     */
    void sortingRoleChanged(FuotenEnums::SortingRole sortingRole);
    /*!
     * \brief This is emitted if the value of the \link BaseModel::sortOrder sortOrder \endlink property changes.
     * \sa BaseModel::sortOrder(), BaseModel::setSortOrder()
     */
    void sortOrderChanged(Qt::SortOrder sortOrder);
    /*!
     * \brief This is emitted if the value of the \link BaseModel::unreadOnly unreadOnly \endlink property changes.
     * \sa BaseModel::unreadOnly(), BaseModel::setUnreadOnly()
     */
    void unreadOnlyChanged(bool unreadOnly);
    /*!
     * \brief This is emitted if the value of the \link BaseModel::limit limit \endlink property changes.
     * \sa BaseModel::limit(), BaseModel::setLimit()
     */
    void limitChanged(int limit);
    /*!
     * \brief This is emitted if the value of the \link BaseModel::loaded loaded \endlink property changes.
     * \sa loaded(), setLoaded()
     */
    void loadedChanged(bool loaded);


protected:
    const QScopedPointer<BaseModelPrivate> d_ptr;
    BaseModel(BaseModelPrivate &dd, QObject *parent = nullptr);

    /*!
     * \brief Sets the inOperation property.
     *
     * \sa inOperation
     */
    void setInOperation(bool nInOperation);

    /*!
     * \brief Will be called by setStorage() whenever the storage changes.
     *
     * The default implementation does nothing. You should use this to connect
     * signals from the AbstractStorage to slots in the model implementation that
     * updates the model data after the local data has been changed.
     *
     * \par Example implementation
     *
     * \code{.cpp}
     * void AbstractFolderModel::handleStorageChanged()
     * {
     *     AbstractStorage *s = storage();
     *     connect(s, &AbstractStorage::readyChanged, this, &AbstractFolderModel::load);
     *     connect(s, &AbstractStorage::requestedFolders, this, &AbstractFolderModel::foldersRequested);
     *     connect(s, &AbstractStorage::renamedFolder, this, &AbstractFolderModel::folderRenamed);
     *     connect(s, &AbstractStorage::createdFolder, this, &AbstractFolderModel::folderCreated);
     *     connect(s, &AbstractStorage::deletedFolder, this, &AbstractFolderModel::folderDeleted);
     *     connect(s, &AbstractStorage::markedReadFolder, this, &AbstractFolderModel::folderMarkedRead);
     * }
     *
     * \endcode
     */
    virtual void handleStorageChanged(AbstractStorage *old);

    /*!
     * \brief Set this to \c true if the model has initially loaded its data.
     *
     * In the reimplementation of a BaseModel this should be set in the implementation of the load() function,
     * after the initial data has been loaded.
     *
     * \sa BaseModel::loaded
     */
    void setLoaded(bool loaded);

    /*!
     * \brief Clears the model and removes all model data.
     */
    virtual void clear() = 0;

private:
    Q_DISABLE_COPY(BaseModel)
    Q_DECLARE_PRIVATE(BaseModel)
};

}


#endif // FUOTENBASEMODEL_H
