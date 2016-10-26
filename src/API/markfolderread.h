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

#ifndef FUOTENMARKFOLDERREAD_H
#define FUOTENMARKFOLDERREAD_H

#include <QObject>
#include "component.h"
#include "../fuoten_global.h"

namespace Fuoten {

class MarkFolderReadPrivate;

/*!
 * \brief Marks all articles in a folder as read on the News App server.
 *
 * To mark an entire folder as read, you have to set valid values to MarkFolderRead::folderId, MarkFolderRead::newestItemId and Component::configuration. After
 * setting the mandatory properties, call execute() to perform the API request.
 *
 * If a valid AbstractStorage object is set to the Component::storage property, AbstractStorage::folderMarkedRead() will be called in the successCallback() to
 * update the local storage. If the request succeeded, the MarkFolderRead::succeeded() signal will be emitted, containing the \a folderId and the \a newestItemId.
 * If something failed, the Component::failed() signal will be emitted and Component::error will contain a valid pointer to an Error object.
 *
 * \par Mandatory properties
 * MarkFolderRead::folderId, MarkFolderRead::newestItemId, Component::configuration
 *
 * \par API route
 * /folders/{folderId}/read
 *
 * \par Method
 * PUT
 *
 * \headerfile "" <Fuoten/API/MarkFolderRead>
 */
class FUOTENSHARED_EXPORT MarkFolderRead : public Component
{
    Q_OBJECT
    /*!
     * \brief The ID of the folder that should be marked as read.
     *
     * \par Access functions:
     * <TABLE><TR><TD>qint64</TD><TD>folderId() const</TD></TR><TR><TD>void</TD><TD>setFolderId(qint64 nFolderId)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>folderIdChanged(qint64 folderId)</TD></TR></TABLE>
     */
    Q_PROPERTY(qint64 folderId READ folderId WRITE setFolderId NOTIFY folderIdChanged)
    /*!
     * \brief The ID of the newest item in the folder.
     *
     * Will mark all items with IDs lower than equal that ID as read. This is mean to prevent marking items as read which the client does not yet know of.
     *
     * \par Access functions:
     * <TABLE><TR><TD>qint64</TD><TD>newestItemId() const</TD></TR><TR><TD>void</TD><TD>setNewestItemId(qint64 nNewestItemId)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>newestItemIdChanged(qint64 newestItemId)</TD></TR></TABLE>
     */
    Q_PROPERTY(qint64 newestItemId READ newestItemId WRITE setNewestItemId NOTIFY newestItemIdChanged)
public:
    /*!
     * \brief Constructs an API request object with the given \a parent to mark an entire folder as read on the remote server.
     */
    explicit MarkFolderRead(QObject *parent = nullptr);

    /*!
     * \brief Returns the ID of the folder to be marked as read.
     */
    qint64 folderId() const;

    /*!
     * \brief Returns the ID of the item that is set to be the newest in the folder.
     */
    qint64 newestItemId() const;

    /*!
     * \brief Sets the ID of the folder to be marked as read.
     */
    void setFolderId(qint64 nFolderId);

    /*!
     * \brief Sets the ID of the newest item in the folder.
     */
    void setNewestItemId(qint64 nNewestItemId);

    /*!
     * \brief Executes the API request.
     *
     * To perform a successful API request to mark a folder as read, MarkFolderRead::folderId and MarkFolderRead::newestItemId have to be valid IDs
     * and there has to be a Configuration object set to Component::configuration.
     *
     * Execution will not run while Component::inOperation returns \c true and will itself set that property to \c true when the request starts.
     */
    Q_INVOKABLE void execute() override;


Q_SIGNALS:
    /*!
     * \brief This signal is emitted when the ID of the folder to mark as read is changed.
     * \sa folderId
     */
    void folderIdChanged(qint64 folderId);

    /*!
     * \brief This signal is emitted when the ID of the newest item in the folder is changed.
     * \sa newestItemId
     */
    void newestItemIdChanged(qint64 newestItemId);

    /*!
     * \brief This signal will be emitted if the API request was successful.
     *
     * Will contain the \a id of the folder that has been marked as read as well as the ID of the \a newestItem
     * that has been marked as read in this folder.
     *
     * \sa Component::failed()
     */
    void succeeded(qint64 id, qint64 newestItem);

protected:
    MarkFolderRead(MarkFolderReadPrivate &dd, QObject *parent = nullptr);

    /*!
     * \brief Finished the folder marking if the request was successful.
     *
     * If Component::storage contains a valid pointer, the AbstractStorage::folderMarkedRead() function will be called to
     * update the local storage. After that the Component::inOperation property will be set to false and the succeeded() signal
     * will be emitted.
     */
    void successCallback() override;

    /*!
     * \brief Will check for valid \a folderId and \a newestItemId.
     *
     * Will at first perform the checks of Component::checkInput(). Will than simply check if the IDs are not lower or equal to \c 0.
     */
    bool checkInput() override;

    /*!
     * \brief Extracts possible errors replied by the News App API.
     */
    void extractError(QNetworkReply *reply) override;

private:
    Q_DISABLE_COPY(MarkFolderRead)
    Q_DECLARE_PRIVATE(MarkFolderRead)

};

}


#endif // FUOTENMARKFOLDERREAD_H
