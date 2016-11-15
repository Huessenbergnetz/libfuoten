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

#ifndef FUOTENMARKALLITEMSREAD_H
#define FUOTENMARKALLITEMSREAD_H

#include <QObject>
#include "component.h"
#include "../fuoten_global.h"

namespace Fuoten {

class MarkAllItemsReadPrivate;

/*!
 * \brief Marks all articles as read on the News App server.
 *
 * To mark all articles/items as read, you have to provide the newest/highest local available item ID to MarkAllItemsRead::newestItemId and you have
 * to set a valid AbstractConfiguration derived class to Component::configuration to provide authentication credentials and server configuration. After
 * setting the mandatory properties, call execute() to perform the API request.
 *
 * If a valid AbstractStorage object is set to the Component::storage property, AbstractStorage::allItemsMarkedRead() will be called in the successCallback() to
 * update the local storage. If the request succeeded, the MarkAllItemsRead::succeeded() signal will be emitted, containing the \a newestItemId.
 * If something failed, the Component::failed() signal will be emitted and Component::error will contain a valid pointer to an Error object.
 *
 * \par Mandatory properties
 * MarkAllItemsRead::newestItemId, Component::configuration
 *
 * \par API route
 * /items/read
 *
 * \par Method
 * PUT
 *
 * \headerfile "" <Fuoten/API/MarkFeedRead>
 */
class FUOTENSHARED_EXPORT MarkAllItemsRead : public Component
{
    Q_OBJECT
    /*!
     * \brief ID of the newest local available item/article.
     *
     * \par Access functions:
     * <TABLE><TR><TD>qint64</TD><TD>newestItemId() const</TD></TR><TR><TD>void</TD><TD>setNewestItemId(qint64 nNewestItemId)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>newestItemIdChanged(qint64 newestItemId)</TD></TR></TABLE>
     */
    Q_PROPERTY(qint64 newestItemId READ newestItemId WRITE setNewestItemId NOTIFY newestItemIdChanged)
public:
    /*!
     * \brief Constructs a new MarkAllItemsRead object with default values and the given \a parent.
     */
    explicit MarkAllItemsRead(QObject *parent = nullptr);

    /*!
     * \brief Constructs a new MarkAllItemsRead object with the given arguments and \a parent.
     */
    explicit MarkAllItemsRead(qint64 newestItemId, QObject *parent = nullptr);

    /*!
     * \brief Getter function for the \link MarkAllItemsRead::newestItemId newestItemId \endlink property.
     * \sa MarkAllItemsRead::setNewestItemId(), MarkAllItemsRead::newestItemIdChanged()
     */
    qint64 newestItemId() const;

    /*!
     * \brief Setter function for the \link MarkAllItemsRead::newestItemId newestItemId \endlink property.
     * Emits the newestItemIdChanged() signal if \a nNewestItemId is not equal to the stored value.
     * \sa MarkAllItemsRead::newestItemId(), MarkAllItemsRead::newestItemIdChanged()
     */
    void setNewestItemId(qint64 nNewestItemId);

    /*!
     * \brief Executes the API request.
     *
     * To perform a successful API request to mark all items/articles as read, MarkAllItemsRead::newestItemId has to be valid ID
     * and there has to be an AbstractConfiguration object set to Component::configuration.
     *
     * Execution will not run while Component::inOperation returns \c true and will itself set that property to \c true when the request starts.
     */
    Q_INVOKABLE void execute() override;


Q_SIGNALS:
    /*!
     * \brief This is emitted if the value of the \link MarkAllItemsRead::newestItemId newestItemId \endlink property changes.
     * \sa MarkAllItemsRead::newestItemId(), MarkAllItemsRead::setNewestItemId()
     */
    void newestItemIdChanged(qint64 newestItemId);

    /*!
     * \brief This signal is emitted if the request to mark all items as read was successful.
     * \param newestItemId  ID of the newest/highest local available item
     */
    void succeeded(qint64 newestItemId);

protected:
    MarkAllItemsRead(MarkAllItemsReadPrivate &dd, QObject *parent = nullptr);

    /*!
     * \brief Finishes the item/article marking if the request was successful.
     *
     * If Component::storage contains a valid pointer, the AbstractStorage::allItemsMarkedRead() slot will be called to
     * update the local storage. After that the Component::inOperation property will be set to false and the succeeded() signal
     * will be emitted.
     */
    void successCallback() override;

    /*!
     * \brief Will check for a valid \a newestItemId.
     *
     * Will at first perform the checks of Component::checkInput(). Will than simply check if \a newestItemId is not lower or equal to \c 0.
     */
    bool checkInput() override;

private:
    Q_DISABLE_COPY(MarkAllItemsRead)
    Q_DECLARE_PRIVATE(MarkAllItemsRead)

};

}


#endif // FUOTENMARKALLITEMSREAD_H
