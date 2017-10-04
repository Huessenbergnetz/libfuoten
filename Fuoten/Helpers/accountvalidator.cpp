/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016 Buschtrommel / Matthias Fehring
 * https://www.buschmann23.de/entwicklung/bibliotheken/libfuoten/
 * https://github.com/Buschtrommel/libfuoten
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
#include "abstractconfiguration.h"
#include "../API/component.h"
#include "../error.h"
#if QT_VERSION < QT_VERSION_CHECK(5, 6, 0)
#include "versionnumber.h"
#else
#include <QVersionNumber>
#endif


using namespace Fuoten;


AccountValidator::AccountValidator(QObject *parent) :
    QObject(parent), d_ptr(new AccountValidatorPrivate(this))
{
}



AccountValidator::~AccountValidator()
{
}


void AccountValidator::start()
{    
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Still in operation. Returning.");
        return;
    }

    qDebug("%s", "Start validating account data.");

    Q_D(AccountValidator);

    if (!d->configuration) {
        setConfiguration(Component::defaultConfiguration());
    }

    Q_ASSERT_X(d->configuration, "start account validation", "invalid configuration object");

    setError(nullptr);

    d->setInOperatin(true);

    if (!d->version) {
        d->version = new GetVersion(this);
        d->version->setConfiguration(configuration());
        connect(d->version, &Component::succeeded, this, &AccountValidator::gotVersion);
        connect(d->version, &Component::failed, this, &AccountValidator::setError);
    }

    d->version->execute();
}



void AccountValidator::gotVersion()
{
    if (configuration()->getServerVersion() < QVersionNumber(5,2,4)) {
        //% "The version of your News App is lower than 5.2.4. Status and user information can not be queried."
        setError(new Error(Error::ServerError, Error::Warning, qtTrId("id-err-version-low-status-user"), configuration()->getServerVersion().toString(), this));
        Q_EMIT succeeded();
        return;
    }

    Q_D(AccountValidator);

    if (!d->status) {
        d->status = new GetStatus(this);
        d->status->setConfiguration(configuration());
        connect(d->status, &Component::succeeded, this, &AccountValidator::gotStatus);
        connect(d->status, &Component::failed, this, &AccountValidator::setError);
    }

    d->status->execute();
}


void AccountValidator::gotStatus()
{
    if (configuration()->getServerVersion() < QVersionNumber(6,0,5)) {
        //% "The version of your News App is lower than 6.0.5. User information can not be queried."
        setError(new Error(Error::ServerError, Error::Warning, qtTrId("id-err-version-low-user"), configuration()->getServerVersion().toString(), this));
        Q_EMIT succeeded();
        return;
    }

    Q_D(AccountValidator);

    if (!d->user) {
        d->user = new GetUser(this);
        d->user->setConfiguration(configuration());
        connect(d->user, &Component::succeeded, this, &AccountValidator::gotUser);
        connect(d->user, &Component::failed, this, &AccountValidator::setError);
    }

    d->user->execute();
}


void AccountValidator::gotUser()
{
    Q_D(AccountValidator);
    d->setInOperatin(false);

    qDebug("%s", "Successfully validated account data.");

    Q_EMIT succeeded();
}



AbstractConfiguration *AccountValidator::configuration() const
{
    Q_D(const AccountValidator);
    AbstractConfiguration *_config = d->configuration;
    if (!_config) {
        _config = Component::defaultConfiguration();
    }
    return _config;
}

void AccountValidator::setConfiguration(AbstractConfiguration *nAbstractConfiguration)
{
    if (Q_UNLIKELY(inOperation())) {
        qWarning("Can not change property %s, still in operation.", "configuration");
        return;
    }

    Q_D(AccountValidator); 
    if (nAbstractConfiguration != d->configuration) {
        d->configuration = nAbstractConfiguration;
        qDebug("Changed configuration to %p.", d->configuration);
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
        qDebug("Changed error to %p.", d->error);
        Q_EMIT errorChanged(error());

        if (old && old->parent() == this) {
            delete old;
        }
    }
    d->setInOperatin(false);
    if (d->error && (d->error->severity() == Error::Critical || d->error->severity() == Error::Fatal)) {
        Q_EMIT failed(error());
    } else {
        Q_EMIT succeeded();
    }
}


