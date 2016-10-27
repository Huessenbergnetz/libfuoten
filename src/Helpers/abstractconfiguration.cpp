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

#include "abstractconfiguration.h"

using namespace Fuoten;

AbstractConfiguration::AbstractConfiguration(QObject *parent) : QSettings(parent)
{

}



void AbstractConfiguration::checkAccountValidity()
{
    setIsAccountValid((!getUsername().isEmpty() && !getPassword().isEmpty() && !getHost().isEmpty() && !getServerVersion().isNull()));
}


QString AbstractConfiguration::getUserAgent() const
{
    return QStringLiteral("Libfuoten %1").arg(QStringLiteral(VERSION_STRING));
}



bool AbstractConfiguration::getUseSSL() const
{
    return true;
}



int AbstractConfiguration::getServerPort() const
{
    return 0;
}



bool AbstractConfiguration::getIgnoreSSLErrors() const
{
    return false;
}



void AbstractConfiguration::setAvatar(const QString &data, const QString &mime)
{
    Q_UNUSED(data)
    Q_UNUSED(mime)
}


void AbstractConfiguration::setDisplayName(const QString &nDisplayName)
{
    Q_UNUSED(nDisplayName)
}



void AbstractConfiguration::setImproperlyConfiguredCron(bool nImproperlyConfiguredCron)
{
    Q_UNUSED(nImproperlyConfiguredCron)
}
