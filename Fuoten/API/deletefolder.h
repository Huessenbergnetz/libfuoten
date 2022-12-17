/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENDELETEFOLDER_H
#define FUOTENDELETEFOLDER_H

#include <QObject>
#include "component.h"
#include "../fuoten_global.h"
#include "fuoten_export.h"


namespace Fuoten {

class DeleteFolderPrivate;

/*!
 * \brief Deletes a folder on the News App server.
 *
 * To delete a folder on the server, set DeleteFolder::folderId to a valid ID and set a AbstractConfiguration to Component::configuration.
 *
 * If a valid AbstractStorage object is set to the Component::storage property, AbstractStorage::folderDeleted() will be called in the
 * successCallback() to delete the folder in the local storage. If the request succeeded, the succeed() signal will be emitted in the successCallback()
 * and it will contain the \a id of the deleted folder. If the request failed, Component::faild() will be emitted and Component::error contains a valid
 * pointer to an Error object.
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
class FUOTEN_EXPORT DeleteFolder : public Component
{
    Q_OBJECT
    /*!
     * \brief ID of the folder to delete.
     *
     * Has to be set to a valid folder ID. This property can not be changed while Component::inOperation() returns \c true.
     *
     * \par Access functions:
     * <TABLE><TR><TD>qint64</TD><TD>folderId() const</TD></TR><TR><TD>void</TD><TD>setFolderId(qint64 nFolderId)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>folderIdChanged(qint64 folderId)</TD></TR></TABLE>
     */
    Q_PROPERTY(qint64 folderId READ folderId WRITE setFolderId NOTIFY folderIdChanged)
public:
    /*!
     * \brief Constructs an API request object with the given \a parent to delete a folder on the remote server.
     */
    explicit DeleteFolder(QObject *parent = nullptr);

    /*!
     * \brief Destroys the %DeleteFolder object.
     */
    ~DeleteFolder() override;

    /*!
     * \brief Returns the ID of the folder to delete.
     * \sa folderId
     */
    qint64 folderId() const;

    /*!
     * \brief Sets the ID of the folder to delete.
     * \sa folderId
     */
    void setFolderId(qint64 nFolderId);


    /*!
     * \brief Executes the API request.
     *
     * To perform a successful API request to delete a folder, DeleteFolder::folderId has to be a valid folder ID and there
     * has to be a AbstractConfiguration object set to Component::configuration.
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
    void folderIdChanged(qint64 folderId);

    /*!
     * \brief Will be emitted in the success callback function and contains the ID of the deleted folder.
     * \sa Component::failed()
     */
    void succeeded(qint64 id);

protected:
    DeleteFolder(DeleteFolderPrivate &dd, QObject *parent = nullptr);

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

private:
    Q_DISABLE_COPY(DeleteFolder)
    Q_DECLARE_PRIVATE(DeleteFolder)
};

}


#endif // FUOTENDELETEFOLDER_H
