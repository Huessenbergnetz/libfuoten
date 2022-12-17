/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENLOGINFLOWV2_H
#define FUOTENLOGINFLOWV2_H

#include <QObject>
#include "component.h"
#include "../fuoten_global.h"
#include "fuoten_export.h"

namespace Fuoten {

class LoginFlowV2Private;

/*!
 * \brief Manages the Login Flow v2 to request an application password.
 *
 * Nextcloud’s Login Flow can be used to request login credentials specific to every
 * single client. This will assure that each client gets its own set of credentials and
 * has the advantage that a client never stores the password of the user and that the
 * user can revoke access on a per client basis from the web.
 *
 * This class helps to use the Login Flow v2 that uses an external browser window (normally
 * the user’s default browser) to request login credentials. The Nextcloud server to request
 * the credentials from is read from Component::configuration, so there has to be at least
 * the host information be set. You should also reimplement AbstractConfiguration::getLoginFlowUserAgent()
 * because it is used to show the user a descriptive name of the application that requests the
 * login credentials.
 *
 * The first request will be sent to <server>/index.php/login/v2 to get the login url opened
 * in the external browser and to the polling endpoint and the polling token. If that was
 * successful, gotLoginUrl() will be emitted that will also have the login url for the user.
 *
 * Directly after emitting gotLoginUrl() the polling for the credentials starts while the user
 * authorizes the application. If the authorization succeedes, the requested credentials will
 * be stored with AbstractConfiguration::setLoginFlowCredentials(), also the Component::succeeded()
 * signal will be emitted containing the retreived JSON credential data.
 *
 * If something failed, Component::failed() will be emitted and Component::error() will contain the
 * Error data object.
 *
 * \since 0.8.0
 *
 * \par Mandatory properties
 * Component::configuration
 *
 * \par API route
 * Not part of the News App API, uses general Nextcloud API at /index.php/login/v2
 *
 * \par Method
 * POST
 *
 * \headerfile "" <Fuoten/API/LoginFlowV2>
 */
class FUOTEN_EXPORT LoginFlowV2 : public Component
{
    Q_OBJECT
    /*!
     * \brief This property holds the seconds left until the \link LoginFlowV2::pollingTimeout pollingTimeout\endlink timer times out.
     *
     * The default value is \c 0. While polling runs, secondsLeftChanged() will be emitted every second.
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>secondsLeft() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>secondsLeftChanged(int secondsLeft)</TD></TR></TABLE>
     *
     * \sa pollingTimeout
     */
    Q_PROPERTY(int secondsLeft READ secondsLeft NOTIFY secondsLeftChanged)
    /*!
     * \brief Sets the interval in seconds at which polling requests are sent to the server.
     *
     * The default value is 2 seconds. Unless the pollingTimeout timer times out or the authorization
     * succeeded, n seconds after the last successful polling request, a new polling request will be sent.
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>pollingInterval() const</TD></TR><TR><TD>void</TD><TD>setPollingInterval(int pollingInterval)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>pollingIntervalChanged(int pollingInterval)</TD></TR></TABLE>
     */
    Q_PROPERTY(int pollingInterval READ pollingInterval WRITE setPollingInterval NOTIFY pollingIntervalChanged)
    /*!
     * \brief Sets the timeout in seconds at which the authorisation request has to be succeeded.
     *
     * The default value is 300 seconds. If this timeout expires, the login flow will be canceled
     * and polling will be stopped. Component::error() will return a valid Error object and
     * the Component::failed() signal will be emitted.
     *
     * This property can not be changed while Component::inOperation() returns \c true.
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>pollingTimeout() const</TD></TR><TR><TD>void</TD><TD>setPollingTimeout(int pollingTimeout)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>pollingTimeoutChanged(int pollingTimeout)</TD></TR></TABLE>
     *
     * \sa secondsLeft
     */
    Q_PROPERTY(int pollingTimeout READ pollingTimeout WRITE setPollingTimeout NOTIFY pollingTimeoutChanged)
    /*!
     * \brief Sets the timeout in seconds for single polling requests.
     *
     * The default value is 5 seconds. Every single polling request will time out after the set seconds
     * if it does not receive an answer. After 5 consecutive timeouts, the login flow will be canceled,
     * Component::error() will return a valid Error object and the Component::failed() signal will be
     * emitted.
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>pollingRequestTimeout() const</TD></TR><TR><TD>void</TD><TD>setPollingRequestTimeout(int pollingRequestTimeout)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>pollingRequestTimeoutChanged(int pollingRequestTimeout)</TD></TR></TABLE>
     */
    Q_PROPERTY(int pollingRequestTimeout READ pollingRequestTimeout WRITE setPollingRequestTimeout NOTIFY pollingRequestTimeoutChanged)
public:
    /*!
     * \brief Constructs a new %LoginFlowV2 object with the given \a parent.
     */
    explicit LoginFlowV2(QObject *parent = nullptr);

