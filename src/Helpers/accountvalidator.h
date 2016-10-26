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

#ifndef FUOTENACCOUNTVALIDATOR_H
#define FUOTENACCOUNTVALIDATOR_H

#include <QObject>
#include "../fuoten_global.h"

namespace Fuoten {

class Configuration;
class Error;

class AccountValidatorPrivate;

/*!
 * \brief Helper class to request News App version, status and user information in once.
 *
 * This should be used after the user account was setup or changed. Set the \link AccountValidator::configuration configuration \endlink property
 * and call start(). Calling start() will create and invoke Version, Status and User in a row.
 *
 * \headerfile "" <Fuoten/Helpers/AccountValidator>
 * \par Mandatory properties
 * AccountValidator::configuration
 * \sa GetStatus, GetUser, GetVersion
 */
class FUOTENSHARED_EXPORT AccountValidator : public QObject
{
    Q_OBJECT
    /*!
     * \brief Pointer to a Configuration object.
     *
     * It is mandatory to set this property to a valid object that contains the authantication data and server configuration.
     *
     * \par Access functions:
     * <TABLE><TR><TD>Configuration*</TD><TD>configuration() const</TD></TR><TR><TD>void</TD><TD>setConfiguration(Configuration *nConfiguration)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>configurationChanged(Configuration *configuration)</TD></TR></TABLE>
     */
    Q_PROPERTY(Fuoten::Configuration *configuration READ configuration WRITE setConfiguration NOTIFY configurationChanged)
    /*!
     * \brief Returns true while the validation is in operation.
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>inOperation() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>inOperationChanged(bool inOperation)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool inOperation READ inOperation NOTIFY inOperationChanged)
    /*!
     * \brief Pointer to an Error object, if any error occured, otherwise a \c nullptr.
     *
     * \par Access functions:
     * <TABLE><TR><TD>Error*</TD><TD>error() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>errorChanged(Error *error)</TD></TR></TABLE>
     */
    Q_PROPERTY(Fuoten::Error *error READ error NOTIFY errorChanged)
public:
    /*!
     * \brief Constructs a new AccountValidator object with the given \a parent.
     */
    explicit AccountValidator(QObject *parent = nullptr);

    /*!
     * \brief Deconstructs the AccountValidator object.
     */
    ~AccountValidator();

    /*!
     * \brief Starts the account validation.
     *
     * Will not start the validation process if inOperation returns \c true and will set it to \c true when it starts.
     */
    Q_INVOKABLE void start();

    /*!
     * \brief Returns a pointer to the currently set Configuration object.
     * \sa configuration
     */
    Configuration *configuration() const;

    /*!
     * \brief Returns \c true while the validation process is active.
     * \sa inOpertion
     */
    bool inOperation() const;

    /*!
     * \brief Returns a pointer to an Error object, if any error occures, otherwise a \c nullptr.
     * \sa error
     */
    Error *error() const;

    /*!
     * \brief Sets the pointer to a Configuration object.
     * \sa configuration
     */
    void setConfiguration(Configuration *nConfiguration);

Q_SIGNALS:
    /*!
     * \brief This signal is emitted when the pointer to a Configuration object changes.
     * \sa configuration
     */
    void configurationChanged(Configuration *configuration);
    /*!
     * \brief This signal is emitted when the operational status changes.
     * \sa inOperation
     */
    void inOperationChanged(bool inOperation);
    /*!
     * \brief This signal is emitted when the pointer to an Error object changes.
     * \sa error
     */
    void errorChanged(Error *error);

    /*!
     * \brief This signal is emitted if the account validation has been succeeded.
     *
     * Will be emitted if at least the News App version can be queried.
     */
    void succeeded();

    /*!
     * \brief This signal is emitted if an error occured.
     *
     * \sa \link AccountValidator::error error \endlink
     */
    void failed(Error *error);

protected:
    const QScopedPointer<AccountValidatorPrivate> d_ptr;

private Q_SLOTS:
    void setError(Error *nError);
    void gotVersion();
    void gotStatus();
    void gotUser();

private:
    Q_DISABLE_COPY(AccountValidator)
    Q_DECLARE_PRIVATE(AccountValidator)

};

}


#endif // FUOTENACCOUNTVALIDATOR_H
