/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENGETWIPESTATUS_H
#define FUOTENGETWIPESTATUS_H

#include <QObject>
#include "component.h"
#include "../fuoten_global.h"

namespace Fuoten {

class GetWipeStatusPrivate;

/*!
 * \brief Requests the wipe status from the Nextcloud.
 *
 * This will get the wipe status form the Nextcloud configured in the \link Component::configuration configuration\endlink
 * object used by this object. So that property is mandatory. This should be used after you get a server reply
 * with HTTP status code 401 or 403. If a wipe is requested, the wipeRequested() signal will be emitted. After
 * the application data has been wiped on the local device, you should use PostWipeSuccess to inform the remote
 * Nextcloud that the wipe has been executed. There is already WipeManager that can handle remote wipe requests.
 *
 * See also https://docs.nextcloud.com/server/latest/developer_manual/client_apis/RemoteWipe/index.html
 *
 * \par Mandatory properties
 * Component::configuration
 *
 * \par API route
 * Not part of the News App API, uses general Nextcloud API at /index.php/core/wipe/check
 *
 * \par Method
 * GET
 *
 * \since 0.8.0
 *
 * \sa WipeManager, PostWipeSuccess
 *
 * \headerfile "" <Fuoten/API/GetWipeStatus>
 */
class FUOTENSHARED_EXPORT GetWipeStatus : public Component
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs a new %GetWipeStatus object with the given \a parent.
     */
    explicit GetWipeStatus(QObject *parent = nullptr);

    /*!
     * \brief Destroys the %GetWipeStatus object.
     */
    ~GetWipeStatus() override;

    /*!
     * \brief Executes the API request.
     *
     * To perform a successful API request, Component::configuration has to be set to a valid object
     * derived from AbstractConfiguration.
     *
     * Execution will not run if the \link Component::inOperation inOperation\endlink property returns
     * \c true and will itself set that property to \c true when starting the request.
     */
    Q_INVOKABLE void execute() override;

Q_SIGNALS:
    /*!
     * \brief Triggered when a remote wipe has been requested.
     *
     * This signal will be emitted in the successCallback().
     */
    void wipeRequested();

protected:
    GetWipeStatus(GetWipeStatusPrivate &dd, QObject *parent = nullptr);

    /*!
     * \brief Checks the input parameters.
     *
     * Will especially check if the password has been set.
     */
    bool checkInput() override;

    /*!
     * \brief Will check the reply if a remote wipe is requested.
     *
     * If a wipe has been requested, the wipeRequested() signal will be emitted.
     */
    void successCallback() override;

private:
    Q_DECLARE_PRIVATE(GetWipeStatus)
    Q_DISABLE_COPY(GetWipeStatus)
};

}

#endif // FUOTENGETWIPESTATUS_H
