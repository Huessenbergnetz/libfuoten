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

#ifndef FUOTENBASEMODEL_H
#define FUOTENBASEMODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include "../fuoten_global.h"

namespace Fuoten {

class BaseModelPrivate;
class AbstractStorage;

/*!
 * \brief Abstract base class for all data models.
 *
 * \headerfile "" <Fuoten/Models/BaseModel>
 */
class FUOTENSHARED_EXPORT BaseModel : public QAbstractItemModel
{
    Q_OBJECT
    /*!
     * \brief Returns true while the model is loading Data.
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
public:
    /*!
     * \brief Constructs a new BaseModel object.
     */
    BaseModel(QObject *parent = nullptr);

    /*!
     * \brief Deconstructs the BaseModel object.
     */
    ~BaseModel();

    /*!
     * \brief Returns the model index of the item identified by database ID.
     *
     * The default implementation returns an invalid QModelIndex. Reimplement this function
     * in a subclass.
     */
    virtual QModelIndex findByID(quint64 id) const;


    bool inOperation() const;
    AbstractStorage *storage() const;

    void setStorage(AbstractStorage *nStorage);

public Q_SLOTS:
    /*!
     * \brief Loads the model data.
     *
     * Reimplement this in a subclass. Do only load the data, if loaded() returns false and
     * set setLoaded() to true after the data has been loaded. All later changes, after the model
     * has loaded the initial data, should be done by signals and slots without reloading the full
     * model.
     *
     * Additionally you could use the inOperation property in this function to indicate, that the model
     * is loading its data.
     */
    virtual void load() = 0;

Q_SIGNALS:
    void inOperationChanged(bool inOperation);
    void storageChanged(AbstractStorage *storage);

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
     * The default implementation does nothing.
     */
    virtual void handleStorageChanged();

    /*!
     * \brief Returns true if the model has initially loaded its data.
     *
     * \sa setLoaded()
     */
    bool loaded() const;

    /*!
     * \brief Set this to true if the model has initially loaded its data.
     *
     * In the implementation of a BaseModel this should be set in the implementation of the load() function,
     * after the initial data has been loaded.
     *
     * \sa loaded()
     */
    void setLoaded(bool loaded);

private:
    Q_DISABLE_COPY(BaseModel)
    Q_DECLARE_PRIVATE(BaseModel)

};

}


#endif // FUOTENBASEMODEL_H
