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

#ifndef FUOTENGETSTATUS_H
#define FUOTENGETSTATUS_H

#include <QObject>
#include "component.h"
#include "../fuoten_global.h"

namespace Fuoten {

class GetStatusPrivate;

/*!
 * \brief Requests the status from the News App.
 *
 * The status reply will contain the News App version number an possible warnings about improperly configurations.
 * To request the status, set the \link Component::configuration configuration \endlink property and call execute().
 *
 * The requested data will be written to Configuration::setServerVersion() and Configuration::setImproperlyConfiguredCron(). You can get the raw JSON response from the Component::succeeded() signal.
 * If the request failed for some reason, Component::failed() will be emitted and the Component::inOperation property will contain a valid pointer to an Error object.
 *
 * \par Mandatory properties
 * Component::configuration
 *
 * \par API route
 * /status
 *
 * \par Method
 * GET
 *
 * \sa AccountValidator
 * \headerfile "" <Fuoten/Generic/GetStatus>
 */
class FUOTENSHARED_EXPORT GetStatus : public Component
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs an API request object with the given \a parent to query the status from the remote server.
     */
    GetStatus(QObject *parent = nullptr);

    /*!
     * \brief Executes the API request.
     *
     * To perform a successful API request, Component::configuration has to be set to a valid Configuration object.
     *
     * Execution will not run if Component::inOperation returns \c true and will itself set that property to \c true when start to perform
     * the request.
     */
    Q_INVOKABLE void execute() override;

protected:
    GetStatus(GetStatusPrivate &dd, QObject *parent = nullptr);

    /*!
     * \brief Finishes the status request if it was successful.
     *
     * Will use Configuration::setServerVersion() and Configuration::setImproperlyConfiguredCron() to store the reply. Afterwards it will
     * set Component::inOperation to \c false and emits the Component::succeeded() signal.
     */
    void successCallback() override;

    /*!
     * \brief Extracts possible errors replied by the News App API.
     */
    void extractError(QNetworkReply *reply) override;

    /*!
     * \brief Checks if \a version and \a warnings are present in the JSON reply.
     *
     * Will at first perform the checks from Component::checkOutput().
     */
    bool checkOutput() override;

private:
    Q_DECLARE_PRIVATE(GetStatus)
    Q_DISABLE_COPY(GetStatus)

};

}

#endif // FUOTENGETSTATUS_H
