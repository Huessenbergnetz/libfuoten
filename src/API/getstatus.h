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
 * To request the status, set the \link Component::configuration configuration \endlink property and call get().
 *
 * The requested data will be written to Configuration::setServerVersion() and Configuration::setImproperlyConfiguredCron(). You can get the raw JSON response from the Component::succeeded() signal.
 *
 * \sa AccountValidator
 * \headerfile "" <Fuoten/Generic/GetStatus>
 */
class FUOTENSHARED_EXPORT GetStatus : public Component
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs a new GetStatus object.
     */
    GetStatus(QObject *parent = nullptr);

    /*!
     * \brief Starts the API request.
     */
    Q_INVOKABLE void execute() Q_DECL_OVERRIDE;

protected:
    GetStatus(GetStatusPrivate &dd, QObject *parent = nullptr);

    void successCallback() Q_DECL_OVERRIDE;

    void extractError(QNetworkReply *reply) Q_DECL_OVERRIDE;

    bool checkOutput() Q_DECL_OVERRIDE;

private:
    Q_DECLARE_PRIVATE(GetStatus)
    Q_DISABLE_COPY(GetStatus)

};

}

#endif // FUOTENGETSTATUS_H
