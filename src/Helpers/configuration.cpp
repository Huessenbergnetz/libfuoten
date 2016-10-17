/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016 Buschtrommel / Matthias Fehring
 * https://www.buschmann23.de/entwicklung/bibliotheken/libfuoten/
 * https://github.com/Buschtrommel/libfuoten
 *
 * helpers/configuration.cpp
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

#include "configuration.h"

using namespace Fuoten;

Configuration::Configuration(QObject *parent) : QSettings(parent)
{

}



void Configuration::checkAccountValidity()
{
    setIsAccountValid((!getUsername().isEmpty() && !getPassword().isEmpty() && !getHost().isEmpty() && !getServerVersion().isNull()));
}


QString Configuration::getUserAgent() const
{
    return QStringLiteral("Libfuoten %1").arg(QStringLiteral(VERSION_STRING));
}



bool Configuration::getUseSSL() const
{
    return true;
}



int Configuration::getServerPort() const
{
    return 0;
}



bool Configuration::getIgnoreSSLErrors() const
{
    return false;
}



void Configuration::setAvatar(const QString &data, const QString &mime)
{
    Q_UNUSED(data)
    Q_UNUSED(mime)
}


void Configuration::setDisplayName(const QString &nDisplayName)
{
    Q_UNUSED(nDisplayName)
}



void Configuration::setImproperlyConfiguredCron(bool nImproperlyConfiguredCron)
{
    Q_UNUSED(nImproperlyConfiguredCron)
}
