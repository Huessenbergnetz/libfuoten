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
#include "abstractconfiguration.h"
#include "../Storage/abstractstorage.h"
#include "../API/getfolders.h"
#include "../API/getfeeds.h"
#include "../API/getitems.h"
#include "../API/getupdateditems.h"
#include "../error.h"

namespace Fuoten {

class SynchronizerPrivate
{
    Q_DECLARE_PUBLIC(Synchronizer)
public:
    explicit SynchronizerPrivate(Synchronizer *parent) :
        q_ptr(parent),
        error(nullptr),
        configuration(nullptr),
        storage(nullptr),
        inOperation(false),
        getFolders(nullptr),
        getFeeds(nullptr),
        getUnread(nullptr),
        getStarred(nullptr),
        getUpdated(nullptr)
    {}

    ~SynchronizerPrivate() {}

    void setError(Error *nError);
    void start();
    void requestFeeds();
    void requestUnread();
    void requestStarred();
    void requestUpdated();
    void finished();


    Synchronizer * const q_ptr;
    Error *error;
    AbstractConfiguration *configuration;
    AbstractStorage *storage;
    bool inOperation;

    GetFolders *getFolders;
    GetFeeds *getFeeds;
    GetItems *getUnread;
    GetItems *getStarred;
    GetUpdatedItems *getUpdated;
};

}

#endif // FUOTENSYNCHRONIZER_P_H
