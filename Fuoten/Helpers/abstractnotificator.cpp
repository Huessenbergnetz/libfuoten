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

#include "abstractnotificator_p.h"

using namespace Fuoten;

AbstractNotificator::AbstractNotificator(QObject *parent) : QObject(parent), d_ptr(new AbstractNotificatorPrivate)
{

}


AbstractNotificator::~AbstractNotificator()
{

}


QString AbstractNotificator::appIcon() const { Q_D(const AbstractNotificator); return d->appIcon; }

void AbstractNotificator::setAppIcon(const QString &appIcon)
{
    Q_D(AbstractNotificator);
    if (d->appIcon != appIcon) {
        d->appIcon = appIcon;
        qDebug("Changed appIcon to \"%s\"", qUtf8Printable(d->appIcon));
        Q_EMIT appIconChanged(d->appIcon);
    }
}


QString AbstractNotificator::appName() const { Q_D(const AbstractNotificator); return d->appName; }

void AbstractNotificator::setAppName(const QString &appName)
{
    Q_D(AbstractNotificator);
    if (d->appName != appName) {
        d->appName = appName;
        qDebug("Changed appName to \"%s\".", qUtf8Printable(d->appName));
        Q_EMIT appNameChanged(d->appName);
    }
}


bool AbstractNotificator::isEnabled() const { Q_D(const AbstractNotificator); return d->enabled; }

void AbstractNotificator::setEnabled(bool enabled)
{
    Q_D(AbstractNotificator);
    if (d->enabled != enabled) {
        d->enabled = enabled;
        qDebug("Changed enabled to %s.", d->enabled ? "true" : "false");
        Q_EMIT enabledChanged(d->enabled);
    }
}
