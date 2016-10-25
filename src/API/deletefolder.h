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

#ifndef FUOTENDELETEFOLDER_H
#define FUOTENDELETEFOLDER_H

#include <QObject>
#include "component.h"
#include "../fuoten_global.h"


namespace Fuoten {

class DeleteFolderPrivate;

/*!
 * \brief Deletes a folder on the server.
 *
 * \par Mandatory properties
 * DeleteFolder::folderId, Component::configuration
 *
 * \par API route
 * /folders/{folderId}
 *
 * \par Method
 * DELETE
 */
class FUOTENSHARED_EXPORT DeleteFolder : public Component
{
    Q_OBJECT
    /*!
     * \brief ID of the folder to delete.
     *
     * Has to be set to a valid folder ID.
     *
     * \par Access functions:
     * <TABLE><TR><TD>quint64</TD><TD>folderId() const</TD></TR><TR><TD>void</TD><TD>setFolderId(quint64 nFolderId)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>folderIdChanged(quint64 folderId)</TD></TR></TABLE>
     */
    Q_PROPERTY(quint64 folderId READ folderId WRITE setFolderId NOTIFY folderIdChanged)
public:
    /*!
     * \brief Constructs a new DeleteFolder object.
     */
    explicit DeleteFolder(QObject *parent = nullptr);

    /*!
     * \brief Returns the ID of the folder to delete.
     * \sa folderId
     */
    quint64 folderId() const;

    /*!
     * \brief Sets the ID of the folder to delete.
     * \sa folderId
     */
    void setFolderId(quint64 nFolderId);


    /*!
     * \brief Executes the API request.
     *
     * To perform a successful API request to delete a folder, DeleteFolder::folderId has to be a valid folder ID and there
     * has to be a Configuration object set to Component::configuration.
     *
     * Execution will not run while Component::inOperation returns \c true and will itself set that property to \c true when the
     * request starts.
     *
     * This function is invokable from QML.
     */
    Q_INVOKABLE void execute() override;


Q_SIGNALS:
    /*!
     * \brief This signal is emitted when the ID of the folder to delete changes.
     * \sa folderId
     */
    void folderIdChanged(quint64 folderId);

    void succeeded(quint64 id);

protected:
    DeleteFolder(DeleteFolderPrivate &dd, QObject *parent = nullptr);

    /*!
     * \brief Finishes the folder deletion if the request was successful.
     */
    void successCallback() override;

    bool checkInput() override;

    void extractError(QNetworkReply *reply) override;

private:
    Q_DISABLE_COPY(DeleteFolder)
    Q_DECLARE_PRIVATE(DeleteFolder)

};

}


#endif // FUOTENDELETEFOLDER_H
