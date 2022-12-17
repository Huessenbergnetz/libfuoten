/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENGETITEMS_H
#define FUOTENGETITEMS_H

#include <QObject>
#include "component.h"
#include "../fuoten.h"
#include "../fuoten_global.h"


namespace Fuoten {

class GetItemsPrivate;

/*!
 * \brief Requests items/articles from the News App API.
 *
 * To request the items/articles, the Component::configuration property has to be set to a valid AbstractConfiguration object. After setting it,
 * call execute() to perform the API request.
 *
 * If a valid AbstractStorage object is set to the Component::storage property, AbstractStorage::itemsRequested will be called in the successCallback()
 * to save the requested items in the local storage. If the request succeeded, the Component::succeeded() signal will be emitted, containing the JSON api
 * reply.
 *
 * If something failed, Component::failed() will be emitted and the Component::error property will contain a valid pointer to an Error object.
 *
 * \par Mandatory properties
 * Component::configuration
 *
 * \par API route
 * /items
 *
 * \par Method
 * GET
 *
 * \sa Synchronizer
 * \headerfile "" <Fuoten/API/GetItems>
 */
class FUOTENSHARED_EXPORT GetItems : public Component
{
    Q_OBJECT
    /*!
     * \brief The number of items/articles that should be returned.
     *
     * Defaults to \c -1, \c -1 returns all items. This property can not be changed while Component::inOperation() returns \c true.
     *
     * \par Access functions:
     * <TABLE><TR><TD>int</TD><TD>batchSize() const</TD></TR><TR><TD>void</TD><TD>setBatchSize(int nBatchSize)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>batchSizeChanged(int batchSize)</TD></TR></TABLE>
     */
    Q_PROPERTY(int batchSize READ batchSize WRITE setBatchSize NOTIFY batchSizeChanged)
    /*!
     * \brief Only return older (lower than equal that ID) items.
     *
     * Defaults to \c 0. This property can not be changed while Component::inOperation() returns \c true.
     *
     * \par Access functions:
     * <TABLE><TR><TD>qint64</TD><TD>offset() const</TD></TR><TR><TD>void</TD><TD>setOffset(qint64 nOffset)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>offsetChanged(qint64 offset)</TD></TR></TABLE>
     */
    Q_PROPERTY(qint64 offset READ offset WRITE setOffset NOTIFY offsetChanged)
    /*!
     * \brief Type of the query.
     *
     * Defines for which parent items/articles should be queried. Defaults to FuotenEnums::All.
     *
     * \par Possible values:
     * \li FuotenEnums::Feed
     * \li FuotenEnums::Folder
     * \li FuotenEnums::Starred
     *
     * This property can not be changed while Component::inOperation() returns \c true.
     *
     * \par Access functions:
     * <TABLE><TR><TD>FuotenEnums::Type</TD><TD>type() const</TD></TR><TR><TD>void</TD><TD>setType(FuotenEnums::Type nType)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>typeChanged(FuotenEnums::Type type)</TD></TR></TABLE>
     */
    Q_PROPERTY(Fuoten::FuotenEnums::Type type READ type WRITE setType NOTIFY typeChanged)
    /*!
     * \brief ID of the folder or feed, 0 for starred and all.
     *
     * Defaults to \c 0. This property can not be changed while Component::inOperation() returns \c true.
     *
     * \par Access functions:
     * <TABLE><TR><TD>qint64</TD><TD>parentId() const</TD></TR><TR><TD>void</TD><TD>setParentId(qint64 nParentId)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>parentIdChanged(qint64 parentId)</TD></TR></TABLE>
     */
    Q_PROPERTY(qint64 parentId READ parentId WRITE setParentId NOTIFY parentIdChanged)
    /*!
     * \brief If true, it returns all items, false returns only unread items.
     *
     * This property can not be changed while Component::inOperation() returns \c true.
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>getRead() const</TD></TR><TR><TD>void</TD><TD>setGetRead(bool nGetRead)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>getReadChanged(bool getRead)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool getRead READ getRead WRITE setGetRead NOTIFY getReadChanged)
    /*!
     * \brief If true, it reverses the sort order.
     *
     * This property can not be changed while Component::inOperation() returns \c true.
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>oldestFirst() const</TD></TR><TR><TD>void</TD><TD>setOldestFirst(bool nOldestFirst)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>oldestFirstChanged(bool oldestFirst)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool oldestFirst READ oldestFirst WRITE setOldestFirst NOTIFY oldestFirstChanged)
public:
    /*!
     * \brief Constructs an API request object with the given \a parent to query items from the remote server.
     */
    explicit GetItems(QObject *parent = nullptr);

    /*!
     * \brief Constructs an API request object with the given parameters and \a parent to query items from the remote server.
     */
    GetItems(int batchSize, qint64 offset, FuotenEnums::Type type, qint64 parentId, bool getRead, bool oldestFirst, QObject *parent = nullptr);

    /*!
     * \brief Destroys the %GetItems object.
     */
    ~GetItems() override;

