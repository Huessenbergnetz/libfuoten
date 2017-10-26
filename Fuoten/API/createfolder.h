/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016-2017 Matthias Fehring
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

#ifndef FUOTENCREATEFOLDER_H
#define FUOTENCREATEFOLDER_H

#include <QObject>
#include "component.h"
#include "../fuoten_global.h"

namespace Fuoten {

class CreateFolderPrivate;

/*!
 * \brief Creates a new folder on the News App server.
 *
 * To create a new folder, set the CreateFolder::name property to a valid string and set the Component::configuration to a valid object.
 * Optionally set the Component::storage property to save the newly created folder in a local storage. After setting the mandatory properties,
 * call execute() to perform the API request.
 *
 * If a valid AbstractStorage object is set to the Component::storage property, AbstractStorage::folderCreated() will be called in the successCallback() to
 * save the new folder in the local storage. If the request succeeded, the Component::succeeded() signal will be emitted, containing the JSON API reply.
 * If something failed, the Component::failed() signal will be emitted and Component::error will contain a valid pointer to an Error object.
 *
 * \par Mandatory properties
 * CreateFolder::name, Component::configuration
 *
 * \par API route
 * /folders
 *
 * \par Method
 * POST
 *
 * \headerfile "" <Fuoten/API/CreateFolder>
 */
class FUOTENSHARED_EXPORT CreateFolder : public Component
{
    Q_OBJECT
    /*!
     * \brief The name for the new folder.
     *
     * When setting this property, the input string will be simplified. See QString::simplified().
     *
     * This property can not be changed while Component::inOperation() returns \c true.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>name() const</TD></TR><TR><TD>void</TD><TD>setName(const QString &nName)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>nameChanged(const QString &name)</TD></TR></TABLE>
     */
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
public:
    /*!
     * \brief Constructs an API request object with the given \a parent to create a new folder on the remote server.
     */
    explicit CreateFolder(QObject *parent = nullptr);

    /*!
     * \brief Returns the name of the folder to create.
     *
     * \sa name
     */
    QString name() const;

    /*!
     * \brief Sets the name of the folder to create.
     *
     * \sa name
     */
    void setName(const QString &nName);

    /*!
     * \brief Executes the API request.
     *
     * To perform a successful API request, CreateFolder::name has to be valid and there has to be a AbstractConfiguration object set to
     * Component::configuration.
     *
     * Execution will not run if Component::inOperation returns \c true and will itself set that property to \c true when start to perform
     * the request.
     */
    Q_INVOKABLE void execute() override;


Q_SIGNALS:
    /*!
     * \brief This signal is emitted when the name of the folder to create changes.
     * \sa name
     */
    void nameChanged(const QString &name);

protected:
    CreateFolder(CreateFolderPrivate &dd, QObject *parent = nullptr);

    /*!
     * \brief Finishes the folder creation if the request was successful.
     *
     * If Component::storage points to a valid object, it will use AbstractStorage::folderCreated() to store the created folder. Afterwards
     * it will set Component::inOperation to false and will emit the Component::succeeded() signal.
     */
    void successCallback() override;

    /*!
     * \brief Extracts possible errors replied by the News App API.
     */
    void extractError(QNetworkReply *reply) override;

    /*!
     * \brief Checks for valid name property.
     *
     * Will at first perform the checks of Component::checkInput(). Returns \c true if the checks pass, otherwise false.
     */
    bool checkInput() override;

    /*!
     * \brief Checks for a non-empty \a folders array in the replied JSON object.
     *
     * Will at first perform the checks fo Component::checkOutput(). Returns \c true if the checks pass, otherwise false.
     */
    bool checkOutput() override;

private:
    Q_DISABLE_COPY(CreateFolder)
    Q_DECLARE_PRIVATE(CreateFolder)

};

}


#endif // FUOTENCREATEFOLDER_H
