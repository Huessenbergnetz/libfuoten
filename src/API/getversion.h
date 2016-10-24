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

#ifndef FUOTENGETVERSION_H
#define FUOTENGETVERSION_H

#include <QObject>
#include "component.h"
#include "../fuoten_global.h"

namespace Fuoten {

class GetVersionPrivate;

/*!
 * \brief Requests the installed News App version from the server.
 *
 * The version reply will only contain the version number of the installed News App.
 * To request the version information, set the \link Component::configuration configuration \endlink property and call get().
 *
 * The requested data will be written to Configuration::setServerVersion(). You can get the raw JSON response from the Component::succeeded() signal.
 *
 * \sa AccountValidator
 * \headerfile "" <Fuoten/API/GetVersion>
 */
class FUOTENSHARED_EXPORT GetVersion : public Component
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs a new GetVersion object.
     */
    GetVersion(QObject *parent = nullptr);

    /*!
     * \brief Starts the API request.
     */
    Q_INVOKABLE void execute() override;

protected:
    GetVersion(GetVersionPrivate &dd, QObject *parent = nullptr);

    void successCallback() override;

    void extractError(QNetworkReply *reply) override;

    bool checkOutput() override;

private:
    Q_DECLARE_PRIVATE(GetVersion)
    Q_DISABLE_COPY(GetVersion)
};

}

#endif // FUOTENGETVERSION_H
