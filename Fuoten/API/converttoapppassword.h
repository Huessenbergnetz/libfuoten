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

#ifndef FUOTENCONVERTTOAPPPASSWORD_H
#define FUOTENCONVERTTOAPPPASSWORD_H

#include <QObject>
#include "component.h"
#include "../fuoten_global.h"

namespace Fuoten {

class ConvertToAppPasswordPrivate;

/*!
 * \brief Converts “normal“ passwords to application passwords.
 *
 * Nextcloud’s OCS API supports the automatic upgrade from “normal“ passwords to
 * application specific passwords. This class implements this API call. Running
 * execute() will send a GET request to the /ocs/v2.php/core/getapppassword API
 * endpoint with the currently configured username and password for authorization.
 *
 * If a 403 HTTP status is returned, the current password is already an application
 * password and Component::succeeded() will be emitted containing an empty QJsonDocument.
 *
 * If a 200 HTTP status code is returned, the used password was not an application
 * password and the response contains a newly generated application password that will be
 * set to AbstractConfiguration::setPassword(). Also the Component::succeeded() signal
 * will be emitted containing the returned JSON data.
 *
 * If an error occured or the returned HTTP status is neither 200 or 403, Component::failed()
 * will be emitted and Comonent::error() will contain information about the occured Error.
 *
 * \note
 * This API call will use AbstractConfiguration::getLoginFlowUserAgent() to set the User-Agent
 * header of the request to generate a user friendly application name user agent that will
 * be shown in the configuration backend of the Nextcloud.
 *
 * \since 0.8.0
 *
 * \par Mandatory properties
 * Component::configuration
 *
 * \par API route
 * Not part of the News App API, uses general Nextcloud OCS API at /ocs/v2.php/core/getapppassword
 *
 * \par Method
 * GET
 *
 * \headerfile "" <Fuoten/API/ConvertToAppPassword>
 */
class FUOTENSHARED_EXPORT ConvertToAppPassword : public Component
{
    Q_OBJECT
public:
    /*!
     * \brief Contructs a new %ConvertToAppPassword object with the given \a parent.
     */
    explicit ConvertToAppPassword(QObject *parent = nullptr);

    /*!
     * \brief Destroys the %ConvertToAppPassword object.
     */
    ~ConvertToAppPassword() override;

    /*!
     * \brief Executes the API request.
     */
    Q_INVOKABLE void execute() override;

protected:
    ConvertToAppPassword(ConvertToAppPasswordPrivate &dd, QObject *parent = nullptr);

    /*!
     * \brief Extracts the newly generated application password from the API response.
     *
     * If the response contains a non-empty application password, it will be set to
     * AbstractConfiguration::setPassword() and Component::succeeded() will be emitted,
     * otherwise the error data from the response will be extracted and Component::failed()
     * will be emitted while Component::error() will contain the Error information.
     */
    void successCallback() override;

    /*!
     * \brief Extracts the error data from the API response.
     *
     * If the API responded with a 403 HTTP status code, the used password is already
     * an application password and all is fine and Component::succeeded() will be emitted
     * with an empty QJsonDocument.
     *
     * For all other errors Component::failed() will be emitted and Component::error()
     * will contain the Error information.
     */
    void extractError(QNetworkReply *reply) override;

private:
    Q_DECLARE_PRIVATE(ConvertToAppPassword)
    Q_DISABLE_COPY(ConvertToAppPassword)
    Q_DISABLE_MOVE(ConvertToAppPassword)
};

}

#endif // FUOTENCONVERTTOAPPPASSWORD_H
