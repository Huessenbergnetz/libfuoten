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

#ifndef FUOTENGETWIPESTATUS_H
#define FUOTENGETWIPESTATUS_H

#include <QObject>
#include "component.h"
#include "../fuoten_global.h"

namespace Fuoten {

class GetWipeStatusPrivate;

class FUOTENSHARED_EXPORT GetWipeStatus : public Component
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs a new %GetWipeStatus object with the given \a parent.
     */
    explicit GetWipeStatus(QObject *parent = nullptr);

    /*!
     * \brief Destroys the %GetWipeStatus object.
     */
    ~GetWipeStatus() override;

    Q_INVOKABLE void execute() override;

Q_SIGNALS:
    void wipeRequested();

protected:
    GetWipeStatus(GetWipeStatusPrivate &dd, QObject *parent = nullptr);

    bool checkInput() override;

    void successCallback() override;

private:
    Q_DECLARE_PRIVATE(GetWipeStatus)
    Q_DISABLE_COPY(GetWipeStatus)
    Q_DISABLE_MOVE(GetWipeStatus)
};

}

#endif // FUOTENGETWIPESTATUS_H
