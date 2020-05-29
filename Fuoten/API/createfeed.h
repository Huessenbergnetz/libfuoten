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

#ifndef FUOTENCREATEFEED_H
#define FUOTENCREATEFEED_H

#include <QObject>
#include "component.h"
#include "../fuoten_global.h"

namespace Fuoten {

class CreateFeedPrivate;

/*!
 * \brief Creates a new feed on the News App server.
 *
 * To create a new feed, set the CreateFeed::url property to a valid URL string and set the Component::configuration to a valid object. To add
 * the feed to a specific folder, set the CreateFeed::folderId property. Optionally set the Component::storage property to save the newly created
 * feed in a local storage. After setting the mandatory properties, call execute() to perform the API request.
 *
 * If a valid AbstractStorage object is set to the Component::storage property, AbstractStorage::feedCreated() will be called in the successCallback() to
 * save the new feed in the local storage. If the request succeeded, the Component::succeeded() signal will be emitted, containing the JSON API reply.
 * If something failed, the Component::failed() signal will be emitted and Component::error will contain a valid pointer to an Error object.
 *
 * \par Mandatory properties
 * CreateFeed::url, Component::configuration
 *
 * \par API route
 * /feeds
 *
 * \par Method
 * POST
 *
 * \headerfile "" <Fuoten/API/CreateFeed>
 */
class FUOTENSHARED_EXPORT CreateFeed : public Component
{
    Q_OBJECT
    /*!
     * \brief URL of the feed to add.
     *
     * This property can not be changed while Component::inOperation() returns \c true.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>url() const</TD></TR><TR><TD>void</TD><TD>setUrl(const QString &nUrl)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>urlChanged(const QString &url)</TD></TR></TABLE>
     */
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    /*!
     * \brief ID of the parent folder.
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
     * \brief Constructs an API request object with the given \a parent to create a feed on the remote server.
     */
    explicit CreateFeed(QObject *parent = nullptr);

    /*!
     * \brief Destroys the %CreateFeed object.
     */
    ~CreateFeed() override;

    /*!
     * \brief Returns the URL of the feed to add.
     * \sa url
     */
    QUrl url() const;

    /*!
     * \brief Returns the ID of the parent folder of the new feed.
     * \sa folderId
     */
    qint64 folderId() const;

    /*!
     * \brief Sets the URL for the new feed.
     * \sa url
     */
    void setUrl(const QUrl &nUrl);

    /*!
     * \brief Sets the ID of the parent folder of the new feed.
     * \sa folderId
     */
    void setFolderId(qint64 nFolderId);

    /*!
     * \brief Executes the API request.
     *
     * To perform a successful API request to create a feed, CreateFeed::url has to be a valid URL and there
     * has to be a AbstractConfiguration object set to Component::configuration.
     *
     * Execution will not run while Component::inOperation returns \c true and will itself set that property to \c true when the
     * request starts.
     */
    Q_INVOKABLE void execute() override;


Q_SIGNALS:
    /*!
     * \brief This is emitted if the URL for the new feed changes.
     * \sa url
     */
    void urlChanged(const QUrl &url);

    /*!
     * \brief This is emitted if the ID of the parent folder for the new feed changes.
     * \sa folderId
     */
    void folderIdChanged(qint64 folderId);

protected:
    CreateFeed(CreateFeedPrivate &dd, QObject *parent = nullptr);

    /*!
     * \brief Finishes the folder deletion if the request was successful.
     */
    void successCallback() override;

    /*!
     * \brief Checks for a valid folder ID.
     *
     * This will at first perform the checks of Component::checkInput() and will
     * than simply check if the folder id is greater than zero.
     */
    bool checkInput() override;

    /*!
     * \brief Extracts possible errors replied by the News App API.
     */
    void extractError(QNetworkReply *reply) override;

    /*!
     * \brief Checks if the returned JSON data contain a non-empty feed array.
     *
     * Will at first perform the checks of Component::checkOutput().
     */
    bool checkOutput() override;

private:
    Q_DISABLE_COPY(CreateFeed)
    Q_DISABLE_MOVE(CreateFeed)
    Q_DECLARE_PRIVATE(CreateFeed)
};

}


#endif // FUOTENCREATEFEED_H
