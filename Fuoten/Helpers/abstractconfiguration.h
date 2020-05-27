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

#ifndef FUOTEN_CONFIG_H
#define FUOTEN_CONFIG_H

#include <QSettings>
#include <QDateTime>
#include <QVersionNumber>
#include "../fuoten_global.h"
#include "../fuoten.h"

class QUrl;
class QJsonDocument;
class QJsonObject;

namespace Fuoten {

/*!
 * \brief Server and authentication settings abstract base class.
 *
 * In order to use libfuoten, you have to reimplement this class, especially all pure
 * virtual functions that provide the necessary data to perform API requests.
 *
 * The reimplemented class will than be used on all API call classes. All of the
 * API classes have a configuration property that takes subclasses of AbstractConfiguration.
 *
 * \headerfile "" <Fuoten/Helpers/AbstractConfiguration>
 */
class FUOTENSHARED_EXPORT AbstractConfiguration : public QSettings
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs a new AbstractConfiguration object with the given \a parent.
     */
    explicit AbstractConfiguration(QObject *parent = nullptr);

    /*!
     * \brief Returns the user name used for authentication.
     *
     * Reimplement this function in a subclass.
     */
    virtual QString getUsername() const = 0;

    /*!
     * \brief Sets the \a username used for authentication.
     *
     * Reimplement this in a subclass. The default implementation does nothing.
     *
     * \param username used for authentication
     */
    virtual void setUsername(const QString &username);

    /*!
     * \brief Returns the password used for authentiction.
     *
     * Reimplement this function in a subclass.
     */
    virtual QString getPassword() const = 0;

    /*!
     * \brief Sets the \a password used for authentication.
     *
     * Reimplement this in a subclass. The default implementation does nothing.
     *
     * \param password used for authentication
     */
    virtual void setPassword(const QString &password);

    /*!
     * \brief Returns true if the connection should use SSL/TLS.
     *
     * The default implementation returns true.
     */
    virtual bool getUseSSL() const;

    /*!
     * \brief Set \a useSSL to \c true to let the connection use SSL/TLS.
     *
     * Reimplement this in a subclass. The default implementation does nothing.
     */
    virtual void setUseSSL(bool useSSL);

    /*!
     * \brief Returns the host the ownCloud/Nextcloud is installed on.
     *
     * Reimplement this function in a subclass.
     */
    virtual QString getHost() const = 0;

    /*!
     * \brief Sets the \a host the remote Nextcloud instance runs on.
     *
     * Reimplement this in a subclass. The default implementation does nothing.
     */
    virtual void setHost(const QString &host);

    /*!
     * \brief Returns the optional installation path of the ownCloud/Nextcloud on the server.
     *
     * Reimplement this function in a subclass.
     */
    virtual QString getInstallPath() const = 0;

    /*!
     * \brief Sets the \a path under which the remote Nextcloud instance is installed.
     *
     * Reimplement this in a subclass. The default implementation does nothing.
     */
    virtual void setInstallPath(const QString &path);

    /*!
     * \brief Returns true if the account data is valid.
     *
     * Reimplement this function in a subclass. You can use checkAccountValidity() to perform a simple check
     * if the important values are not empty.
     *
     * \sa setIsAccountValid(), checkAccountValidity()
     */
    virtual bool isAccountValid() const = 0;

    /*!
     * \brief Returns the News App version number used on the server.
     *
     * \sa setServerVersion()
     */
    virtual QVersionNumber getServerVersion() const = 0;

    /*!
     * \brief Returns the server port used for the connection.
     *
     * The default implementation returns 0, what means that the connection should use the default port
     * for HTTP or HTTPS.
     */
    virtual int getServerPort() const;

    /*!
     * \brief Sets the server \a port used for the connection.
     *
     * Reimplement this in a subclass. The default implementation does nothing.
     */
    virtual void setServerPort(int port);

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
     *
     * \param nDisplayName  the user's display name
     */
    virtual void setDisplayName(const QString &nDisplayName);

    /*!
     * \brief Set this to true if the server's cron job is improperly configured.
     *
     * The default implementation does nothing.
     *
     * \param nImproperlyConfiguredCron \c true if the server side CRON is imporperly configured, otherwise \c false
     */
    virtual void setImproperlyConfiguredCron(bool nImproperlyConfiguredCron);

    /*!
     * \brief Sets the version of the News App used on the server.
     *
     * Reimplement this in a subclass.
     *
     * \param nServerVersion    the server version string
     */
    virtual void setServerVersion(const QString &nServerVersion) = 0;

    /*!
     * \brief Sets the user's avatar.
     *
     * Reimplement this in a subclass if you want to handle the user's avatar image returned by Fuoten::Generic::Status.
     * \a data will be a base64 encoded data string representing the image data. \a mime will contain the image's mime type.
     *
     * The default implementation does nothing.
     *
     * \param data  base64 encoded image data
     * \param mime  MIME type of the image
     */
    virtual void setAvatar(const QString &data, const QString &mime);

