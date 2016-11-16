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

#ifndef FUOTENGETUPDATEDITEMS_H
#define FUOTENGETUPDATEDITEMS_H

#include <QObject>
#include <QDateTime>
#include "component.h"
#include "../fuoten.h"
#include "../fuoten_global.h"

namespace Fuoten {

class GetUpdatedItemsPrivate;

/*!
 * \brief Requests updated items/articles from the News App API.
 *
 * To request updated items/articles, the Component::configuration property has to be set to a valid AbstractConfiguration object and a valid timestamp
 * has to be set to GetUpdatedItems::lastModified. After setting the mandatory properties, call execute() to perform the API request.
 *
 * If a valid AbstractStorage object is set to the Component::storage property, AbstractStorage::itemsRequested will be called in the successCallback()
 * to save the requested items in the local storage. If the request succeeded, the Component::succeeded() signal will be emitted, containing the JSON api
 * reply.
 *
 * If something failed, Component::failed() will be emitted and the Component::error property will contain a valid pointer to an Error object.
 *
 * \par Mandatory properties
 * Component::configuration, GetUpdatedItems::lastModified
 *
 * \par API route
 * /items/updated
 *
 * \par Method
 * GET
 *
 * \sa Synchronizer
 * \headerfile "" <Fuoten/API/GetUpdatedItems>
 */
class FUOTENSHARED_EXPORT GetUpdatedItems : public Component
{
    Q_OBJECT
    /*!
     * \brief Return only items with a lastModified timestamp >= this one.
     *
     * This may also return already existing items whose read or starred status has been changed.
     * This property can not be changed while Component::inOperation() returns \c true.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QDateTime</TD><TD>lastModified() const</TD></TR><TR><TD>void</TD><TD>setLastModified(const QDateTime &nLastModified)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>lastModifiedChanged(const QDateTime &lastModified)</TD></TR></TABLE>
     */
    Q_PROPERTY(QDateTime lastModified READ lastModified WRITE setLastModified NOTIFY lastModifiedChanged)
    /*!
     * \brief Type of the query.
     *
     * Defaults to FuotenEnums::All. This property can not be changed while Component::inOperation() returns \c true.
     *
     * \par Access functions:
     * <TABLE><TR><TD>FuotenEnums::Type</TD><TD>type() const</TD></TR><TR><TD>void</TD><TD>setType(FuotenEnums::Type nType)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>typeChanged(FuotenEnums::Type type)</TD></TR></TABLE>
     */
    Q_PROPERTY(Fuoten::FuotenEnums::Type type READ type WRITE setType NOTIFY typeChanged)
    /*!
     * \brief ID of the folder or feed, use 0 for Starred and All.
     *
     * Defaults to \c 0. This property can not be changed while Component::inOperation() returns \c true.
     *
     * \par Access functions:
     * <TABLE><TR><TD>qint64</TD><TD>parentId() const</TD></TR><TR><TD>void</TD><TD>setParentId(qint64 nParentId)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>parentIdChanged(qint64 parentId)</TD></TR></TABLE>
     */
    Q_PROPERTY(qint64 parentId READ parentId WRITE setParentId NOTIFY parentIdChanged)
public:
    /*!
     * \brief Constructs a new GetUpdatedItems object with default values and the given \a parent.
     */
    explicit GetUpdatedItems(QObject *parent = nullptr);

    /*!
     * \brief Constructs a new GetUpdatedItems object with the given arguments and \a parent.
     */
    GetUpdatedItems(const QDateTime &lastModified, FuotenEnums::Type type, qint64 parentId, QObject *parent = nullptr);

    /*!
     * \brief Getter function for the \link GetUpdatedItems::lastModified lastModified \endlink property.
     * \sa GetUpdatedItems::setLastModified(), GetUpdatedItems::lastModifiedChanged()
     */
    QDateTime lastModified() const;
    /*!
     * \brief Getter function for the \link GetUpdatedItems::type type \endlink property.
     * \sa GetUpdatedItems::setType(), GetUpdatedItems::typeChanged()
     */
    FuotenEnums::Type type() const;
    /*!
     * \brief Getter function for the \link GetUpdatedItems::parentId parentId \endlink property.
     * \sa GetUpdatedItems::setParentId(), GetUpdatedItems::parentIdChanged()
     */
    qint64 parentId() const;

    /*!
     * \brief Setter function for the \link GetUpdatedItems::lastModified lastModified \endlink property.
     * Emits the lastModifiedChanged() signal if \a nLastModified is not equal to the stored value.
     * \sa GetUpdatedItems::lastModified(), GetUpdatedItems::lastModifiedChanged()
     */
    void setLastModified(const QDateTime &nLastModified);
    /*!
     * \brief Setter function for the \link GetUpdatedItems::type type \endlink property.
     * Emits the typeChanged() signal if \a nType is not equal to the stored value.
     * \sa GetUpdatedItems::type(), GetUpdatedItems::typeChanged()
     */
    void setType(FuotenEnums::Type nType);
    /*!
     * \brief Setter function for the \link GetUpdatedItems::parentId parentId \endlink property.
     * Emits the parentIdChanged() signal if \a nParentId is not equal to the stored value.
     * \sa GetUpdatedItems::parentId(), GetUpdatedItems::parentIdChanged()
     */
    void setParentId(qint64 nParentId);


    /*!
     * \brief Executes the API request.
     *
     * To perform a successful API request, Component::configuration has to be set to a valid AbstractConfiguration object and
     * GetUpdatedItems::lastModified has to contain a valid time.
     *
     * Execution will not run if Component::inOperation returns \c true and will itself set that property to \c true when start to perform
     * the request.
     */
    Q_INVOKABLE void execute() override;


Q_SIGNALS:
    /*!
     * \brief This is emitted if the value of the \link GetUpdatedItems::lastModified lastModified \endlink property changes.
     * \sa GetUpdatedItems::lastModified(), GetUpdatedItems::setLastModified()
     */
    void lastModifiedChanged(const QDateTime &lastModified);
    /*!
     * \brief This is emitted if the value of the \link GetUpdatedItems::type type \endlink property changes.
     * \sa GetUpdatedItems::type(), GetUpdatedItems::setType()
     */
    void typeChanged(FuotenEnums::Type type);
    /*!
     * \brief This is emitted if the value of the \link GetUpdatedItems::parentId parentId \endlink property changes.
     * \sa GetUpdatedItems::parentId(), GetUpdatedItems::setParentId()
     */
    void parentIdChanged(qint64 parentId);

protected:
    GetUpdatedItems(GetUpdatedItemsPrivate &dd, QObject *parent = nullptr);

    /*!
     * \brief Finishes the the operation if the request was successful.
     *
     * If Component::storage points to a valid object, it will use AbstractStorage::itemsRequested() to store, update and delete the
     * items in the local storage according to the server reply. Afterwards it will set Component::inOperation to false and will emit
     * the Component::succeeded() signal.
     */
    void successCallback() override;

    /*!
     * \brief Checks for an \a items array in the JSON API reply.
     *
     * Will at first perform the checks from Component::checkOutput() and will than check if the \a items array is present.
     * Will \b not check if the array is empty.
     */
    bool checkOutput() override;

    /*!
     * \brief Checks for valid input values.
     *
     * Will at first perfrom the checks from Component::checkInput() and will than check if the values of the properties are
     * in valid ranges.
     */
    bool checkInput() override;

private:
    Q_DISABLE_COPY(GetUpdatedItems)
    Q_DECLARE_PRIVATE(GetUpdatedItems)

};

}


#endif // FUOTENGETUPDATEDITEMS_H
