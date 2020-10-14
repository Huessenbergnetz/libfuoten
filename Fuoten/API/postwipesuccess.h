/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016-2020 Hüssenbergnetz/Matthias Fehring
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

#ifndef FUOTENPOSTWIPESUCCESS_H
#define FUOTENPOSTWIPESUCCESS_H

#include <QObject>
#include "component.h"
#include "../fuoten_global.h"

namespace Fuoten {

class PostWipeSuccessPrivate;

/*!
 * \brief Signals wipe completion to the Nextcloud server.
 *
 * This posts the success message to the Nextcloud server after a requested remote wipe
 * has been successfuly performed on the local device. Should be used after a remote wipe
 * has been signaled by GetWipeStatus::wipeRequested() and the local wipe has been performed.
 * There is already WipeManager that can handle remote wipe requests.
 *
 * See also https://docs.nextcloud.com/server/latest/developer_manual/client_apis/RemoteWipe/index.html
 *
 * \par Mandatory properties
 * Component::configuration
 *
 * \par API route
 * Not part of the News App API, uses general Nextcloud API at /index.php/core/wipe/success
 *
 * \par Method
 * POST
 *
 * \since 0.8.0
 *
 * \sa WipeManager, GetWipeStatus
 *
 * \headerfile "" <Fuoten/API/PostWipeSuccess>
 */
class FUOTENSHARED_EXPORT PostWipeSuccess : public Component
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs a new %PostWipeSuccess object with the given \a parent.
     */
    explicit PostWipeSuccess(QObject *parent = nullptr);

    /*!
     * \brief Destroys the %PostWipeSuccess object.
     */
    ~PostWipeSuccess() override;

    /*!
     * \brief Executes the API request.
     *
     * To perform a successful API request, the \link Component::configuration configuration\endlink
     * property has to be set to a valid object derived from AbstractConfiguration.
     *
     * Execution will not run if the \link Component::inOperation inOperation\endlink property returns
     * \c true and will itself set that property to \c true when starting the request.
     */
    Q_INVOKABLE void execute() override;

protected:
    PostWipeSuccess(PostWipeSuccessPrivate &dd, QObject *parent = nullptr);

    /*!
     * \brief Checks the input parameters.
     *
     * Will especially check if the password has been set.
     */
    bool checkInput() override;

    /*!
     * \brief Will emit the succeeded() signal.
     */
    void successCallback() override;

private:
    Q_DECLARE_PRIVATE(PostWipeSuccess)
    Q_DISABLE_COPY(PostWipeSuccess)
    Q_DISABLE_MOVE(PostWipeSuccess)
};

}

#endif // FUOTENPOSTWIPESUCCESS_H
