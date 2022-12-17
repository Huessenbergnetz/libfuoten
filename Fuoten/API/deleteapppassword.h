/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENDELETEAPPPASSWORD_H
#define FUOTENDELETEAPPPASSWORD_H

#include <QObject>
#include "component.h"
#include "../fuoten_global.h"
#include "fuoten_export.h"

namespace Fuoten {

class DeleteAppPasswordPrivate;

/*!
 * \brief Deletes application passwords.
 *
 * Nextcloud’s OCS API supports the deletion of application passwords that have been created manually
 * or by using the Login Flow mechanism or by converting “normal“ passwords to application passwords.
 * This API class can be used to delete such application passwords on the remote Nextcloud server
 * that have been stored in the \link AbstractConfiguration configuration\endlink. This is useful if
 * you delete the local account and want to also clean up the application password used for it on the
 * remote server.
 *
 * \note
 * This class will not remove the deleted application password from your local storage/settings. The
 * Nextcloud API docs recommend to even delete the local stored password if the remote deletion fails.
 * So you should connect to both signals Component::failed() and Component::succeeded() to delete the
 * local data after executing this API command.
 *
 * \since 0.8.0
 *
 * \par Mandatory properties
 * Component::configuration
 *
 * \par API route
 * Not part of the News App API, uses general Nextcloud OCS API at /ocs/v2.php/core/apppassword
 *
 * \par Method
 * DELETE
 *
 * \headerfile "" <Fuoten/API/DeleteAppPassword>
 */
class FUOTEN_EXPORT DeleteAppPassword : public Component
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs a new %DeleteAppPassword object with the given \a parent.
     */
    explicit DeleteAppPassword(QObject *parent = nullptr);

    /*!
     * \brief Destroys the %DeleteAppPassword object.
     */
    ~DeleteAppPassword() override;

    /*!
     * Executes the API request.
     */
    Q_INVOKABLE void execute() override;

protected:
    DeleteAppPassword(DeleteAppPasswordPrivate &dd, QObject *parent = nullptr);

    /*!
     * \brief Simply emits Component::succeeded() signal.
     */
    void successCallback() override;

private:
    Q_DECLARE_PRIVATE(DeleteAppPassword)
    Q_DISABLE_COPY(DeleteAppPassword)
};

}

#endif // FUOTENDELETEAPPPASSWORD_H
