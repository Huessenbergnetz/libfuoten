/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENMARKITEM_H
#define FUOTENMARKITEM_H

#include <QObject>
#include "component.h"
#include "../fuoten_global.h"

namespace Fuoten {

class MarkItemPrivate;

/*!
 * \brief Marks an item/article as read/unread on the News App server.
 *
 * To mark an article as read or unread, you have to set a valid MakrItem::itemId and Component::configuration. Use MarkItem::unread to define
 * if the article should be marked as read or unread. After setting the mandatory properties, call execute() to perform the API request.
 *
 * If a valid AbstractStorage object is set to the Component::storage property, AbstractStorage::itemMarked() will be called in the successCallback() to
 * update the local storage. If the request succeeded, the MarkItem::succeeded() signal will be emitted, containing the \a itemId and the \a unread status.
 * If something failed, the Component::failed() signal will be emitted and Component::error will contain a valid pointer to an Error object.
 *
 * \par Mandatory properties
 * MarkItem::itemId, Component::configuration
 *
 * \par API route
 * /folders/{itemId}/{read|unread}
 *
 * \par Method
 * PUT
 *
 * \headerfile "" <Fuoten/API/MarkItem>
 */
class FUOTENSHARED_EXPORT MarkItem : public Component
{
    Q_OBJECT
    /*!
     * \brief ID of the item that should be marked as read or unread.
     *
     * This property can not be changed while Component::inOperation() returns \c true.
     *
     * \par Access functions:
     * <TABLE><TR><TD>qint64</TD><TD>itemId() const</TD></TR><TR><TD>void</TD><TD>setItemId(qint64 nItemId)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>itemIdChanged(qint64 itemId)</TD></TR></TABLE>
     */
    Q_PROPERTY(qint64 itemId READ itemId WRITE setItemId NOTIFY itemIdChanged)
    /*!
     * \brief If true, the item will be marked as unread, otherwise as read.
     *
     * Defaults to \c false. This property can not be changed while Component::inOperation() returns \c true.
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>unread() const</TD></TR><TR><TD>void</TD><TD>setUnread(bool nUnread)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>unreadChanged(bool unread)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool unread READ unread WRITE setUnread NOTIFY unreadChanged)
public:
    /*!
     * \brief Constructs a new MarkItem object with default values and the given \a parent.
     */
    explicit MarkItem(QObject *parent = nullptr);

    /*!
     * \brief Constructs a new MarkItem object with the given arguments and \a parent.
     */
    MarkItem(qint64 itemId, bool unread, QObject *parent = nullptr);

    /*!
     * \brief Destroys the %MarkItem object.
     */
    ~MarkItem() override;

    /*!
     * \brief Getter function for the \link MarkItem::itemId itemId \endlink property.
     * \sa MarkItem::setItemId(), MarkItem::itemIdChanged()
     */
    qint64 itemId() const;
    /*!
     * \brief Getter function for the \link MarkItem::unread unread \endlink property.
     * \sa MarkItem::setUnread(), MarkItem::unreadChanged()
     */
    bool unread() const;

    /*!
     * \brief Setter function for the \link MarkItem::itemId itemId \endlink property.
     * Emits the itemIdChanged() signal if \a nItemId is not equal to the stored value.
     * \sa MarkItem::itemId(), MarkItem::itemIdChanged()
     */
    void setItemId(qint64 nItemId);
    /*!
     * \brief Setter function for the \link MarkItem::unread unread \endlink property.
     * Emits the unreadChanged() signal if \a nUnread is not equal to the stored value.
     * \sa MarkItem::unread(), MarkItem::unreadChanged()
     */
    void setUnread(bool nUnread);

    /*!
     * \brief Executes the API request.
     *
     * To perform a successful API request to mark an article as read, MarkItem::itemId has to be a valid ID
     * and there has to be an AbstractConfiguration object set to Component::configuration.
     *
     * Execution will not run while Component::inOperation returns \c true and will itself set that property to \c true when the request starts.
     */
    Q_INVOKABLE void execute() override;


Q_SIGNALS:
    /*!
     * \brief This is emitted if the value of the \link MarkItem::itemId itemId \endlink property changes.
     * \sa MarkItem::itemId(), MarkItem::setItemId()
     */
    void itemIdChanged(qint64 itemId);
    /*!
     * \brief This is emitted if the value of the \link MarkItem::unread unread \endlink property changes.
     * \sa MarkItem::unread(), MarkItem::setUnread()
     */
    void unreadChanged(bool unread);

    /*!
     * \brief This signal is emitted if the request to mark an article as read was successful.
     * \param itemId    ID of the item that has been marked as read or unread
     * \param unread    \c true if the item has been marked as unread, otherwise \c false
     */
    void succeeded(qint64 itemId, bool unread);

protected:
    MarkItem(MarkItemPrivate &dd, QObject *parent = nullptr);

    /*!
     * \brief Finishes the item marking if the request was successful.
     *
     * If Component::storage contains a valid pointer, the AbstractStorage::itemMarked() function will be called to
     * update the local storage. After that the Component::inOperation property will be set to false and the succeeded() signal
     * will be emitted.
     */
    void successCallback() override;

    /*!
     * \brief Will check for valid \a itemId.
     *
     * Will at first perform the checks of Component::checkInput(). Will than simply check if the ID is not lower or equal to \c 0.
     */
    bool checkInput() override;

    /*!
     * \brief Extracts possible errors replied by the News App API.
     */
    void extractError(QNetworkReply *reply) override;

private:
    Q_DISABLE_COPY(MarkItem)
    Q_DISABLE_MOVE(MarkItem)
    Q_DECLARE_PRIVATE(MarkItem)
};

}


#endif // FUOTENMARKITEM_H
