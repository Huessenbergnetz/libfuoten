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

#ifndef FUOTENABSTRACTFEEDMODEL_H
#define FUOTENABSTRACTFEEDMODEL_H

#include <QObject>
#include "basemodel.h"
#include "../fuoten_global.h"

namespace Fuoten {

class AbstractFeedModelPrivate;
class Feed;

/*!
 * \brief Abstract feed model that provides basic functionalities for Feed models.
 *
 * \headerfile "" <Fuoten/Models/AbstractFeedModel>
 */
class FUOTENSHARED_EXPORT AbstractFeedModel : public BaseModel
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs a new empty abstract Feed model with the given \a parent.
     */
    explicit AbstractFeedModel(QObject *parent = nullptr);

    QModelIndex findByID(qint64 id) const override;
    QHash<qint64, QModelIndex> findByIDs(const QList<qint64> ids) const override;

public Q_SLOTS:
    void load() override;

protected Q_SLOTS:
    void feedsRequested(const QList<qint64> &updatedFeeds, const QList<qint64> &newFeeds, const QList<qint64> &deletedFeeds);
    void feedCreated(qint64 id, qint64 folderId);
    void feedDeleted(qint64 id);
    void feedMoved(qint64 id, qint64 targetFolderId);
    void feedRenamed(qint64 id, const QString &newName);
    void feedMarkedRead(qint64 id, qint64 newestItemId);

protected:
    AbstractFeedModel(AbstractFeedModelPrivate &dd, QObject *parent = nullptr);

    void handleStorageChanged() override;

    QList<Feed*> feeds() const;

private:
    Q_DISABLE_COPY(AbstractFeedModel)
    Q_DECLARE_PRIVATE(AbstractFeedModel)
};

}

#endif // ABSTRACTFEEDMODEL_H
