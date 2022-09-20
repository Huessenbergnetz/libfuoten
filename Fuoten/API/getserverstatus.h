/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENGETSERVERSTATUS_H
#define FUOTENGETSERVERSTATUS_H

#include <QObject>
#include <QVersionNumber>
#include "component.h"
#include "../fuoten_global.h"

namespace Fuoten {

class GetServerStatusPrivate;

/*!
 * \brief Requests the server status from status.php.
 *
 * This is not really part of the News App API but a helper class. It should be especially
 * helpful to support the login flow. The request gets the information every Nextcloud server
 * publicly exports via its status.php file. That returns a JSON object containing some helpful
 * information about the Nextcloud instance like state of maintenance mode and current version.
 *
 * The result is exposed to the appropriate properties of this class as well as QJsonDocument via
 * the Component::succeeded() signal.
 *
 * \since 0.8.0
 *
 * \par Mandatory properties
 * Component::configuration
 *
 * \par API route
 * Not part of the News App API, will directly call status.php in the root of your nextcloud installation
 *
 * \par Method
 * GET
 *
 * \headerfile "" <Fuoten/API/GetServerStatus>
 */
class FUOTENSHARED_EXPORT GetServerStatus : public Component
{
    Q_OBJECT
    /*!
     * \brief Returns \c true when the Nextcloud is completely installed, otherwise \c false.
     *
     * The default value is \c false. It will be set in the successCallback().
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>isInstalled() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>isInstalledChanged(bool isInstalled)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool isInstalled READ isInstalled NOTIFY isInstalledChanged)
    /*!
     * \brief Returns \c true when the Nextcloud is in maintenance mode, otherwise \c false.
     *
     * The default value is \c false. It will be set in the successCallback().
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>isInMaintenance() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>isInMaintenanceChanged(bool isInMaintenance)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool isInMaintenance READ isInMaintenance NOTIFY isInMaintenanceChanged)
    /*!
     * \brief Returns \c true when the Nextcloud needs a database upgrade, otherwise \c false.
     *
     * The default value is \c false. It will be set in the successCallback().
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>needsDbUpgrade() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>needsDbUpgradeChanged(bool needsDbUpgrade)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool needsDbUpgrade READ needsDbUpgrade NOTIFY needsDbUpgradeChanged)
    /*!
     * \brief This property holds the version of the Nextcloud as string.
     *
     * The default value is an empty string. It will be set in the successCallback().
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>versionString() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>versionStringChanged(const QString &versionString)</TD></TR></TABLE>
     */
    Q_PROPERTY(QString versionString READ versionString NOTIFY versionStringChanged)
    /*!
     * \brief This property holds the edition of the Nextcloud.
     *
     * The default value is an empty string. It will be set in the successCallback().
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>edition() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>editionChanged(const QString &edition)</TD></TR></TABLE>
     */
    Q_PROPERTY(QString edition READ edition NOTIFY editionChanged)
    /*!
     * \brief This property holds the product name of the Nextcloud.
     *
     * The default value is an empty string. It will be set in the successCallback().
     *
     * \note
     * The default product name of a Nextcloud is “Nextcloud“ but can be changed by the administrators,
     * so do not rely on this value.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>productname() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>productnameChanged(const QString &productname)</TD></TR></TABLE>
     */
    Q_PROPERTY(QString productname READ productname NOTIFY productnameChanged)
    /*!
     * \brief Returns \c true when the Nextcloud has extended support, otherwise \c false.
     *
     * The default value is \c false. It will be set in the successCallback().
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>hasExtendedSupport() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>hasExtendedSupportChanged(bool hasExtendedSupport)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool hasExtendedSupport READ hasExtendedSupport NOTIFY hasExtendedSupportChanged)
    /*!
     * \brief This property describes the possibility of setting up an account for the Nextcloud.
     *
     * The default value is GetServerStatus::NotPossible. It will be set in the successCallback().
     * The value of this property will be calculated by the values of isInstalled(), isInMaintenance(),
     * needsDbUpgrade() and version().
     *
     * \par Access functions:
     * <TABLE><TR><TD>GetServerStatus::SetupPossible</TD><TD>setupPossible() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>setupPossibleChanged(GetServerStatus::SetupPossible setupPossible)</TD></TR></TABLE>
     */
    Q_PROPERTY(Fuoten::GetServerStatus::SetupPossible setupPossible READ setupPossible NOTIFY setupPossibleChanged)
public:
    /*!
     * \brief Constructs a new %GetServerStatus object with the given \a parent.
     */
    explicit GetServerStatus(QObject *parent = nullptr);

    /*!
     * \brief Deconstructs the %GetServerStatus object.
     */
    ~GetServerStatus() override;

    /*!
     * \brief This enum describes the setup possibility.
     */
    enum SetupPossible {
        NotPossible = 0,    /**< Setup is not possible, either the Nextcloud instance is not completely installed and/or it is in maintenance mode. */
        Manual      = 1,    /**< Only manual setup is possible. This means, that account setup via <A HREF="https://docs.nextcloud.com/server/latest/developer_manual/client_apis/LoginFlow/index.html" REL="external noopener">Login Flow</A> is not possible. */
        LoginFlow   = 2,    /**< Login Flow setup is possible. */
        LoginFlowV2 = 3     /**< Login Flow v2 setup is possible. */
    };
    Q_ENUM(SetupPossible)

    /*!
     * \brief Executes the API request to get the server status.
     */
    Q_INVOKABLE void execute() override;

