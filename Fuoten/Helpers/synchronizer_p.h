/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016-2017 Matthias Fehring
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

#ifndef FUOTENSYNCHRONIZER_P_H
#define FUOTENSYNCHRONIZER_P_H

#include "synchronizer.h"
#include "abstractconfiguration.h"
#include "abstractnotificator.h"
#include "../Storage/abstractstorage.h"
#include "../API/getfolders.h"
#include "../API/getfeeds.h"
#include "../API/getitems.h"
#include "../API/getupdateditems.h"
#include "../API/markmultipleitems.h"
#include "../API/starmultipleitems.h"
#include "../error.h"
#include <QTimer>
#include <QDateTime>

namespace Fuoten {

class SynchronizerPrivate
{
    Q_DECLARE_PUBLIC(Synchronizer)
public:
    explicit SynchronizerPrivate(Synchronizer *parent) :
        q_ptr(parent)
    {}

    ~SynchronizerPrivate() {}


    void cleanup()
    {
        if (getFolders) {
            getFolders->deleteLater();
            getFolders = nullptr;
        }
        if (getFeeds) {
            getFeeds->deleteLater();
            getFeeds = nullptr;
        }
        if (getStarred) {
            getStarred->deleteLater();
            getStarred = nullptr;
        }
        if (getUnread) {
            getUnread->deleteLater();
            getUnread = nullptr;
        }
        if (getUpdated) {
            getUpdated->deleteLater();
            getUpdated = nullptr;
        }
        if (starMultipleItems) {
            starMultipleItems->deleteLater();
            starMultipleItems = nullptr;
        }
        if (unstarMultipleItems) {
            unstarMultipleItems->deleteLater();
            unstarMultipleItems = nullptr;
        }
        if (readMultipleItems) {
            readMultipleItems->deleteLater();
            readMultipleItems = nullptr;
        }
        if (unreadMultipleItems) {
            unreadMultipleItems->deleteLater();
            unreadMultipleItems = nullptr;
        }
        if (storage) {
            QObject::disconnect(storage, 0, q_ptr, 0);
        }
        queuedUnreadArticles.clear();
        queuedReadArticles.clear();
        queuedStarredArticles.clear();
        queuedUnstarredArticles.clear();
        inOperation = false;
        progress = 0.0;
        totalActions = 0.0;
        performedActions = 0.0;
        currentAction.clear();
        Q_Q(Synchronizer);
        Q_EMIT q->progressChanged(0.0);
        Q_EMIT q->currentActionChanged(QString());
    }

    void setInOperation(bool nInOperation)
    {
        if (inOperation != nInOperation) {
            Q_Q(Synchronizer);
            inOperation = nInOperation;
            Q_EMIT q->inOperationChanged(inOperation);
        }
    }


    QList<QPair<qint64, QString> > queuedStarredArticles;
    QList<QPair<qint64, QString> > queuedUnstarredArticles;
    IdList queuedUnreadArticles;
    IdList queuedReadArticles;
    Synchronizer * const q_ptr;
    Error *error = nullptr;
    AbstractConfiguration *configuration = nullptr;
    AbstractStorage *storage = nullptr;
    GetFolders *getFolders = nullptr;
    GetFeeds *getFeeds = nullptr;
    GetItems *getUnread = nullptr;
    GetItems *getStarred = nullptr;
    GetUpdatedItems *getUpdated = nullptr;
    StarMultipleItems *starMultipleItems = nullptr;
    StarMultipleItems *unstarMultipleItems = nullptr;
    MarkMultipleItems *readMultipleItems = nullptr;
    MarkMultipleItems *unreadMultipleItems = nullptr;
    QTimer *deferTimer = nullptr;
    AbstractNotificator *notificator = nullptr;
    QString currentAction;
    QDateTime startTime;
    qreal progress = 0.0;
    qreal totalActions = 0.0;
    qreal performedActions = 0.0;
    bool inOperation = false;
};

}

#endif // FUOTENSYNCHRONIZER_P_H
