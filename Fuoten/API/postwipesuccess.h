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

#ifndef FUOTENPOSTWIPESUCCESS_H
#define FUOTENPOSTWIPESUCCESS_H

#include <QObject>
#include "component.h"
#include "../fuoten_global.h"

namespace Fuoten {

class PostWipeSuccessPrivate;

class FUOTENSHARED_EXPORT PostWipeSuccess : public Component
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs a new %PostWipeSuccess object with the given \a parent.
     */
    explicit PostWipeSuccess(QObject *parent = nullptr);

    /*!
     * \brief Destroys the %PostWipeSuccess object.
     */
    ~PostWipeSuccess() override;

    Q_INVOKABLE void execute() override;

protected:
    PostWipeSuccess(PostWipeSuccessPrivate &dd, QObject *parent = nullptr);

    bool checkInput() override;

    void successCallback() override;

private:
    Q_DECLARE_PRIVATE(PostWipeSuccess)
    Q_DISABLE_COPY(PostWipeSuccess)
    Q_DISABLE_MOVE(PostWipeSuccess)
};

}

#endif // FUOTENPOSTWIPESUCCESS_H
