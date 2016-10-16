/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016 Buschtrommel / Matthias Fehring
 * https://www.buschmann23.de/entwicklung/bibliotheken/libfuoten/
 * https://github.com/Buschtrommel/libfuoten
 *
 * component.h
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

#ifndef COMPONENT_H
#define COMPONENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QSslError>
#include <QUrlQuery>
#include <QJsonDocument>
#include "fuoten_global.h"

class QNetworkReply;

namespace Fuoten {

class ComponentPrivate;
class Error;

/*!
 * \brief Base class for all API requests.
 */
class FUOTENSHARED_EXPORT Component : public QObject
{
    Q_OBJECT
    /*!
     * \brief Define a custom QNetowrkAccessManager to perform netowkr operations.
     *
     * When no custom QNetworkAccessManager is set, a new one will be created when sending the API
     * request via sendRequest(). The internal created network manager will be a child object of
     * this class. If you set your own custom QNetworkAccessManager, this will not automatically be
     * a child object of this class.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QNetworkAccessManager*</TD><TD>networkAccessManager() const</TD></TR><TR><TD>void</TD><TD>setNetworkAccessManager(QNetworkAccessManager *nNetworkAccessManager)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>networkAccessManagerChanged(QNetworkAccessManager *networkAccessManager)</TD></TR></TABLE>
     */
    Q_PROPERTY(QNetworkAccessManager *networkAccessManager READ networkAccessManager WRITE setNetworkAccessManager NOTIFY networkAccessManagerChanged)
    /*!
     * \brief Returns true while the request is in operation.
     *
     * \sa setInOperation()
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>inOperation() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>inOperationChanged(bool inOperation)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool inOperation READ inOperation NOTIFY inOperationChanged)
    /*!
     * \brief Timeout in seconds for network requests.
     *
     * If you set the timeout to 0 or lower, it wil be disabled. Default value: \a 120 \a seconds
     *
     * \par Access functions:
     * <TABLE><TR><TD>quint8</TD><TD>requestTimeout() const</TD></TR><TR><TD>void</TD><TD>setRequestTimeout(quint8 nRequestTimeout)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>requestTimeoutChanged(quint8 requestTimeout)</TD></TR></TABLE>
     */
    Q_PROPERTY(quint8 requestTimeout READ requestTimeout WRITE setRequestTimeout NOTIFY requestTimeoutChanged)
    /*!
     * \brief Pointer to an error object, if any error occured.
     *
     * If no error occured, it will return a \c nullptr. The error is set internally by setError().
     *
     * \par Access functions:
     * <TABLE><TR><TD>Error*</TD><TD>error() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>errorChanged(Error *error)</TD></TR></TABLE>
     */
    Q_PROPERTY(Error *error READ error NOTIFY errorChanged)
    /*!
     * \brief Set this to true to use SSL/TLS.
     *
     * The default is \c true.
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>useSSL() const</TD></TR><TR><TD>void</TD><TD>setUseSSL(bool nUseSSL)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>useSSLChanged(bool useSSL)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool useSSL READ useSSL WRITE setUseSSL NOTIFY useSSLChanged)
    /*!
     * \brief The hostname of the ownCloud/Nextcloud server.
     *
     * Set this to the hostname your cloud server is installed on. If for example your cloud is installed on \a https://cloud.example.com,
     * set the hostname to \c cloud.example.com, if your cloud is installed on \a https://example.com/mycloud/, set the hostname to \c example.com
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>host() const</TD></TR><TR><TD>void</TD><TD>setHost(const QString &nHost)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>hostChanged(const QString &host)</TD></TR></TABLE>
     */
    Q_PROPERTY(QString host READ host WRITE setHost NOTIFY hostChanged)
    /*!
     * \brief The port the server runs on.
     *
     * Only set this to another value than \c 0 if your server runs on a different port than the standard ports for HTTP or HTTPS.
     *
     * \par Access functions:
     * <TABLE><TR><TD>quint16</TD><TD>port() const</TD></TR><TR><TD>void</TD><TD>setPort(quint16 nPort)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>portChanged(quint16 port)</TD></TR></TABLE>
     */
    Q_PROPERTY(quint16 port READ port WRITE setPort NOTIFY portChanged)
    /*!
     * \brief Subdirectory the cloud is installed in.
     *
     * Set this to the path the ownCloud/Nextcloud is installed on your server. Start with a leading slash and omit a finishing slash.
     * If your cloud is installed for example on \a https://example.com/cloud/, than set the installPath to \c /cloud. If your cloud
     * is installed on \a https://cloud.example.com/, leave this property empty. Default: \a empty.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>installPath() const</TD></TR><TR><TD>void</TD><TD>setInstallPath(const QString &nInstallPath)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>installPathChanged(const QString &installPath)</TD></TR></TABLE>
     */
    Q_PROPERTY(QString installPath READ installPath WRITE setInstallPath NOTIFY installPathChanged)
    /*!
     * \brief Ignores all SSL errors. Be careful.
     *
     * Be careful when setting this to true. It will ignore all SSL errors and the sslErrors() signal will not be emitted. Default: \c false
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>ignoreSSLErrors() const</TD></TR><TR><TD>void</TD><TD>setIgnoreSSLErrors(bool nIgnoreSSLErrors)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>ignoreSSLErrorsChanged(bool ignoreSSLErrors)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool ignoreSSLErrors READ ignoreSSLErrors WRITE setIgnoreSSLErrors NOTIFY ignoreSSLErrorsChanged)
    /*!
     * \brief The username used for the authentication.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>username() const</TD></TR><TR><TD>void</TD><TD>setUsername(const QString &nUsername)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>usernameChanged(const QString &username)</TD></TR></TABLE>
     */
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)
    /*!
     * \brief The password used for the authentication.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>password() const</TD></TR><TR><TD>void</TD><TD>setPassword(const QString &nPassword)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>passwordChanged(const QString &password)</TD></TR></TABLE>
     */
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    /*!
     * \brief The user agent to use in the request.
     *
     * Default: Libfuoten CURRENT_VERSION
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>userAgent() const</TD></TR><TR><TD>void</TD><TD>setUserAgent(const QString &nUserAgent)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>userAgentChanged(const QString &userAgent)</TD></TR></TABLE>
     */
    Q_PROPERTY(QString userAgent READ userAgent WRITE setUserAgent NOTIFY userAgentChanged)
public:
    /*!
     * \brief Constructs a new Component object.
     */
    Component(QObject *parent = nullptr);

