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
     * \par Access functions:
     * <TABLE><TR><TD>quint64</TD><TD>folderId() const</TD></TR><TR><TD>void</TD><TD>setFolderId(quint64 nFolderId)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>folderIdChanged(quint64 folderId)</TD></TR></TABLE>
     */
    Q_PROPERTY(quint64 folderId READ folderId WRITE setFolderId NOTIFY folderIdChanged)
    /*!
     * \brief The new name for the folder.
     *
     * When setting this property, the input string will be simplified. See QString::simplified().
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>newName() const</TD></TR><TR><TD>void</TD><TD>setNewName(const QString &nNewName)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>newNameChanged(const QString &newName)</TD></TR></TABLE>
     */
    Q_PROPERTY(QString newName READ newName WRITE setNewName NOTIFY newNameChanged)
public:
    /*!
     * \brief Constructs a new RenameFolder object.
     */
    explicit RenameFolder(QObject *parent = nullptr);

    quint64 folderId() const;
    QString newName() const;

    void setFolderId(quint64 nFolderId);
    void setNewName(const QString &nNewName);

    /*!
     * \brief Executes the API request.
     *
     * To perform a successful API request, RenameFolder::folderId and RenameFolder::newName have to be valid.
     */
    Q_INVOKABLE void execute() override;


Q_SIGNALS:
    void folderIdChanged(quint64 folderId);
    void newNameChanged(const QString &newName);

    /*!
     * \brief Will be emitted when the request was successful.
     *
     * Will contain the ID of the renamed folder as well as the new name.
     *
     * \sa Component::failed()
     */
    void succeeded(quint64 id, const QString &newName);

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