    /*!
     * \brief Compares \a versionString with the requested Nextcloud version.
     *
     * Returns an integer less than, equal to, or greater than zero depending on wether
     * the requested Nextcloud version is less than, equal to, or greater than \a versionString.
     */
    Q_INVOKABLE int compareVersion(const QString &versionString) const;

    /*!
     * \brief Compares \a version with the requested Nextcloud version.
     *
     * Returns an integer less than, equal to, or greater than zero depending on wether
     * the requested Nextcloud version is less than, equal to, or greater than \a version.
     */
    int compareVersion(const QVersionNumber &version) const;

    /*!
     * \brief Returns \c true when the Nextcloud is completely installed, otherwise \c false.
     *
     * The default value for this is \c false.
     *
     * \sa isInstalled, isInstalledChanged()
     */
    bool isInstalled() const;
    /*!
     * \brief Returns \c true when the Nextcloud is in maintenance mode, otherwise \c false.
     *
     * The default value for this is \c false.
     *
     * \sa isInMaintenance, isInMaintenanceChanged()
     */
    bool isInMaintenance() const;
    /*!
     * \brief Returns \c true when the Nextcloud needs a database upgrade, otherwise \c false.
     *
     * The default value for this is \c false.
     *
     * \sa needsDbUpgrade, needsDbUpgradeChanged()
     */
    bool needsDbUpgrade() const;
    /*!
     * \brief Returns the version of the Nextcloud.
     *
     * The default value is a null version.
     *
     * \sa versionString(), versionChanged()
     */
    QVersionNumber version() const;
    /*!
     * \brief Returns the version of the Nextcloud as a string.
     *
     * The default value is an empty string.
     *
     * \sa versionString, version(), versionStringChanged()
     */
    QString versionString() const;
    /*!
     * \brief Returns the edition of the Nextcloud.
     *
     * The default value is an empty string.
     *
     * \sa edition, editionChanged()
     */
    QString edition() const;
    /*!
     * \brief Returns the product name of the Nextcloud.
     *
     * The default value is an empty string.
     *
     * \note
     * The default product name of a Nextcloud is “Nextcloud“ but can be changed by the administrators,
     * so do not rely on this value.
     *
     * \sa productname, productnameChanged()
     */
    QString productname() const;
    /*!
     * \brief Returns \c true when the Nextcloud has extended support, otherwise \c false.
     *
     * The default value is \c false. It will be set in the successCallback().
     *
     * \sa hasExtendedSupport, hasExtendedSupportChanged()
     */
    bool hasExtendedSupport() const;

    /*!
     * \brief This property describes the possibility of setting up an account for the Nextcloud.
     *
     * The default value is GetServerStatus::NotPossible. It will be set in the successCallback().
     * The value of this property will be calculated by the values of isInstalled(), isInMaintenance(),
     * needsDbUpgrade() and version().
     *
     * \sa setupPossible, setupPossibleChanged()
     */
    Fuoten::GetServerStatus::SetupPossible setupPossible() const;

Q_SIGNALS:
    /*!
     * \brief Emitted when the isInstalled value changes.
     *
     * This signal will be triggered in the successCallback().
     *
     * \sa isInstalled, isInstalled()
     */
    void isInstalledChanged(bool isInstalled);
    /*!
     * \brief Emitted when the isInMaintenance values changes.
     *
     * This signal will be triggered in the successCallback().
     *
     * \sa isInMaitenance, isInMaintenance()
     */
    void isInMaintenanceChanged(bool isInMaitenance);
    /*!
     * \brief Emitted when the needsDbUpgrade values changes.
     *
     * This signal will be triggered in the successCallback().
     *
     * \sa needsDbUpgrade, needsDbUpgrade()
     */
    void needsDbUpgradeChanged(bool needsDbUpgrade);
    /*!
     * \brief Emitted when the value of version() changes.
     *
     * This signal will be triggered in the successCallback().
     *
     * \sa version()
     */
    void versionChanged(const QVersionNumber &version);
    /*!
     * \brief Emitted when the value of versionString changes.
     *
     * This signal will be triggered in the successCallback().
     *
     * \sa versionString, versionString()
     */
    void versionStringChanged(const QString &versionString);
    /*!
     * \brief Emitted when the value of edition changes.
     *
     * This signal will be triggered in the successCallback().
     *
     * \sa edition, edition()
     */
    void editionChanged(const QString &edition);
    /*!
     * \brief Emitted when the value of productname changes.
     *
     * This signal will be triggered in the successCallback().
     *
     * \sa productname, productname()
     */
    void productnameChanged(const QString &productname);
    /*!
     * \brief Emitted when the value of hasExtendedSupport changes.
     *
     * This signal will be triggered in the successCallback().
     *
     * \sa hasExtendedSupport, hasExtendedSupport()
     */
    void hasExtendedSupportChanged(bool hasExtendedSupport);
    /*!
     * \brief Emitted when the value of setupPossible changes.
     *
     * This signal will be triggered in the successCallback().
     *
     * \sa setupPossible, setupPossible()
     */
    void setupPossibleChanged(Fuoten::GetServerStatus::SetupPossible setupPossible);

protected:
    GetServerStatus(GetServerStatusPrivate &dd, QObject *parent = nullptr);

    /*!
     * \brief Extracts the server response and sets the property values.
     */
    void successCallback() override;

private:
    Q_DECLARE_PRIVATE(GetServerStatus)
    Q_DISABLE_COPY(GetServerStatus)
    Q_DISABLE_MOVE(GetServerStatus)
};

}

#endif // FUOTENGETSERVERSTATUS_H