    /*!
     * \brief Deconstructs the Component object.
     */
    ~Component();

    /*!
     * \brief Defines the expected JSON type.
     */
    enum ExpectedJSONType {
        Empty   = 0,
        Array   = 1,
        Object  = 2
    };

    QNetworkAccessManager *networkAccessManager() const;
    bool inOperation() const;
    quint8 requestTimeout() const;
    Error *error() const;
    bool useSSL() const;
    QString host() const;
    quint16 port() const;
    QString installPath() const;
    bool ignoreSSLErrors() const;
    QString username() const;
    QString password() const;
    QString userAgent() const;

    void setNetworkAccessManager(QNetworkAccessManager *nNetworkAccessManager);
    void setRequestTimeout(quint8 nRequestTimeout);
    void setUseSSL(bool nUseSSL);
    void setHost(const QString &nHost);
    void setPort(quint16 nPort);
    void setInstallPath(const QString &nInstallPath);
    void setIgnoreSSLErrors(bool nIgnoreSSLErrors);
    void setUsername(const QString &nUsername);
    void setPassword(const QString &nPassword);
    void setUserAgent(const QString &nUserAgent);

Q_SIGNALS:
    void networkAccessManagerChanged(QNetworkAccessManager *networkAccessManager);
    void inOperationChanged(bool inOperation);
    void requestTimeoutChanged(quint8 requestTimeout);
    void errorChanged(Error *error);
    void useSSLChanged(bool useSSL);
    void hostChanged(const QString &host);
    void portChanged(quint16 port);
    void installPathChanged(const QString &installPath);
    void ignoreSSLErrorsChanged(bool ignoreSSLErrors);
    void usernameChanged(const QString &username);
    void passwordChanged(const QString &password);
    void userAgentChanged(const QString &userAgent);

