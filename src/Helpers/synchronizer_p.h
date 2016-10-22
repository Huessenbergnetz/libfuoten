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

#ifndef FUOTENSYNCHRONIZER_P_H
#define FUOTENSYNCHRONIZER_P_H

#include "synchronizer.h"
#include "configuration.h"
#include "../Storage/abstractstorage.h"
#include "../API/getfolders.h"
#include "../error.h"

namespace Fuoten {

class SynchronizerPrivate
{
    Q_DECLARE_PUBLIC(Synchronizer)
public:
    SynchronizerPrivate(Synchronizer *parent) :
        q_ptr(parent),
        error(nullptr),
        configuration(nullptr),
        storage(nullptr),
        inOperation(false),
        getFolders(nullptr)
    {}

    ~SynchronizerPrivate() {}

    void setError(Error *nError);
    void start();
    void finished();


    Synchronizer * const q_ptr;
    Error *error;
    Configuration *configuration;
    AbstractStorage *storage;
    bool inOperation;

    GetFolders *getFolders;

};

}

#endif // FUOTENSYNCHRONIZER_P_H
