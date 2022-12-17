/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTEN_CONFIG_H
#define FUOTEN_CONFIG_H

#include <QObject>
#include <QDateTime>
#include <QVersionNumber>
#include "../fuoten_global.h"
#include "../fuoten.h"
#include "fuoten_export.h"

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
class FUOTEN_EXPORT AbstractConfiguration : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs a new AbstractConfiguration object with the given \a parent.
     */
    explicit AbstractConfiguration(QObject *parent = nullptr);

    /*!
     * \brief Destroys the %AbstractConfiguration object.
     */
    ~AbstractConfiguration() override;

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
     * \since 0.8.0
     * \return user agent string
     */
    Q_INVOKABLE virtual QString getLoginFlowUserAgent() const;

    /*!
     * \brief Deletes all local account data.
     *
     * Reimplement this to delete the configuration entries for username, password, host, path,
     * port and current verion. Make sure to emit the accountDelted() signal after deleting the
     * data. The default version does nothing.
     */
    Q_INVOKABLE virtual void deleteAccount();

public Q_SLOTS:
    /*!
     * \brief Sets login credentials requested from the login flow API and returns \c true on success.
     *
     * See also https://docs.nextcloud.com/server/latest/developer_manual/client_apis/LoginFlow/index.html
     *
     * Reimplement this in a subclass. The default implementation does nothing.
     *
     * \since 0.8.0
     */
    virtual bool setLoginFlowCredentials(const QUrl &credentialUrl);

    /*!
     * \brief Sets the login credentials requested from the login flow v2 API and returns \c true on success.
     *
     * This has to be a valid JSON object requested from the polling endpoint.
     * See also https://docs.nextcloud.com/server/latest/developer_manual/client_apis/LoginFlow/index.html
     *
     * \since 0.8.0
     */
    bool setLoginFlowCredentials(const QByteArray &credentials);

    /*!
     * \brief Sets the login credentials requested from the login flow v2 API and returns \c true on success.
     *
     * This has to be a valid JSON object requested from the polling endpoint.
     * See also https://docs.nextcloud.com/server/latest/developer_manual/client_apis/LoginFlow/index.html
     *
     * \since 0.8.0
     */
    bool setLoginFlowCredentials(const QJsonDocument &credentials);

    /*!
     * \brief Sets the login credentials requested from the login flow v2 API and returns \c true on success.
     *
     * This has to be a valid JSON object requested from the polling endpoint.
     * See also https://docs.nextcloud.com/server/latest/developer_manual/client_apis/LoginFlow/index.html
     *
     * \since 0.8.0
     */
    bool setLoginFlowCredentials(const QJsonObject &credentials);

    /*!
     * \brief Sets the server data via \a url and returns \c true on success, otherwise \c false.
     *
     * This takes the scheme and the host and if available the port and the path from the \a url to
     * set them to setUseSSL(), setHost(), setServerPort() and setInstallPath().
     *
     * \since 0.8.0
     */
    bool setServerUrl(const QUrl &url);

    /*!
     * \brief Sets the server data via \a url and returns \c true on success, otherwise \c false.
     *
     * This takes the scheme and the host and if available the port and the path from the \a url to
     * set them to setUseSSL(), setHost(), setServerPort() and setInstallPath().
     *
     * \since 0.8.0
     */
    bool setServerUrl(const QString &url);

Q_SIGNALS:
    /*!
     * \brief Emitted when the account data has been deleted.
     *
     * Emit this signal in your implementation of deleteAccount() after the account data
     * has been deleted.
     */
    void accountDeleted();

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
