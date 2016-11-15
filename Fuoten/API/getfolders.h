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

#ifndef FUOTENGETFOLDERS_H
#define FUOTENGETFOLDERS_H

#include <QObject>
#include "component.h"
#include "../fuoten_global.h"

namespace Fuoten {

class GetFoldersPrivate;

/*!
 * \brief Requests the list of folders from the News App API.
 *
 * To request the folder list, the Component::configuration property has to be set to a valid AbstractConfiguration object. After setting it,
 * call execute() to perform the API request.
 *
 * If a valid AbstractStorage object is set to the Component::storage property, AbstractStorage::foldersRequested will be called in the successCallback()
 * to save the requested folders in the local storage. If the request succeeded, the Component::succeeded() signal will be emitted, containing the JSON api
 * reply.
 *
 * If something failed, Component::failed() will be emitted and the Component::error property will contain a valid pointer to an Error object.
 *
 * \par Mandatory properties
 * Component::configuration
 *
 * \par API route
 * /folders
 *
 * \par Method
 * GET
 *
 * \sa Synchronizer
 * \headerfile "" <Fuoten/API/GetFolders>
 */
class FUOTENSHARED_EXPORT GetFolders : public Component
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs an API request object with the given \a parent to query folders from the remote server.
     */
    explicit GetFolders(QObject *parent = nullptr);

    /*!
     * \brief Executes the API request.
     *
     * To perform a successful API request, Component::configuration has to be set to a valid AbstractConfiguration object.
     *
     * Execution will not run if Component::inOperation returns \c true and will itself set that property to \c true when start to perform
     * the request.
     */
    Q_INVOKABLE void execute() override;

protected:
    GetFolders(GetFoldersPrivate &dd, QObject *parent = nullptr);

    /*!
     * \brief Finishes the the operation if the request was successful.
     *
     * If Component::storage points to a valid object, it will use AbstractStorage::foldersRequested() to store, update and delete the
     * folders in the local storage according to the server reply. Afterwards it will set Component::inOperation to false and will emit
     * the Component::succeeded() signal.
     */
    void successCallback() override;

    /*!
     * \brief Checks for a \a folders array in the JSON API reply.
     *
     * Will at first perform the checks from Component::checkOutput() and will than check if the \a folders array is present.
     * Will \b not check if the array is empty.
     */
    bool checkOutput() override;

private:
    Q_DECLARE_PRIVATE(GetFolders)
    Q_DISABLE_COPY(GetFolders)
};

}

#endif // FUOTENGETFOLDERS_H
