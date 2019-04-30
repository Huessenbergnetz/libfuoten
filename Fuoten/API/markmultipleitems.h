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

#ifndef FUOTENMARKMULTIPLEITEMS_H
#define FUOTENMARKMULTIPLEITEMS_H

#include <QObject>
#include "component.h"
#include "../fuoten_global.h"

namespace Fuoten {

class MarkMultipleItemsPrivate;

/*!
 * \brief Marks a list of articles read or unread on the News App server.
 *
 * To mark a list of articles as read or unread, you have to set a list of article IDs to MarkMultipleItems::itemIds and a valid object to Component::configuration.
 * Use MarkMultipleItems::unread to define if you want to mark the articles as read or unread. After setting the mandatory properties, call execute() to perform the API request.
 *
 * If a valid AbstractStorage object is set to the Component::storage property, AbstractStorage::itemsMarked() will be called in the successCallback() to
 * update the local storage. If the request succeeded, the MarkMultipleItemsPrivate::succeeded() signal will be emitted, containing the \a feedIds and the \a unread state.
 * If something failed, the Component::failed() signal will be emitted and Component::error will contain a valid pointer to an Error object.
 *
 * \par Mandatory properties
 * MarkMultipleItems::feedIds,Component::configuration
 *
 * \par API route
 * /folders/{read|unread}/multiple
 *
 * \par Method
 * PUT
 *
 * \headerfile "" <Fuoten/API/MarkMultipleItems>
 */
class FUOTENSHARED_EXPORT MarkMultipleItems : public Component
{
    Q_OBJECT
    /*!
     * \brief List of IDs of items that should be marked as read or unread.
     *
     * This property can not be changed while Component::inOperation() returns \c true.
     *
     * \par Access functions:
     * <TABLE><TR><TD>IdList</TD><TD>itemIds() const</TD></TR><TR><TD>void</TD><TD>setItemIds(const IdList &nItemIds)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>itemIdsChanged(const IdList &itemIds)</TD></TR></TABLE>
     */
    Q_PROPERTY(IdList itemIds READ itemIds WRITE setItemIds NOTIFY itemIdsChanged)
    /*!
     * \brief True if items should be marked as unread, otherwise false.
     *
     * This property can not be changed while Component::inOperation() returns \c true.
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>unread() const</TD></TR><TR><TD>void</TD><TD>setUnread(bool nUnread)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>unreadChanged(bool unread)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool unread READ unread WRITE setUnread NOTIFY unreadChanged)
public:
    /*!
     * \brief Constructs a new MarkMultipleItems object with default values and the given \a parent.
     */
    explicit MarkMultipleItems(QObject *parent = nullptr);

    /*!
     * \brief Constructs a new MarkMultipleItems object with the given arguments and \a parent.
     */
    MarkMultipleItems(const IdList &itemIds, bool unread, QObject *parent = nullptr);


    /*!
     * \brief Getter function for the \link MarkMultipleItems::itemIds itemIds \endlink property.
     * \sa MarkMultipleItems::setItemIds(), MarkMultipleItems::itemIdsChanged()
     */
    IdList itemIds() const;
    /*!
     * \brief Getter function for the \link MarkMultipleItems::unread unread \endlink property.
     * \sa MarkMultipleItems::setUnread(), MarkMultipleItems::unreadChanged()
     */
    bool unread() const;

    /*!
     * \brief Setter function for the \link MarkMultipleItems::itemIds itemIds \endlink property.
     * Emits the itemIdsChanged() signal if \a nItemIds is not equal to the stored value.
     * \sa MarkMultipleItems::itemIds(), MarkMultipleItems::itemIdsChanged()
     */
    void setItemIds(const IdList &nItemIds);
    /*!
     * \brief Setter function for the \link MarkMultipleItems::unread unread \endlink property.
     * Emits the unreadChanged() signal if \a nUnread is not equal to the stored value.
     * \sa MarkMultipleItems::unread(), MarkMultipleItems::unreadChanged()
     */
    void setUnread(bool nUnread);


    /*!
     * \brief Executes the API request.
     *
     * To perform a successful API request to mark a folder as read, MarkFolderRead::folderId and MarkFolderRead::newestItemId have to be valid IDs
     * and there has to be a AbstractConfiguration object set to Component::configuration.
     *
     * Execution will not run while Component::inOperation returns \c true and will itself set that property to \c true when the request starts.
     */
    Q_INVOKABLE void execute() override;


Q_SIGNALS:
    /*!
     * \brief This is emitted if the value of the \link MarkMultipleItems::itemIds itemIds \endlink property changes.
     * \sa MarkMultipleItems::itemIds(), MarkMultipleItems::setItemIds()
     */
    void itemIdsChanged(const Fuoten::IdList &itemIds);
    /*!
     * \brief This is emitted if the value of the \link MarkMultipleItems::unread unread \endlink property changes.
     * \sa MarkMultipleItems::unread(), MarkMultipleItems::setUnread()
     */
    void unreadChanged(bool unread);

    /*!
     * \brief This signal is emitted if the request to mark multiple items as read or unread was successful.
     * \param itemIds   list of IDs of articles that have been marked as read or unread
     * \param unread    \c true if the articles in the list have been marked as unread, \c false if marked as read
     */
    void succeeded(const Fuoten::IdList &itemIds, bool unread);

protected:
    MarkMultipleItems(MarkMultipleItemsPrivate &dd, QObject *parent = nullptr);

    /*!
     * \brief Finishes the marking if the request was successful.
     *
     * If Component::storage contains a valid pointer, the AbstractStorage::itemsMarked() function will be called to
     * update the local storage. After that the Component::inOperation property will be set to false and the succeeded() signal
     * will be emitted.
     */
    void successCallback() override;

    /*!
     * \brief Will check if the \a itemIds property is not empty.
     *
     * Will at first perform the checks of Component::checkInput(). Will than simply check for the existence of the itemIds list.
     */
    bool checkInput() override;

private:
    Q_DISABLE_COPY(MarkMultipleItems)
    Q_DECLARE_PRIVATE(MarkMultipleItems)

};

}


#endif // FUOTENMARKMULTIPLEITEMS_H
