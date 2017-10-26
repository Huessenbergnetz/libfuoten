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

#ifndef FUOTENGETUSER_H
#define FUOTENGETUSER_H

#include <QObject>
#include "component.h"
#include "../fuoten_global.h"

namespace Fuoten {

class GetUserPrivate;

/*!
 * \brief Requests the user information from the News App.
 *
 * The user reply will contain information about the authenticated user like display name, avatar and last login time.
 * To request the user information, set the \link Component::configuration configuration \endlink property and call execute().
 *
 * The requested data will be written to AbstractConfiguration::setDisplayName() and AbstractConfiguration::setAvatar(). You can get the raw JSON response from the Component::succeeded() signal.
 * If something failed, the Component::failed() signal will be emitted and Component::error will contain a valid pointer to an Error object.
 *
 * \par Mandatory properties
 * Component::configuration
 *
 * \par API route
 * /user
 *
 * \par Method
 * GET
 *
 * \sa AccountValidator
 * \headerfile "" <Fuoten/API/GetUser>
 */
class FUOTENSHARED_EXPORT GetUser : public Component
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs an API request object with the given \a parent to query information about the authenticated user from the remote server.
     */
    explicit GetUser(QObject *parent = nullptr);

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
    GetUser(GetUserPrivate &dd, QObject *parent = nullptr);

    /*!
     * \brief Finishes the user request if it was successful.
     *
     * Will use AbstractConfiguration::setDisplayName() and AbstractConfiguration::setAvatar() to store the reply. Afterwards it will
     * set Component::inOperation to \c false and emits the Component::succeeded() signal.
     */
    void successCallback() override;

    /*!
     * \brief Checks for \a displayName in the replied JSON object.
     *
     * Will at first perform the checks from Component::checkOutput().
     */
    bool checkOutput() override;

private:
    Q_DECLARE_PRIVATE(GetUser)
    Q_DISABLE_COPY(GetUser)

};

}

#endif // FUOTENGETUSER_H
