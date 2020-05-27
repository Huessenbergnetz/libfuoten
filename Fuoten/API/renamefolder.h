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

#ifndef FUOTENRENAMEFOLDER_H
#define FUOTENRENAMEFOLDER_H

#include <QObject>
#include "component.h"
#include "../fuoten_global.h"

namespace Fuoten {

class RenameFolderPrivate;

/*!
 * \brief Renames a folder on the News App server.
 *
 * To rename a folder, set the RenameFolder::folderId and RenameFolder::newName as well as the Component::configuration property.
 * Optionally set the Component::storage property to save the renamed folder. After setting the mandatory properties, call execute() to
 * perform the API request.
 *
 * If an AbstractStorage object is present, AbstractStorage::folderRenamed() will be used in the successCallback() to save the new folder name. If the request succeeded,
 * the RenameFolder::succeeded() signal will be emitted. If something failed, the Component::failed() signal will be emitted and Component::error will contain
 * a valid pointer to an Error object.
 *
 * \par Mandatory properties
 * RenameFolder::folderId, RenameFolder::newName
 *
 * \par API route
 * /folders/{folderId}
 *
 * \par Method
 * PUT
 *
 * \headerfile "" <Fuoten/API/RenameFolder>
 */
class FUOTENSHARED_EXPORT RenameFolder : public Component
{
    Q_OBJECT
    /*!
     * \brief ID of the folder to rename.
     *
     * This property can not be changed while Component::inOperation() returns \c true.
     *
     * \par Access functions:
     * <TABLE><TR><TD>qint64</TD><TD>folderId() const</TD></TR><TR><TD>void</TD><TD>setFolderId(qint64 nFolderId)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>folderIdChanged(qint64 folderId)</TD></TR></TABLE>
     */
    Q_PROPERTY(qint64 folderId READ folderId WRITE setFolderId NOTIFY folderIdChanged)
    /*!
     * \brief The new name for the folder.
     *
     * When setting this property, the input string will be simplified. See QString::simplified().
     *
     * This property can not be changed while Component::inOperation() returns \c true.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>newName() const</TD></TR><TR><TD>void</TD><TD>setNewName(const QString &nNewName)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>newNameChanged(const QString &newName)</TD></TR></TABLE>
     */
    Q_PROPERTY(QString newName READ newName WRITE setNewName NOTIFY newNameChanged)
public:
    /*!
     * \brief Constructs an API request object with the given \a parent to rename a folder on the remote server.
     */
    explicit RenameFolder(QObject *parent = nullptr);

    /*!
     * \brief Destroys the %RenameFolder object.
     */
    ~RenameFolder() override;

    /*!
     * \brief Returns the currently set ID of the folder that should be renamed.
     * \sa folderId
     */
    qint64 folderId() const;

    /*!
     * \brief Returns the currently set new name for the folder.
     * \sa name
     */
    QString newName() const;

    /*!
     * \brief Sets the ID of the folder that should be renamed.
     * \sa folderId
     */
    void setFolderId(qint64 nFolderId);

    /*!
     * \brief Sets the new name for the folder.
     * \sa newName
     */
    void setNewName(const QString &nNewName);

    /*!
     * \brief Executes the API request.
     *
     * To perform a successful API request, RenameFolder::folderId and RenameFolder::newName have to be valid.
     *
     * Execution will not run while Component::inOperation returns \c true and will itself set that property to \c true when the request starts.
     */
    Q_INVOKABLE void execute() override;


Q_SIGNALS:
    /*!
     * \brief This signal is emitted when the ID of the folder changes.
     * \sa folderId
     */
    void folderIdChanged(qint64 folderId);

    /*!
     * \brief This signal is emitted when the new name for the folder changes.
     * \sa newName
     */
    void newNameChanged(const QString &newName);

    /*!
     * \brief Will be emitted when the request was successful.
     *
     * Will contain the ID of the renamed folder as well as the new name.
     *
     * \sa Component::failed()
     */
    void succeeded(qint64 id, const QString &newName);

protected:
    RenameFolder(RenameFolderPrivate &dd, QObject *parent = nullptr);

    /*!
     * \brief Finishes the renaming if it was successful.
     *
     * If Component::storage points to a valid object, it will use AbstractStorage::folderRenamed() to store the renamed folder. Afterwards
     * it will set Component::inOperation to false and will emit the RenameFolder::succeeded() signal.
     */
    void successCallback() override;

    /*!
     * \brief Extracts possible errors replied by the News App API.
     */
    void extractError(QNetworkReply *reply) override;


    /*!
     * \brief Checks for valid folderId and newName properties.
     *
     * Will at first perform the checks from Component::checkInput() and will than check if RenameFolder::folderId is greater than zero
     * and RenameFolder::newName is not empty.
     */
    bool checkInput() override;

private:
    Q_DISABLE_COPY(RenameFolder)
    Q_DECLARE_PRIVATE(RenameFolder)

};

}


#endif // FUOTENRENAMEFOLDER_H
