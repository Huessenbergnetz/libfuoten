/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
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
