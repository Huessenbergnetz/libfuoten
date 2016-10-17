/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016 Buschtrommel / Matthias Fehring
 * https://www.buschmann23.de/entwicklung/bibliotheken/libfuoten/
 * https://github.com/Buschtrommel/libfuoten
 *
 * helpers/configuration.h
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

#ifndef FUOTEN_CONFIG_H
#define FUOTEN_CONFIG_H

#include <QSettings>
#if QT_VERSION < QT_VERSION_CHECK(5, 6, 0)
#include "newsappversion.h"
#else
#include <QVersionNumber>
#endif
#include "../fuoten_global.h"

namespace Fuoten {

/*!
 * \brief Server and authentication settings abstract base class.
 *
 * In order to use libfuoten, you have to reimplement this class, especially all pure
 * virtual functions that provide the necessary data to perform API requests.
 *
 * The reimplemented class will than be used on all API call classes. All of the
 * API classes have a configuration property that takes subclasses of Configuration.
 *
 * \headerfile "" <Fuoten/Helpers/configuration.h>
 */
class FUOTENSHARED_EXPORT Configuration : public QSettings
{
    Q_OBJECT
public:
    Configuration(QObject *parent = nullptr);

    /*!
     * \brief Returns the user name used for authentication.
     *
     * Reimplement this function in a subclass.
     */
    virtual QString getUsername() const = 0;

    /*!
     * \brief Returns the password used for authentiction.
     *
     * Reimplement this function in a subclass.
     */
    virtual QString getPassword() const = 0;

    /*!
     * \brief Returns true if the connection should use SSL/TLS.
     *
     * The default implementation returns true.
     */
    virtual bool getUseSSL() const;

    /*!
     * \brief Returns the host the ownCloud/Nextcloud is installed on.
     *
     * Reimplement this function in a subclass.
     */
    virtual QString getHost() const = 0;

    /*!
     * \brief Returns the optional installation path of the ownCloud/Nextcloud on the server.
     *
     * Reimplement this function in a subclass.
     */
    virtual QString getInstallPath() const = 0;

    /*!
     * \brief Returns true if the account data is valid.
     *
     * Reimplement this function in a subclass. You can use checkAccountValidity() to perform a simple check
     * if the important values are not empty.
     *
     * \sa setIsAccountValid(), checkAccountValidity()
     */
    virtual bool isAccountValid() const = 0;

#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
    /*!
     * \brief Returns the News App version number used on the server.
     *
     * When building on Qt 5.6 or newer, this returns a QVersionNumber, otherwise it returns a NewsAppVersion.
     *
     * \sa setServerVersion()
     */
    virtual QVersionNumber getServerVersion() const = 0;
#else
    virtual NewsAppVersion getServerVersion() const = 0;
#endif

    /*!
     * \brief Returns the server port used for the connection.
     *
     * The default implementation returns 0, what means that the connection should use the default port
     * for HTTP or HTTPS.
     */
    virtual int getServerPort() const;

    /*!
     * \brief Returns true if SSL/TLS errors should be ignored.
     *
     * The default implementation returns false.
     */
    virtual bool getIgnoreSSLErrors() const;

    /*!
     * \brief Returns the user agent to send as request header.
     *
     * The default implementation returns "Libfuoten VERSION"
     */
    virtual QString getUserAgent() const;

    /*!
     * \brief Sets the user's display name.
     *
     * The default implementation does nothing.
     */
    virtual void setDisplayName(const QString &nDisplayName);

    /*!
     * \brief Set this to true if the server's cron job is improperly configured.
     *
     * The default implementation does nothing.
     */
    virtual void setImproperlyConfiguredCron(bool nImproperlyConfiguredCron);

    /*!
     * \brief Sets the version of the News App used on the server.
     *
     * Reimplement this in a subclass.
     */
    virtual void setServerVersion(const QString &nServerVersion) = 0;

    /*!
     * \brief Set to true if the account data is valid.
     *
     * Reimplement this in a subclass. You can use checkAccountValidity() to perform a simple check if the
     * important data is not empty.
     *
     * \sa isAccountValid(), checkAccountValidity()
     */
    virtual void setIsAccountValid(bool nIsAccountValid) = 0;

    /*!
     * \brief Sets the user's avatar.
     *
     * Reimplement this in a subclass if you want to handle the user's avatar image returned by Fuoten::Generic::Status.
     * \a data will be a base64 encoded data string representing the image data. \a mime will contain the image's mime type.
     *
     * The default implementation does nothing.
     */
    virtual void setAvatar(const QString &data, const QString &mime);

protected:
    /*!
     * \brief Performs a simple check on the important account data.
     *
     * This will check if username, password and host are not empty. You can use this function to
     * set the account validity. The result of the check will be set to setIsAccountValid().
     *
     * \sa setIsAccountValid(), isAccountValid()
     */
    void checkAccountValidity();

private:
    Q_DISABLE_COPY(Configuration)

};

}

#endif // FUOTEN_CONFIG_H
