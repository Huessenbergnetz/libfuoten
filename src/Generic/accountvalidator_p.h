/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016 Buschtrommel / Matthias Fehring
 * https://www.buschmann23.de/entwicklung/bibliotheken/libfuoten/
 * https://github.com/Buschtrommel/libfuoten
 *
 * Generic/accountvalidator_p.h
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

#ifndef ACCOUNTVALIDATOR_P_H
#define ACCOUNTVALIDATOR_P_H

#include "accountvalidator.h"
#include "version.h"
#include "status.h"
#include "user.h"

namespace Fuoten {
namespace Generic {


class AccountValidatorPrivate
{
public:
    AccountValidatorPrivate(AccountValidator *parent) :
        q_ptr(parent),
        configuration(nullptr),
        inOperation(false),
        error(nullptr),
        version(nullptr),
        status(nullptr),
        user(nullptr)
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

    Configuration *configuration;
    bool inOperation;
    Error *error;

    Version *version;
    Status *status;
    User *user;
};

}
}

#endif // ACCOUNTVALIDATOR_P_H
