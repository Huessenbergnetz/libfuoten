/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016-2020 Hüssenbergnetz/Matthias Fehring
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

#ifndef FUOTENGETSTATUS_H
#define FUOTENGETSTATUS_H

#include <QObject>
#include "component.h"
#include "../fuoten_global.h"

namespace Fuoten {

class GetStatusPrivate;

/*!
 * \brief Requests the status from the News App.
 *
 * The status reply will contain the News App version number and possible warnings about improperly configurations.
 * To request the status, set the \link Component::configuration configuration \endlink property and call execute().
 *
 * The requested data will be stored in the versionString, improperlyConfiguredCron and incorrectDbCharset properties.
 * You can get the raw JSON response from the Component::succeeded() signal. If the request failed for some reason,
 * Component::failed() will be emitted and the Component::inOperation property will contain a valid pointer to an Error object.
 *
 * \par Mandatory properties
 * Component::configuration
 *
 * \par API route
 * /status
 *
 * \par Method
 * GET
 *
 * \headerfile "" <Fuoten/API/GetStatus>
 */
class FUOTENSHARED_EXPORT GetStatus : public Component
{
    Q_OBJECT
    /*!
     * \brief This property holds the version of the Nextcoud News app as a string.
     *
     * The default value is an empty string. It will be set in the successCallback().
     *
     * \since 0.8.0
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>versionString() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>versionStringChanged(const QString versionString)</TD></TR></TABLE>
     */
    Q_PROPERTY(QString versionString READ versionString NOTIFY versionStringChanged)
    /*!
     * \brief Returns \c true when the Nextcloud News app will fail to update the feed correctly, otherwise \c false.
     *
     * The default value is \c false. It will be set in the successCallback().
     *
     * \since 0.8.0
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>impropertyConfiguredCron() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>improperlyConfiguredCronChanged(bool impropertyConfiguredCron)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool impropertyConfiguredCron READ improperlyConfiguredCron NOTIFY improperlyConfiguredCronChanged)
    /*!
     * \brief Returns \c true when the database charset of the Nextcloud News app is set up incorrectly, otherwise \c false.
     *
     * The default value is \c false. It will be set in the successCallback().
     *
     * \since 0.8.0
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>incorrectDbCharset() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>incorrectDbCharsetChanged(bool incorrectDbCharset)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool incorrectDbCharset READ incorrectDbCharset NOTIFY incorrectDbCharsetChanged)
public:
    /*!
     * \brief Constructs an API request object with the given \a parent to query the status from the remote server.
     */
    explicit GetStatus(QObject *parent = nullptr);

    /*!
     * \brief Destroys the %GetStatus object.
     */
    ~GetStatus() override;

    /*!
     * \brief Executes the API request.
     *
     * To perform a successful API request, Component::configuration has to be set to a valid AbstractConfiguration object.
     *
     * Execution will not run if Component::inOperation returns \c true and will itself set that property to \c true when start to perform
     * the request.
     */
    Q_INVOKABLE void execute() override;

    /*!
     * \brief Returns the version of the Nextcloud News app as a string.
     *
     * The default value is an empty string.
     *
     * \since 0.8.0
     * \sa versionString, versionStringChanged()
     */
    QString versionString() const;

    /*!
     * \brief Returns \c true if the webapp will fail to update the feed correctly.
     * \since 0.8.0
     * \sa improperlyConfiguredCron, improperlyConfiguredCronChanged()
     */
    bool improperlyConfiguredCron() const;

    /*!
     * \brief Returns \c true if the database charset is set up incorrectly.
     * \since 0.8.0
     * \sa incorrectDbCharset, incorrectDbCharsetChanged()
     */
    bool incorrectDbCharset() const;

Q_SIGNALS:
    /*!
     * \brief Emitted when the versionString value changed.
     *
     * This signal will be triggered in the successCallback().
     *
     * \since 0.8.0
     * \sa versionString, versionString()
     */
    void versionStringChanged(const QString &versionString);

    /*!
     * \brief Emitted when the improperlyConfiguredCron value changed.
     *
     * This signal will be triggered in the successCallback().
     *
     * \since 0.8.0
     * \sa improperlyConfiguredCron, improperlyConfiguredCron()
     */
    void improperlyConfiguredCronChanged(bool improperlyConfiguredCron);

    /*!
     * \brief Emitted when the incorrectDbCharset value changed.
     *
     * This signal will be triggered in the successCallback().
     *
     * \since 0.8.0
     * \sa incorrectDbCharset, incorrectDbCharset()
     */
    void incorrectDbCharsetChanged(bool incorrectDbCharset);

protected:
    GetStatus(GetStatusPrivate &dd, QObject *parent = nullptr);

    /*!
     * \brief Finishes the status request if it was successful.
     *
     * Will set the versionString, improperlyConfiguredCron and incorrectDbCharset properties. Afterwards
     * it willset Component::inOperation to \c false and emits the Component::succeeded() signal.
     */
    void successCallback() override;

    /*!
     * \brief Checks if \a version and \a warnings are present in the JSON reply.
     *
     * Will at first perform the checks from Component::checkOutput().
     */
    bool checkOutput() override;

private:
    Q_DECLARE_PRIVATE(GetStatus)
    Q_DISABLE_COPY(GetStatus)
    Q_DISABLE_MOVE(GetStatus)
};

}

#endif // FUOTENGETSTATUS_H
