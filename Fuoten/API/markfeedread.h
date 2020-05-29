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

#ifndef FUOTENMARKFEEDREAD_H
#define FUOTENMARKFEEDREAD_H

#include <QObject>
#include "component.h"
#include "../fuoten_global.h"

namespace Fuoten {

class MarkFeedReadPrivate;

/*!
 * \brief Marks all articles in a feed as read on the News App server.
 *
 * To mark an entire feed as read, you have to set valid values to MarkFeedRead::feedId, MarkFeedRead::newestItemId and Component::configuration. After
 * setting the mandatory properties, call execute() to perform the API request.
 *
 * If a valid AbstractStorage object is set to the Component::storage property, AbstractStorage::feedMarkedRead() will be called in the successCallback() to
 * update the local storage. If the request succeeded, the MarkFeedRead::succeeded() signal will be emitted, containing the \a feedId and the \a newestItemId.
 * If something failed, the Component::failed() signal will be emitted and Component::error will contain a valid pointer to an Error object.
 *
 * \par Mandatory properties
 * MarkFeedRead::feedId, MarkFeedRead::newestItemId, Component::configuration
 *
 * \par API route
 * /feeds/{feedId}/read
 *
 * \par Method
 * PUT
 *
 * \headerfile "" <Fuoten/API/MarkFeedRead>
 */
class FUOTENSHARED_EXPORT MarkFeedRead : public Component
{
    Q_OBJECT
    /*!
     * \brief ID of the feed to mark as read.
     *
     * This property can not be changed while Component::inOperation() returns \c true.
     *
     * \par Access functions:
     * <TABLE><TR><TD>qint64</TD><TD>feedId() const</TD></TR><TR><TD>void</TD><TD>setFeedId(qint64 nFeedId)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>feedIdChanged(qint64 feedId)</TD></TR></TABLE>
     */
    Q_PROPERTY(qint64 feedId READ feedId WRITE setFeedId NOTIFY feedIdChanged)
    /*!
     * \brief ID of the newest local available item in the feed.
     *
     * This property can not be changed while Component::inOperation() returns \c true.
     *
     * \par Access functions:
     * <TABLE><TR><TD>qint64</TD><TD>newestItemId() const</TD></TR><TR><TD>void</TD><TD>setNewestItemId(qint64 nNewestItemId)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>newestItemIdChanged(qint64 newestItemId)</TD></TR></TABLE>
     */
    Q_PROPERTY(qint64 newestItemId READ newestItemId WRITE setNewestItemId NOTIFY newestItemIdChanged)
public:
    /*!
     * \brief Constructs an API request object with the given \a parent to mark an entire feed as read on the remote server.
     */
    explicit MarkFeedRead(QObject *parent = nullptr);

    /*!
     * \brief Destroys the %MarkFeedRead object.
     */
    ~MarkFeedRead() override;

    /*!
     * \brief Returns the ID of the feed to be marked as read.
     * \sa feedId
     */
    qint64 feedId() const;

    /*!
     * \brief Returns the ID of the newest item in the feed.
     * \sa newestItemId
     */
    qint64 newestItemId() const;

    /*!
     * \brief Sets the ID of the feed to be marked as read.
     * \sa feedId
     */
    void setFeedId(qint64 nFeedId);

    /*!
     * \brief Sets the ID of the newest item in the feed.
     * \sa newestItemId
     */
    void setNewestItemId(qint64 nNewestItemId);

    /*!
     * \brief Executes the API request.
     *
     * To perform a successful API request to mark a folder as read, MarkFeedRead::feedId and MarkFeedRead::newestItemId have to be valid IDs
     * and there has to be an AbstractConfiguration object set to Component::configuration.
     *
     * Execution will not run while Component::inOperation returns \c true and will itself set that property to \c true when the request starts.
     */
    Q_INVOKABLE void execute() override;


Q_SIGNALS:
    /*!
     * \brief This signal is emitted if the ID of the feed to mark as read changes.
     * \sa feedId
     */
    void feedIdChanged(qint64 feedId);

    /*!
     * \brief This signal is emitted if the ID of the newest item in the feed changes.
     * \sa newestItemId
     */
    void newestItemIdChanged(qint64 newestItemId);

    /*!
     * \brief This signal is emitted if the request to mark a feed as read was successful.
     * \param feedId        ID of the feed that has been marked as read
     * \param newestItemId  ID of the newest local available item in the feed.
     */
    void succeeded(qint64 feedId, qint64 newestItemId);

protected:
    MarkFeedRead(MarkFeedReadPrivate &dd, QObject *parent = nullptr);

    /*!
     * \brief Finishes the feed marking if the request was successful.
     *
     * If Component::storage contains a valid pointer, the AbstractStorage::feedMarkedRead() function will be called to
     * update the local storage. After that the Component::inOperation property will be set to false and the succeeded() signal
     * will be emitted.
     */
    void successCallback() override;

    /*!
     * \brief Will check for valid \a feedId and \a newestItemId.
     *
     * Will at first perform the checks of Component::checkInput(). Will than simply check if the IDs are not lower or equal to \c 0.
     */
    bool checkInput() override;

    /*!
     * \brief Extracts possible errors replied by the News App API.
     */
    void extractError(QNetworkReply *reply) override;

private:
    Q_DISABLE_COPY(MarkFeedRead)
    Q_DISABLE_MOVE(MarkFeedRead)
    Q_DECLARE_PRIVATE(MarkFeedRead)
};

}


#endif // FUOTENMARKFEEDREAD_H