    /*!
     * \brief Destroys the %LoginFlowV2 object.
     */
    ~LoginFlowV2() override;

    /*!
     * \brief Executes the API request.
     */
    Q_INVOKABLE void execute() override;

    /*!
     * \brief Returns the remaining seconds until pollingTimeout times out.
     *
     * The default value is \c 0.
     *
     * \sa secondsLeft, secondsLeftChanged()
     */
    int secondsLeft() const;

    /*!
     * \brief Returns the polling interval in seconds.
     *
     * \sa pollingInterval, setPollingInterval(), pollingIntervalChanged()
     */
    int pollingInterval() const;

    /*!
     * \brief Sets the polling interval in seconds.
     *
     * \sa pollingInterval, pollingInterval(), pollingIntervalChanged()
     */
    void setPollingInterval(int pollingInterval);

    /*!
     * \brief Returns the polling timeout in seconds.
     *
     * \sa pollingTimeout, setPollingTimeout(), pollingTimeoutChanged()
     */
    int pollingTimeout() const;

    /*!
     * \brief Sets the polling timeout in seconds.
     *
     * \sa pollingTimeout, pollingTimeout(), pollingTimeoutChanged()
     */
    void setPollingTimeout(int pollingTimeout);

    /*!
     * \brief Returns the timeout for the single polling requests in seconds.
     *
     * \sa pollingRequestTimeout, setPollingRequestTimeout(), pollingRequestTimeoutChanged()
     */
    int pollingRequestTimeout() const;

    /*!
     * \brief Set the timeout for the single polling requests in seconsd.
     *
     * \sa pollingRequestTimeout, pollingRequestTimeout(), pollingRequestTimeoutChanged();
     */
    void setPollingRequestTimeout(int pollingRequestTimeout);

Q_SIGNALS:
    /*!
     * \brief Emitted when the login url has been requested from the Nextcloud server.
     *
     * The emitted \a url should be opened by a web browser to load and show the authorization
     * login page to the user. This signal is emitted in the successCallback().
     */
    void gotLoginUrl(const QUrl &url);
    /*!
     * \brief Emitted every seconds when the polling is active.
     *
     * The \a %secondsLeft are the remaining seconds until the pollingTimeout times out.
     *
     * \sa secondsLeft, secondsLeft()
     */
    void secondsLeftChanged(int secondsLeft);
    /*!
     * \brief Emitted when the \link LoginFlowV2::pollingInterval pollingInterval\endlink has been changed.
     *
     * \sa pollingInterval, pollingInterval(), setPollingInterval()
     */
    void pollingIntervalChanged(int pollingInterval);
    /*!
     * \brief Emitted when the \link LoginFlowV2::pollingTimeout pollingTimeout\endlink has been changed.
     *
     * \sa pollingTimeout, pollingTimeout(), setPollingTimeout()
     */
    void pollingTimeoutChanged(int pollingTimeout);
    /*!
     * \brief Emitted when the \link LoginFlowV2::pollingRequestTimeout pollingRequestTimeout\endlink has been changed.
     *
     * \sa pollingRequestTimeout, pollingRequestTimeout(), setPollingRequestTimeout()
     */
    void pollingRequestTimeoutChanged(int pollingRequestTimeout);

protected:
    LoginFlowV2(LoginFlowV2Private &dd, QObject *parent = nullptr);

    /*!
     * \brief Always returns \c true.
     */
    bool checkInput() override;

    /*!
     * \brief Extracts the login url and the polling data from the initial authorisation request.
     *
     * If the returned data is valid, gotLoginUrl() will be emitted an the polling for the application
     * password start immediatly.
     */
    void successCallback() override;

private:
    Q_DECLARE_PRIVATE(LoginFlowV2)
    Q_DISABLE_COPY(LoginFlowV2)
};

}

#endif // FUOTENLOGINFLOWV2_H