    /*!
     * \brief This signal is emitted if the SSL/TLS session encountered errors during the set up.
     *
     * Will only be emitted if \link Component::ignoreSSLErrors ignoreSSLErrors \endlink is set to \c false (the default).
     */
    void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);

    /*!
     * \brief Emit this signal in a subclass when the request was successful.
     */
    void succeeded(const QJsonDocument &result);

    /*!
     * \brief Emit this signal in a subclass when the request failed for some reason.
     */
    void failed(Error *error);

protected:
    const QScopedPointer<ComponentPrivate> d_ptr;
    Component(ComponentPrivate &dd, QObject *parent = nullptr);

    /*!
     * \brief Sets the value of the \link Component::inOperation inOperation \endlink property.
     *
     * Use this in subclasses of Component to indicate, that the request is still running.
     */
    void setInOperation(bool nInOperation);

    /*!
     * \brief Sets the pointer of the \link Component::error error \endlink property.
     *
     * If there was already an error set, the old Error will be deleted.
     */
    void setError(Error *nError);

    /*!
     * \brief Sets the expected JSON type for initial outpt check.
     *
     * Default: Empty
     */
    void setExpectedJSONType(ExpectedJSONType type);

    /*!
     * \brief Sets the API route.
     */
    void setApiRoute(const QString &route);

    /*!
     * \brief Returns the JSON result document.
     */
    QJsonDocument jsonResult() const;

    /*!
     * \brief Performs basic input checks.
     *
     * Reimplement this in a subclass and call the parent's class implementation from there.
     */
    virtual bool checkInput();

    /*!
     * \brief Performs basic input checks.
     *
     * Reimplement this in a subclass and call the parent's class implementation from there.
     */
    virtual bool checkOutput();

    /*!
     * \brief Sets the operation the network manager should perform for this call.
     *
     * Default: QNetworkAccessManager::GetOperation
     */
    void setNetworkOperation(QNetworkAccessManager::Operation operation);

    /*!
     * \brief Returns the currently set HTTP headers for the request.
     *
     * \sa setRequestHeaders(), addRequestHeader(), addRequestHeaders()
     */
    QHash<QByteArray, QByteArray> requestHeaders() const;

    /*!
     * \brief Sets the headers to use for the HTTP request.
     *
     * \sa requestHeaders(), addRequestHeader(), addRequestHeaders()
     */
    void setRequestHeaders(const QHash<QByteArray, QByteArray> &headers);

    /*!
     * \brief Adds a header to the HTTP request.
     *
     * \sa addRequestHeaders(), setRequestHeaders(), requestHeaders()
     */
    void addRequestHeader(const QByteArray & headerName, const QByteArray &headerValue);

    /*!
     * \brief Adds headers to the HTTP request.
     *
     * \sa addRequestHeader(), setRequestHeaders(), requestHeaders()
     */
    void addRequestHeaders(const QHash<QByteArray, QByteArray> &headers);

    /*!
     * \brief Sets the payload for the request.
     */
    void setPayload(const QByteArray &payload);

    /*!
     * \brief Sets the URL query for the request.
     */
    void setUrlQuery(const QUrlQuery &query);

    /*!
     * \brief Reimplement this in a subclass to work on the request result.
     */
    virtual void successCallback() = 0;

    /*!
     * \brief Sends the request to the server.
     */
    void sendRequest();

    /*!
     * \brief Reimplement this in a subclass to extract errors from the request result.
     */
    virtual void extractError(QNetworkReply *reply) = 0;


private Q_SLOTS:
    void _requestFinished();
    void _requestTimedOut();
    void _ignoreSSLErrors(QNetworkReply *reply, const QList<QSslError> &errors);

private:
    Q_DISABLE_COPY(Component)
    Q_DECLARE_PRIVATE(Component)

};

}


#endif // COMPONENT_H
