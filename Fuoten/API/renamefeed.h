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

#ifndef FUOTENRENAMEFEED_H
#define FUOTENRENAMEFEED_H

#include <QObject>
#include "component.h"
#include "../fuoten_global.h"

namespace Fuoten {

class RenameFeedPrivate;

/*!
 * \brief Renames a feed on the News App server.
 *
 * To rename a feed, set the RenameFeed::feedId and RenameFeed::newName as well as the Component::configuration property.
 * Optionally set the Component::storage property to save the renamed folder. After setting the mandatory properties, call execute() to
 * perform the API request.
 *
 * If an AbstractStorage object is present, AbstractStorage::feedRenamed() will be used in the successCallback() to save the new feed name. If the request succeeded,
 * the RenameFeed::succeeded() signal will be emitted. If something failed, the Component::failed() signal will be emitted and Component::error will contain
 * a valid pointer to an Error object.
 *
 * \par Mandatory properties
 * RenamFeed::folderId, RenameFeed::newName
 *
 * \par API route
 * /feeds/{feedId}/rename
 *
 * \par Method
 * PUT
 *
 * \headerfile "" <Fuoten/API/RenameFolder>
 */
class FUOTENSHARED_EXPORT RenameFeed : public Component
{
    Q_OBJECT
    /*!
     * \brief The ID of the feed that should be renamed.
     *
     * \par Access functions:
     * <TABLE><TR><TD>qint64</TD><TD>feedId() const</TD></TR><TR><TD>void</TD><TD>setFeedId(qint64 nFeedId)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>feedIdChanged(qint64 feedId)</TD></TR></TABLE>
     */
    Q_PROPERTY(qint64 feedId READ feedId WRITE setFeedId NOTIFY feedIdChanged)
    /*!
     * \brief The new name for the feed.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>newName() const</TD></TR><TR><TD>void</TD><TD>setNewName(const QString &nNewName)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>newNameChanged(const QString &newName)</TD></TR></TABLE>
     */
    Q_PROPERTY(QString newName READ newName WRITE setNewName NOTIFY newNameChanged)
public:
    /*!
     * \brief Constructs an API request object with the given \a parent to rename a feed on the remote server.
     */
    explicit RenameFeed(QObject *parent = nullptr);

    /*!
     * \brief Returns the ID of the feed that should be renamed.
     * \sa feedId
     */
    qint64 feedId() const;

    /*!
     * \brief Returns the new name for the feed.
     * \sa newName
     */
    QString newName() const;

    /*!
     * \brief Sets the ID of the feed that should be renamed.
     * \param nFeedId ID of the feed
     * \sa feedId
     */
    void setFeedId(qint64 nFeedId);

    /*!
     * \brief Sets the new name for the feed.
     * \param nNewName the new name for the feed
     * \ſa newName
     */
    void setNewName(const QString &nNewName);


    /*!
     * \brief Executes the API request.
     *
     * To perform a successful API request, RenameFeed::feedId and RenameFeed::newName have to be valid.
     *
     * Execution will not run while Component::inOperation returns \c true and will itself set that property to \c true when the request starts.
     */
    Q_INVOKABLE void execute() override;


Q_SIGNALS:
    /*!
     * \brief This signal is emitted if the \link RenameFeed::feedId feedId \endlink property changes.
     * \param feedId the ID of the feed to change the name
     * \sa feedId
     */
    void feedIdChanged(qint64 feedId);

    /*!
     * \brief This signal is emitted if the \link RenameFeed::newName newName \endlink property changes.
     * \param newName the new name for the feed
     * \sa newName
     */
    void newNameChanged(const QString &newName);

    /*!
     * \brief Will be emitted when the request was successful.
     *
     * Will contain the ID of the renamed folder as well as the new name.
     *
     * \param id the ID of the feed that has changed the name
     * \param newName the new name for the feed
     * \sa Component::failed()
     */
    void succeeded(qint64 id, const QString &newName);

protected:
    RenameFeed(RenameFeedPrivate &dd, QObject *parent = nullptr);

    /*!
     * \brief Finishes the renaming if it was successful.
     *
     * If Component::storage points to a valid object, it will use AbstractStorage::feedRenamed() to store the renamed feed. Afterwards
     * it will set Component::inOperation to false and will emit the RenameFeed::succeeded() signal.
     */
    void successCallback() override;

    /*!
     * \brief Extracts possible errors replied by the News App API.
     */
    void extractError(QNetworkReply *reply) override;


    /*!
     * \brief Checks for valid feedId and newName properties.
     *
     * Will at first perform the checks from Component::checkInput() and will than check if RenameFeed::feedId is greater than zero
     * and RenameFeed::newName is not empty.
     */
    bool checkInput() override;

private:
    Q_DISABLE_COPY(RenameFeed)
    Q_DECLARE_PRIVATE(RenameFeed)

};

}


#endif // FUOTENRENAMEFEED_H
