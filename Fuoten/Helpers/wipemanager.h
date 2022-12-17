/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENWIPEMANAGER_H
#define FUOTENWIPEMANAGER_H

#include <QObject>
#include "../fuoten_global.h"
#include "fuoten_export.h"

namespace Fuoten {

class WipeManagerPrivate;
class AbstractConfiguration;
class AbstractStorage;
class AbstractNotificator;

/*!
 * \brief Handles and manages remote wipe requests.
 *
 * Coordinates the use of different classes and objects to perform remotely requested wipe
 * of application data on this device. It is used in the implementations of Component::extractError()
 * after the Nextcloud server replied with a 401 or 403 HTTP status code. If that happens,
 * checkForWipe() is called. It uses GetWipeStatus to check if a remote wipe is requested by the
 * server for this application. If a wipe is requested, it will use AbstractStorage::clearStorage()
 * and AbstractConfiguration::deleteAccount() to delete the local data. After the data has been
 * wiped it will use PostWipeSuccess to inform the Nextcloud server about the performed wipe.
 *
 * If no \link AbstractConfiguration configuration\endlink or no \link AbstractStorage storage\endlink
 * or no \link AbstractNotificator notificator\endlink has been set to this object, Component::defaultConfiguration(),
 * Component::defaultStorage() or Component::defaultNotifactor() will be used if they have been set.
 * At least a configuration is mandatory.
 *
 * To set a default wipe manager that will be used for all actions, use Component::setDefaultWipemanager().
 *
 * \since 0.8.0
 *
 * \sa PostWipeSuccess, GetWipeStatus
 *
 * \headerfile "" <Fuoten/Helper/WipeManager>
 */
class FUOTEN_EXPORT WipeManager : public QObject
{
    Q_OBJECT
    /*!
     * \brief Returns \c true while remote wipe is running.
     *
     * \par Access functions
     * \li bool inOperation() const
     *
     * \par Notifier signal
     * \li void inOperationChanged(bool inOperation)
     */
    Q_PROPERTY(bool inOperation READ inOperation NOTIFY inOperationChanged)
public:
    /*!
     * \brief Constructs a new %WipeManager object with the given \a parent.
     */
    explicit WipeManager(QObject *parent = nullptr);

    /*!
     * \brief Destroys the %WipeManager object.
     */
    ~WipeManager() override;

    /*!
     * \brief Returns \c true while remote wipe is running
     *
     * \sa WipeManager::inOperation, inOperationChanged()
     */
    bool inOperation() const;

    /*!
     * \brief Sets the \a configuration.
     *
     * If no configuration has been set, Component::defaultConfiguration() will be used.
     */
    void setConfiguration(AbstractConfiguration *configuration);

    /*!
     * \brief Sets the \a storage.
     *
     * If no storage has been set, Component::defaultStorage() will be used.
     */
    void setStorage(AbstractStorage *storage);

    /*!
     * \brief Sets the \a notificator.
     *
     * If no notificator has been set, Component::defaultNotificator() will be used.
     */
    void setNotificator(AbstractNotificator *notificator);

    /*!
     * \brief Call this to check if remote wipe should be performed.
     *
     * This will use GetWipeStatus to check if a remote wipe has been requested by the
     * Nextcloud server. If a wipe has been requested, the wipe will be performed and
     * the wipeStarted() signal will be emitted. If something fails, wipeFailed() will
     * be emitted. If the wipe has been performed, wipeDone() will be emitted.
     */
    void checkForWipe();

Q_SIGNALS:
    /*!
     * \brief Emitted if the \link WipeManager::inOperation inOperation\endlink property changes.
     */
    void inOperationChanged(bool inOperation);
    /*!
     * \brief Emitted if the wipe has been started.
     */
    void wipeStarted();
    /*!
     * \brief Emitted if the wipe has been failed.
     */
    void wipeFailed();
    /*!
     * \brief Emitted if the wipe has been done.
     */
    void wipeDone();

private:
    const QScopedPointer<WipeManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(WipeManager)
    Q_DISABLE_COPY(WipeManager)
};

}

#endif // FUOTENWIPEMANAGER_H