    /*!
     * \brief Returns the last date and time in UTC a successful snychronization has been performed.
     *
     * Reimplement this in a subclass if you want let the Synchronizer save the last sync time.
     * The default implementation returns a invalid QDateTime. Your implementation should also
     * return an invalid QDateTime if there has been no last sync.
     *
     * \sa setLastSync()
     */
    virtual QDateTime getLastSync() const;

    /*!
     * \brief Sets the data and time in UTC the last successfule synchronization has been performed.
     *
     * Reimplement this in a subclass if you want let the Synchronizer save the last sync time.
     * The default implementation does nothing.
     *
     * \sa getLastSync()
     * \param syncTime  the date and time the last/current synchronization has successfully finished
     */
    virtual void setLastSync(const QDateTime &syncTime);

    /*!
     * \brief Returns the item deletion strategy for the feed identified by \a feedId.
     *
     * The default implementation returns \c FuotenEnums::NoItemDeletion.
     *
     * \param feedId    ID of the feed to get the deletion strategy for
     */
    virtual FuotenEnums::ItemDeletionStrategy getPerFeedDeletionStrategy(qint64 feedId) const;

    /*!
     * \brief Returns the deletion value for the feed identified by \a feedId.
     *
     * If getPerFeedDeletionStrategy() returns FuotenEnums::DeleteItemsByTime, the returned value should be
     * treated as older than \a n days. If for example \c 5 is returned, all items in the feed that are older
     * than \c 5 days should be removed. Reference time should be the publication time.
     *
     * If getPerFeedDeletionStrategy() return FuotenEnums::DeleteItemsByCount, the returned value should be
     * treated as numer of items in the feed to keep. If for example \c 200 is returned, so many items should
     * be deleted that after the deletion the feed contains \c 200 items.
     *
     * The default implementation returns \c 0.
     *
     * \param feedId    ID of the feed to get the deletion value for
     */
    virtual quint16 getPerFeedDeletionValue(qint64 feedId) const;

    /*!
     * \brief Returns a user agent string usable for the login flow.
     *
     * In the login flow API, the user agent is used to show the user the application that requests a new
     * application password and will also be used for the identifier of the generated application password.
     *
     * See also https://docs.nextcloud.com/server/latest/developer_manual/client_apis/LoginFlow/index.html
     *
     * Reimplement this in a subclass. The default implementation simply returns the same as getUserAgent(),
     * the string "Libfuoten $VERSION" and that is not really meaningful to a user.
     *
     * \return user agent string
     */
    virtual QString getLoginFlowUserAgent() const;

public Q_SLOTS:
    /*!
     * \brief Sets login credentials requested from the login flow API.
     *
     * See also https://docs.nextcloud.com/server/latest/developer_manual/client_apis/LoginFlow/index.html
     *
     * Reimplement this in a subclass. The default implementation does nothing.
     *
     * \param credentialUrl the URL returned by the login flow API with the nc:// scheme
     * \return \c true on success
     */
    virtual bool setLoginFlowCredentials(const QUrl &credentialUrl);

    /*!
     * \brief Sets the login credentials requested from the login flow v2 API.
     *
     * This has to be a valid JSON object requested from the polling endpoint.
     * See also https://docs.nextcloud.com/server/latest/developer_manual/client_apis/LoginFlow/index.html
     *
     * \param credentials JSON object
     * \return \c true on success
     */
    virtual bool setLoginFlowCredentials(const QByteArray &credentials);

    /*!
     * \brief Sets the login credentials requested from the login flow v2 API.
     *
     * This has to be a valid JSON object requested from the polling endpoint.
     * See also https://docs.nextcloud.com/server/latest/developer_manual/client_apis/LoginFlow/index.html
     *
     * \param credentials JSON object
     * \return \c true on success
     */
    virtual bool setLoginFlowCredentials(const QJsonDocument &credentials);

    /*!
     * \brief Sets the login credentials requested from the login flow v2 API.
     *
     * This has to be a valid JSON object requested from the polling endpoint.
     * See also https://docs.nextcloud.com/server/latest/developer_manual/client_apis/LoginFlow/index.html
     *
     * \param credentials JSON object
     * \return \c true on success
     */
    virtual bool setLoginFlowCredentials(const QJsonObject &credentials);

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

    /*!
     * \brief Set to true if the account data is valid.
     *
     * Reimplement this in a subclass. You can use checkAccountValidity() to perform a simple check if the
     * important data is not empty.
     *
     * \sa isAccountValid(), checkAccountValidity()
     */
    virtual void setIsAccountValid(bool nIsAccountValid) = 0;

private:
    Q_DISABLE_COPY(AbstractConfiguration)

};

}

#endif // FUOTEN_CONFIG_H
