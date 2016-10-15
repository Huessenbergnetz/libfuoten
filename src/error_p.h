/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016 Buschtrommel / Matthias Fehring
 * https://www.buschmann23.de/entwicklung/bibliotheken/libfuoten/
 * https://github.com/Buschtrommel/libfuoten
 *
 * error_p.h
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

#ifndef ERROR_P_H
#define ERROR_P_H

#include "error.h"

namespace Fuoten {

class ErrorPrivate
{
public:
    QString text;
    Error::Type type = Error::NoError;
    QString data;
    Error::Severity severity = Error::Nothing;

#ifdef QT_DEBUG
    void printOut()
    {
#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)

        switch(severity) {
        case Error::Warning:
            qWarning("%s", qUtf8Printable(text));
            break;
        case Error::Critical:
            qCritical("%s", qUtf8Printable(text));
            break;
        case Error::Fatal:
            qFatal("%s", qUtf8Printable(text));
            break;
        case Error::Nothing:
        default:
            break;
        }

        if (!data.isEmpty()) {
            qDebug("%s", qUtf8Printable(data));
        }

#else

        switch(severity) {
        case Error::Warning:
            qWarning("%s", text.toUtf8().constData());
            break;
        case Error::Critical:
            qCritical("%s", text.toUtf8().constData());
            break;
        case Error::Fatal:
            qFatal("%s", text.toUtf8().constData());
            break;
        case Error::Nothing:
        default:
            break;
        }

        if (!data.isEmpty()) {
            qDebug("%s", data.toUtf8().constData());
        }

#endif
    }
#endif

};

}

#endif // ERROR_P_H
