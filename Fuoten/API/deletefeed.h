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

#ifndef FUOTENDELETEFEED_H
#define FUOTENDELETEFEED_H

#include <QObject>
#include "component.h"
#include "../fuoten_global.h"

namespace Fuoten {

class DeleteFeedPrivate;

/*!
 * \brief Deletes a feed on the News App server.
 *
 * To delete a feed on the server, set DeleteFeed::feedId to a valid ID and set an AbstractConfiguration to Component::configuration.
 *
 * If a valid AbstractStorage object is set to the Component::storage property, AbstractStorage::feedDeleted() will be called in the
 * successCallback() to delete the feed in the local storage. If the request succeeded, the succeed() signal will be emitted in the successCallback()
 * and it will contain the \a id of the deleted feed. If the request failed, Component::faild() will be emitted and Component::error contains a valid
 * pointer to an Error object.
 *
 * \par Mandatory properties
 * DeleteFeed::feedId, Component::configuration
 *
 * \par API route
 * /feeds/{feedId}
 *
 * \par Method
 * DELETE
 *
 * \headerfile "" <Fuoten/API/DeleteFeed>
 */
class FUOTENSHARED_EXPORT DeleteFeed : public Component
{
    Q_OBJECT
    /*!
     * \brief ID of the feed to delete.
     *
     * This property can not be changed while Component::inOperation() returns \c true.
     *
     * \par Access functions:
     * <TABLE><TR><TD>qint64</TD><TD>feedId() const</TD></TR><TR><TD>void</TD><TD>setFeedId(qint64 nFeedId)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>feedIdChanged(qint64 feedId)</TD></TR></TABLE>
     */
    Q_PROPERTY(qint64 feedId READ feedId WRITE setFeedId NOTIFY feedIdChanged)
public:
    /*!
     * \brief Constructs an API request object with the given \a parent to delete a feed on the remote server.
     */
    explicit DeleteFeed(QObject *parent = nullptr);

    /*!
     * \brief Returns the ID of the feed to delete.
     * \sa feedId
     */
    qint64 feedId() const;

    /*!
     * \brief Sets the ID of the feed to delete.
     * \sa feedId
     */
    void setFeedId(qint64 nFeedId);

    /*!
     * \brief Executes the API request.
     *
     * To perform a successful API request to delete a feed, DeleteFeed::feedId has to be a valid feed ID and there
     * has to be an AbstractConfiguration object set to Component::configuration.
     *
     * Execution will not run while Component::inOperation returns \c true and will itself set that property to \c true when the
     * request starts.
     */
    Q_INVOKABLE void execute() override;


Q_SIGNALS:
    /*!
     * \brief This is emitted if the ID of the feed to delete changes.
     * \sa feedId
     */
    void feedIdChanged(qint64 feedId);

    /*!
     * \brief Will be emitted in the success callback function and contains the ID of the deleted folder.
     * \sa Component::failed()
     */
    void succeeded(qint64 id);

protected:
    DeleteFeed(DeleteFeedPrivate &dd, QObject *parent = nullptr);

    /*!
     * \brief Finishes the feed deletion if the request was successful.
     */
    void successCallback() override;

    /*!
     * \brief Checks for a valid feed ID.
     *
     * This will at first perform the checks of Component::checkInput() and will
     * than simply check if the feed id is greater than zero.
     */
    bool checkInput() override;

    /*!
     * \brief Extracts possible errors replied by the News App API.
     */
    void extractError(QNetworkReply *reply) override;

private:
    Q_DISABLE_COPY(DeleteFeed)
    Q_DECLARE_PRIVATE(DeleteFeed)

};

}


#endif // FUOTENDELETEFEED_H
