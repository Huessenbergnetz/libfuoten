/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016-2018 Hüssenbergnetz/Matthias Fehring
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

#ifndef FUOTENMOVEFEED_H
#define FUOTENMOVEFEED_H

#include <QObject>
#include "component.h"
#include "../fuoten_global.h"

namespace Fuoten {


class MoveFeedPrivate;

/*!
 * \brief Moves a feed to a different folder on the News App server.
 *
 * To move a feed, set the MoveFeed::feedId and MoveFeed::folderId properties to valid IDs and set the Component::configuration to a valid object.
 * Optionally set the Component::storage property to move the feed also in a local storage. After setting the mandatory properties, call execute()
 * to perform the API request.
 *
 * If a valid AbstractStorage object is set to the Component::storage property, AbstractStorage::feedMoved() will be called in the successCallback() to
 * to move the feed in the local storage. If the request succeeded, the MoveFeed::succeeded() signal will be emitted, containing the ID of the moved
 * feed and the ID of the target folder.
 * If something failed, the Component::failed() signal will be emitted and Component::error will contain a valid pointer to an Error object.
 *
 * \par Mandatory properties
 * MoveFeed::feedId, MoveFeed::folderId, Component::configuration
 *
 * \par API route
 * /feed/{feedId}/move
 *
 * \par Method
 * PUT
 *
 * \headerfile "" <Fuoten/API/MoveFeed>
 */
class FUOTENSHARED_EXPORT MoveFeed : public Component
{
    Q_OBJECT
    /*!
     * \brief ID of the feed that should be moved.
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
     * \brief ID of the folder the feed should be moved to.
     *
     * This property can not be changed while Component::inOperation() returns \c true.
     *
     * \par Access functions:
     * <TABLE><TR><TD>qint64</TD><TD>folderId() const</TD></TR><TR><TD>void</TD><TD>setFolderId(qint64 nFolderId)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>folderIdChanged(qint64 folderId)</TD></TR></TABLE>
     */
    Q_PROPERTY(qint64 folderId READ folderId WRITE setFolderId NOTIFY folderIdChanged)
public:
    /*!
     * \brief Constructs an API request object with the given \a parent to move a feed on the remote server to a different folder.
     */
    explicit MoveFeed(QObject *parent = nullptr);

    /*!
     * \brief Returns the ID of the feed that should be moved.
     * \sa feedId
     */
    qint64 feedId() const;

    /*!
     * \brief Returns the ID of the folder the feed should be moved to.
     * \sa folderId
     */
    qint64 folderId() const;

    /*!
     * \brief Sets the ID of the feed that should be moved.
     * \sa feedId
     */
    void setFeedId(qint64 nFeedId);

    /*!
     * \brief Sets the ID of the folder the feed should be moved to.
     * \sa folderId
     */
    void setFolderId(qint64 nFolderId);


    /*!
     * \brief Executes the API request.
     *
     * To perform a successful API request to move a feed, MoveFeed::feedId and MoveFeed::folderId have to
     * contain valid IDs and there has to be a AbstractConfiguration object set to Component::configuration.
     *
     * Execution will not run while Component::inOperation returns \c true and will itself set that property to \c true when the
     * request starts.
     */
    Q_INVOKABLE void execute() override;


Q_SIGNALS:
    /*!
     * \brief This is emitted if the ID of the feed that should be moved changes.
     * \sa feedId
     */
    void feedIdChanged(qint64 feedId);

    /*!
     * \brief This is emitted if the ID of the folder the feed should be moved to changes.
     * \sa folderId
     */
    void folderIdChanged(qint64 folderId);

    /*!
     * \brief This is emitted if the API request was successful.
     * \param feedId    ID of the feed that has been moved
     * \param folderId  ID of the folder the feed has been moved to
     */
    void succeeded(qint64 feedId, qint64 folderId);

protected:
    MoveFeed(MoveFeedPrivate &dd, QObject *parent = nullptr);

    /*!
     * \brief Finishes the feed moving if the request was successful.
     */
    void successCallback() override;

    /*!
     * \brief Checks for a valid feed and folder ID.
     *
     * This will at first perform the checks of Component::checkInput() and will
     * than simply check if the feed ID is greater than zero and if the folder ID is
     * not negative.
     */
    bool checkInput() override;

    /*!
     * \brief Extracts possible errors replied by the News App API.
     */
    void extractError(QNetworkReply *reply) override;

private:
    Q_DISABLE_COPY(MoveFeed)
    Q_DECLARE_PRIVATE(MoveFeed)

};

}


#endif // FUOTENMOVEFEED_H
