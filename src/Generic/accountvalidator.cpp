/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016 Buschtrommel / Matthias Fehring
 * https://www.buschmann23.de/entwicklung/bibliotheken/libfuoten/
 * https://github.com/Buschtrommel/libfuoten
 *
 * Generic/accountvalidator.cpp
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

#include "accountvalidator_p.h"
#include "../Helpers/configuration.h"
#include "../error.h"
#if QT_VERSION < QT_VERSION_CHECK(5, 6, 0)
#include "../Helpers/newsappversion.h"
#else
#include <QVersionNumber>
#endif

#ifdef QT_DEBUG
#include <QtDebug>
#endif

using namespace Fuoten;
using namespace Generic;


AccountValidator::AccountValidator(QObject *parent) :
    QObject(parent), d_ptr(new AccountValidatorPrivate(this))
{
}



AccountValidator::~AccountValidator()
{
}


void AccountValidator::start()
{
    Q_D(AccountValidator);

    setError(nullptr);

    d->setInOperatin(true);

    if (!configuration()) {
        //% "No configuration available."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("id-err-no-config"), QString(), this));
        Q_EMIT failed(error());
        return;
    }

    if (!d->version) {
        d->version = new Version(this);
        d->version->setConfiguration(configuration());
        connect(d->version, &Component::succeeded, this, &AccountValidator::gotVersion);
        connect(d->version, &Component::failed, this, &AccountValidator::setError);
    }

    d->version->get();
}



void AccountValidator::gotVersion()
{
#if QT_VERSION < QT_VERSION_CHECK(5, 6, 0)
    if (configuration()->getServerVersion().lowerThan(5,2,4)) {
        //% "The version of your News App is lower than 5.2.4. Status and user information can not be queried."
        setError(new Error(Error::ServerError, Error::Warning, qtTrId("id-err-version-low-status-user"), configuration()->getServerVersion().toString(), this));
        return;
    }
#else
    if (configuration()->getServerVersion() < QVersionNumber(5,2,4)) {
        //% "The version of your News App is lower than 5.2.4. Status and user information can not be queried."
        setError(new Error(Error::ServerError, Error::Warning, qtTrId("id-err-version-low-status-user"), configuration()->getServerVersion().toString(), this));
        return;
    }
#endif

    Q_D(AccountValidator);

    if (!d->status) {
        d->status = new Status(this);
        d->status->setConfiguration(configuration());
        connect(d->status, &Component::succeeded, this, &AccountValidator::gotStatus);
        connect(d->status, &Component::failed, this, &AccountValidator::setError);
    }

    d->status->get();
}


void AccountValidator::gotStatus()
{
#if QT_VERSION < QT_VERSION_CHECK(5, 6, 0)
    if (configuration()->getServerVersion().lowerThan(6,0,5)) {
        //% "The version of your News App is lower than 6.0.5. User information can not be queried."
        setError(new Error(Error::ServerError, Error::Warning, qtTrId("id-err-version-low-user"), configuration()->getServerVersion().toString(), this));
        return;
    }
#else
    if (configuration()->getServerVersion() < QVersionNumber(6,0,5)) {
        //% "The version of your News App is lower than 6.0.5. User information can not be queried."
        setError(new Error(Error::ServerError, Error::Warning, qtTrId("id-err-version-low-user"), configuration()->getServerVersion().toString(), this));
        return;
    }
#endif

    Q_D(AccountValidator);

    if (!d->user) {
        d->user = new User(this);
        d->user->setConfiguration(configuration());
        connect(d->user, &Component::succeeded, this, &AccountValidator::gotUser);
        connect(d->user, &Component::failed, this, &AccountValidator::setError);
    }

    d->user->get();
}


void AccountValidator::gotUser()
{
    Q_D(AccountValidator);
    d->setInOperatin(false);
    Q_EMIT succeeded();
}



Configuration *AccountValidator::configuration() const { Q_D(const AccountValidator); return d->configuration; }

void AccountValidator::setConfiguration(Configuration *nConfiguration)
{
    Q_D(AccountValidator); 
    if (nConfiguration != d->configuration) {
        d->configuration = nConfiguration;
#ifdef QT_DEBUG
        qDebug() << "Changed configuration to" << d->configuration;
#endif
        Q_EMIT configurationChanged(configuration());
    }
}




bool AccountValidator::inOperation() const { Q_D(const AccountValidator); return d->inOperation; }




Error *AccountValidator::error() const { Q_D(const AccountValidator); return d->error; }

void AccountValidator::setError(Error *nError)
{
    Q_D(AccountValidator);
    if (nError != d->error) {
        Error *old = d->error;
        d->error = nError;
        if (old && old->parent() == this) {
            delete old;
        }
#ifdef QT_DEBUG
        qDebug() << "Changed error to" << d->error;
#endif
        Q_EMIT errorChanged(error());
    }
    d->setInOperatin(false);
    if (d->error && (d->error->severity() == Error::Critical || d->error->severity() == Error::Fatal)) {
        Q_EMIT failed(error());
    } else {
        Q_EMIT succeeded();
    }
}


