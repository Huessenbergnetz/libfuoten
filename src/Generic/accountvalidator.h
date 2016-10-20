/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016 Buschtrommel / Matthias Fehring
 * https://www.buschmann23.de/entwicklung/bibliotheken/libfuoten/
 * https://github.com/Buschtrommel/libfuoten
 *
 * Generic/accountvalidator.h
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

#ifndef ACCOUNTVALIDATOR_H
#define ACCOUNTVALIDATOR_H

#include <QObject>
#include "../fuoten_global.h"

namespace Fuoten {

class Configuration;
class Error;

namespace Generic {

class AccountValidatorPrivate;

/*!
 * \brief Helper class to request News App version, status and user information in once.
 *
 * This should be used after the user account was setup or changed. Set the \link AccountValidator::configuration configuration \endlink property
 * and call start(). Calling start() will create and invoke Version, Status and User in a row.
 *
 * \headerfile "" <Fuoten/Generic/accountvalidator.h>
 * \sa Status, User, Version
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
     * \brief Contains a pointer to a Error object, if any error occured.
     *
     * \par Access functions:
     * <TABLE><TR><TD>Error*</TD><TD>error() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>errorChanged(Error *error)</TD></TR></TABLE>
     */
    Q_PROPERTY(Fuoten::Error *error READ error NOTIFY errorChanged)
public:
    /*!
     * \brief Constructs a new AccountValidator object.
     */
    explicit AccountValidator(QObject *parent = nullptr);

    /*!
     * \brief Deconstructs the AccountValidator object.
     */
    ~AccountValidator();

    /*!
     * \brief Starts the account validation.
     */
    Q_INVOKABLE void start();

    Configuration *configuration() const;
    bool inOperation() const;
    Error *error() const;

    void setConfiguration(Configuration *nConfiguration);

Q_SIGNALS:
    void configurationChanged(Configuration *configuration);
    void inOperationChanged(bool inOperation);
    void errorChanged(Error *error);

    /*!
     * \brief This signal will be emitted if the account validation has been succeeded.
     *
     * Will be emitted if at least the News App version can be queried.
     */
    void succeeded();

    /*!
     * \brief This signal will be emitted if an error occured.
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
}


#endif // ACCOUNTVALIDATOR_H
