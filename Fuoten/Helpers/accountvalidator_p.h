/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016-2019 Hüssenbergnetz/Matthias Fehring
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

#ifndef FUOTENACCOUNTVALIDATOR_P_H
#define FUOTENACCOUNTVALIDATOR_P_H

#include "accountvalidator.h"
#include "../API/getversion.h"
#include "../API/getstatus.h"
#include "../API/getuser.h"

namespace Fuoten {

class AccountValidatorPrivate
{
public:
    explicit AccountValidatorPrivate(AccountValidator *parent) :
        q_ptr(parent)
    {}

    ~AccountValidatorPrivate() {}

    void setInOperatin(bool inop)
    {
        if (inOperation != inop) {
            Q_Q(AccountValidator);
            inOperation = inop;
            Q_EMIT q->inOperationChanged(inOperation);
        }
    }

    AccountValidator * const q_ptr;
    Q_DECLARE_PUBLIC(AccountValidator)

    AbstractConfiguration *configuration = nullptr;
    Error *error = nullptr;
    GetVersion *version = nullptr;
    GetStatus *status = nullptr;
    GetUser *user = nullptr;

    bool inOperation = false;
};

}

#endif // FUOTENACCOUNTVALIDATOR_P_H
