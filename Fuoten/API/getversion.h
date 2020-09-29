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

#ifndef FUOTENGETVERSION_H
#define FUOTENGETVERSION_H

#include <QObject>
#include <QVersionNumber>
#include "component.h"
#include "../fuoten_global.h"

namespace Fuoten {

class GetVersionPrivate;

/*!
 * \brief Requests the installed News App version from the server.
 *
 * The version reply will only contain the version number of the installed News App.
 * To request the version information, set the \link Component::configuration configuration \endlink property and call execute().
 *
 * You can get the raw JSON response from the Component::succeeded() signal. If something failed, the Component::failed() signal
 * will be emitted and Component::error will contain a valid pointer to an Error object.
 *
 * \par Mandatory properties
 * Component::configuration
 *
 * \par API route
 * /version
 *
 * \par Method
 * GET
 *
 * \sa AccountValidator
 * \headerfile "" <Fuoten/API/GetVersion>
 */
class FUOTENSHARED_EXPORT GetVersion : public Component
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
public:
    /*!
     * \brief Constructs an API request object with the given \a parent to query the News App version from the remote server.
     */
    explicit GetVersion(QObject *parent = nullptr);

    /*!
     * \brief Destroys the %GetVersion object.
     */
    ~GetVersion() override;

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
     * \brief Returns the version of the Nextcloud News app.
     *
     * The default value is a null version.
     *
     * \since 0.8.0
     * \sa versionChanged()
     */
    QVersionNumber version() const;

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
     * \brief Emitted when the version value changed.
     *
     * This signal will be triggered in the successCallback().
     *
     * \since 0.8.0
     * \sa version()
     */
    void versionChanged(const QVersionNumber &version);

protected:
    GetVersion(GetVersionPrivate &dd, QObject *parent = nullptr);

    /*!
     * \brief Finishes the version request if it was successful.
     *
     * Will set the versionString property and emit the versionStringChanged()
     * and versionChanged() signals.
     */
    void successCallback() override;

    /*!
     * \brief Checks for \a version in the replied JSON object.
     *
     * Will at first perform the checks from Component::checkOutput().
     */
    bool checkOutput() override;

private:
    Q_DECLARE_PRIVATE(GetVersion)
    Q_DISABLE_COPY(GetVersion)
    Q_DISABLE_MOVE(GetVersion)
};

}

#endif // FUOTENGETVERSION_H
