/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
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

    /*!
     * \brief Destroys the %AbstractFeedModel object.
     */
    ~AbstractFeedModel() override;

    /*!
     * \brief Returns the model index of the Feed identified by \a id.
     * \param id ID of the feed to find in the model
     */
    QModelIndex findByID(qint64 id) const override;

    /*!
     * \brief Returns IDs and model indices of the Feed objects in the model defined by their ID in \a ids.
     *
     * The \a key of the hash table contains the ID of the Feed, the \a value will contain the feed's model index.
     * Only feeds will be returned of that the ID is part of the model.
     *
     * \param ids list of feed IDs to find in the model
     */
    QHash<qint64, QModelIndex> findByIDs(const IdList &ids) const override;

public Q_SLOTS:
    /*!
     * \brief Populates the model with data from the local storage.
     *
     * Uses AbstractStorage::getFeeds() to get Feed objects from the local storage that will be added to the model.
     * If the BaseModel::parentId property is set to a value greater than \c -1, only feeds will be loaded that are
     * part of the folder indentified by the \a parentId property.
     */
    void load() override;

protected Q_SLOTS:
    /*!
     * \brief Takes and processes data after feeds have been requested.
     *
     * handleStorageChanged() will connect the AbstractStorage::requestedFeeds() signal to this slot.
     *
     * \param updatedFeeds  list of IDs of feeds that have been updated
     * \param newFeeds      list of IDs of feeds that are new to the local storage
     * \param deletedFeeds  list of IDs of feeds that have been deleted in the local storage (and remotely before)
     */
    void feedsRequested(const Fuoten::IdList &updatedFeeds, const Fuoten::IdList &newFeeds, const Fuoten::IdList &deletedFeeds);

    /*!
     * \brief Takes and processes data after a feed has been created.
     *
     * handleStorageChanged() will connect the AbstractStorage::createdFeed() signal to this slot.
     *
     * \param id        ID of the newly created feed
     * \param folderId  ID of the folder the newly created feed is part of
     */
    void feedCreated(qint64 id, qint64 folderId);

    /*!
     * \brief Takes and processes data after a feed has been deleted.
     *
     * handleStorageChanged() will connect the AbstractStorage::deletedFeed() signal to this slot.
     *
     * \param id ID of the deleted feed
     */
    void feedDeleted(qint64 id);

    /*!
     * \brief Takes and processes data after a feed has been moved to a different folder.
     *
     * handleStorageChanged() will connect the AbstractStorage::movedFeed() signal to this slot.
     *
     * \param id                ID of the moved feed
     * \param targetFolderId    ID of the folder the feed has been moved to
     */
    void feedMoved(qint64 id, qint64 targetFolderId);

    /*!
     * \brief Takes and processes data after a feed has been renamed.
     *
     * handleStorageChanged() will connect the AbstractStorage::renamedFeed() signal to this slot.
     *
     * \param id        ID of the renamed feed
     * \param newName   new name for the feed
     */
    void feedRenamed(qint64 id, const QString &newName);

    /*!
     * \brief Takes and processes data after a complete feed has been marked as read.
     *
     * handleStorageChanged() will connect the AbstractStorage::markedReadFeed() and AbstractStorage::markedReadFeedInQueue() signals to this slot.
     *
     * \param id            ID of the feed that has been marked as read
     * \param newestItemId  unused
     */
    void feedMarkedRead(qint64 id, qint64 newestItemId);

    /*!
     * \brief Takes and processes data after a complete folder has been marked as read.
     *
     * handleStorageChanged() will connect the AbstractStorage::markedReadFolder() and AbstractStorage::markedReadFolderInQueue() signals to this slot.
     *
     * \param folderId      ID of the folder that has been marked as read
     * \param newestItemId  unused
     */
    void folderMarkedRead(qint64 folderId, qint64 newestItemId);

    /*!
     * \brief Takes and processes data after a folder has been deleted.
     *
     * handleStorageChanged() will connect the AbstractStorage::deletedFolder() signal to this slot.
     *
     * \param folderId ID of the folder that has been deleted
     */
    void folderDeleted(qint64 folderId);

    /*!
     * \brief Takes and processes data after items/articles have been requested/updated.
     *
     * handleStorageChanged() will connect the AbstractStorage::requestedItems() signal to this slot.
     * The model will reload all feeds in it and will update the unreadCount properties, it will not
     * use the arguments of the slot
     */
    void itemsRquested(const Fuoten::IdList &updatedItems, const Fuoten::IdList &newItems, const Fuoten::IdList &deletedItems);

    /*!
     * \brief Takes and processes data after an item/article has been marked as read/unread.
     *
     * handleStorageChanged() will connect the AbstractStorage::markedItem() signal to this slot.
     *
     * \param itemId    ID of the item that has been marked as read or unread
     * \param unread    \c true if the item has been marked as unread, \c false if it has been marked as read
     */
    void itemMarked(qint64 itemId, bool unread);

    /*!
     * \brief Should be called after a set of items has been marked as read or unread.
     *
     * Will update the unreadCount property of all Feed objects in the model.
     *
     * handleStorageChanged() will connect the following signals to this slot:
     * \li AbstractStorage::markedItems()
     * \li AbstractStorage::markedAllItemsRead()
     * \li AbstractStorage::markedAllItemsReadInQueue()
     */
    void itemsMarked();

protected:
    AbstractFeedModel(AbstractFeedModelPrivate &dd, QObject *parent = nullptr);

    /*!
     * \brief Connects the feed realted signals of AbstractStorage to the model's slots.
     */
    void handleStorageChanged(AbstractStorage *old) override;

    /*!
     * \brief Returns the list of Feed objects in the model.
     */
    QList<Feed*> feeds() const;

    /*!
     * \brief Clears the complete model and will destroy all Feed object.
     */
    void clear() override;

private:
    Q_DISABLE_COPY(AbstractFeedModel)
    Q_DISABLE_MOVE(AbstractFeedModel)
    Q_DECLARE_PRIVATE(AbstractFeedModel)
};

}

#endif // ABSTRACTFEEDMODEL_H