    /*!
     * \brief Getter function for the \link GetItems::batchSize batchSize \endlink property.
     * \sa GetItems::setBatchSize(), GetItems::batchSizeChanged()
     */
    int batchSize() const;
    /*!
     * \brief Getter function for the \link GetItems::offset offset \endlink property.
     * \sa GetItems::setOffset(), GetItems::offsetChanged()
     */
    qint64 offset() const;
    /*!
     * \brief Getter function for the \link GetItems::type type \endlink property.
     * \sa GetItems::setType(), GetItems::typeChanged()
     */
    FuotenEnums::Type type() const;
    /*!
     * \brief Getter function for the \link GetItems::parentId parentId \endlink property.
     * \sa GetItems::setParentId(), GetItems::parentIdChanged()
     */
    qint64 parentId() const;
    /*!
     * \brief Getter function for the \link GetItems::getRead getRead \endlink property.
     * \sa GetItems::setGetRead(), GetItems::getReadChanged()
     */
    bool getRead() const;
    /*!
     * \brief Getter function for the \link GetItems::oldestFirst oldestFirst \endlink property.
     * \sa GetItems::setOldestFirst(), GetItems::oldestFirstChanged()
     */
    bool oldestFirst() const;

    /*!
     * \brief Setter function for the \link GetItems::batchSize batchSize \endlink property.
     * Emits the batchSizeChanged() signal if \a nBatchSize is not equal to the stored value.
     * \sa GetItems::batchSize(), GetItems::batchSizeChanged()
     */
    void setBatchSize(int nBatchSize);
    /*!
     * \brief Setter function for the \link GetItems::offset offset \endlink property.
     * Emits the offsetChanged() signal if \a nOffset is not equal to the stored value.
     * \sa GetItems::offset(), GetItems::offsetChanged()
     */
    void setOffset(qint64 nOffset);
    /*!
     * \brief Setter function for the \link GetItems::type type \endlink property.
     * Emits the typeChanged() signal if \a nType is not equal to the stored value.
     * \sa GetItems::type(), GetItems::typeChanged()
     */
    void setType(FuotenEnums::Type nType);
    /*!
     * \brief Setter function for the \link GetItems::parentId parentId \endlink property.
     * Emits the parentIdChanged() signal if \a nParentId is not equal to the stored value.
     * \sa GetItems::parentId(), GetItems::parentIdChanged()
     */
    void setParentId(qint64 nParentId);
    /*!
     * \brief Setter function for the \link GetItems::getRead getRead \endlink property.
     * Emits the getReadChanged() signal if \a nGetRead is not equal to the stored value.
     * \sa GetItems::getRead(), GetItems::getReadChanged()
     */
    void setGetRead(bool nGetRead);
    /*!
     * \brief Setter function for the \link GetItems::oldestFirst oldestFirst \endlink property.
     * Emits the oldestFirstChanged() signal if \a nOldestFirst is not equal to the stored value.
     * \sa GetItems::oldestFirst(), GetItems::oldestFirstChanged()
     */
    void setOldestFirst(bool nOldestFirst);



    /*!
     * \brief Executes the API request.
     *
     * To perform a successful API request, Component::configuration has to be set to a valid AbstractConfiguration object.
     *
     * Execution will not run if Component::inOperation returns \c true and will itself set that property to \c true when start to perform
     * the request.
     */
    Q_INVOKABLE void execute() override;


Q_SIGNALS:
    /*!
     * \brief This is emitted if the value of the \link GetItems::batchSize batchSize \endlink property changes.
     * \sa GetItems::batchSize(), GetItems::setBatchSize()
     */
    void batchSizeChanged(int batchSize);
    /*!
     * \brief This is emitted if the value of the \link GetItems::offset offset \endlink property changes.
     * \sa GetItems::offset(), GetItems::setOffset()
     */
    void offsetChanged(qint64 offset);
    /*!
     * \brief This is emitted if the value of the \link GetItems::type type \endlink property changes.
     * \sa GetItems::type(), GetItems::setType()
     */
    void typeChanged(FuotenEnums::Type type);
    /*!
     * \brief This is emitted if the value of the \link GetItems::parentId parentId \endlink property changes.
     * \sa GetItems::parentId(), GetItems::setParentId()
     */
    void parentIdChanged(qint64 parentId);
    /*!
     * \brief This is emitted if the value of the \link GetItems::getRead getRead \endlink property changes.
     * \sa GetItems::getRead(), GetItems::setGetRead()
     */
    void getReadChanged(bool getRead);
    /*!
     * \brief This is emitted if the value of the \link GetItems::oldestFirst oldestFirst \endlink property changes.
     * \sa GetItems::oldestFirst(), GetItems::setOldestFirst()
     */
    void oldestFirstChanged(bool oldestFirst);

protected:
    GetItems(GetItemsPrivate &dd, QObject *parent = nullptr);

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
    Q_DISABLE_COPY(GetItems)
    Q_DECLARE_PRIVATE(GetItems)
};

}


#endif // FUOTENGETITEMS_H
