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

#ifndef FUOTENERROR_P_H
#define FUOTENERROR_P_H

#include "error.h"
#include <QMetaEnum>
#include <QMetaObject>

namespace Fuoten {

class ErrorPrivate
{
public:
    QString text;
    Error::Type type = Error::NoError;
    QString data;
    Error::Severity severity = Error::Nothing;

    void printOut()
    {
        switch(severity) {
        case Error::Warning:
            qWarning("%s: %s", Error::staticMetaObject.enumerator(Error::staticMetaObject.indexOfEnumerator("Type")).valueToKey(type), qUtf8Printable(text));
            break;
        case Error::Critical:
            qCritical("%s: %s", Error::staticMetaObject.enumerator(Error::staticMetaObject.indexOfEnumerator("Type")).valueToKey(type), qUtf8Printable(text));
            break;
        case Error::Fatal:
            qFatal("%s: %s", Error::staticMetaObject.enumerator(Error::staticMetaObject.indexOfEnumerator("Type")).valueToKey(type), qUtf8Printable(text));
            break;
        case Error::Nothing:
        default:
            break;
        }

#ifndef QT_NO_DEBUG_OUTPUT
        if (!data.isEmpty()) {
            qDebug("Error data: %s", qUtf8Printable(data));
        }
#endif
    }

};

}

#endif // FUOTENERROR_P_H
