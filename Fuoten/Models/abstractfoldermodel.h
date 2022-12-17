/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENABSTRACTFOLDERMODEL_H
#define FUOTENABSTRACTFOLDERMODEL_H

#include <QObject>
#include "basemodel.h"
#include "../fuoten_global.h"
#include "fuoten_export.h"

namespace Fuoten {

class AbstractFolderModelPrivate;
class Folder;

/*!
 * \brief Abstract folder model that provides basic functionalities for folder models.
 *
 * \headerfile "" <Fuoten/Model/AbstractFolderModel>
 */
class FUOTEN_EXPORT AbstractFolderModel : public BaseModel
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs a new empty abstract folder model with the given \a parent.
     */
    explicit AbstractFolderModel(QObject *parent = nullptr);

    /*!
     * \brief Destroys the %AbstractFolderModel object.
     */
    ~AbstractFolderModel() override;

    /*!
     * \brief Returns the model index of the folder identified by \a id.
     */
    QModelIndex findByID(qint64 id) const override;

public Q_SLOTS:
    /*!
     * \brief Populates the mode with data from the local storage.
     *
     * Uses AbstractStorage::getFolders() to get Folder objects from the local storage that will be added to the model.
     */
    void load() override;

protected:
    AbstractFolderModel(AbstractFolderModelPrivate &dd, QObject *parent = nullptr);

    /*!
     * \brief Connetcs the folder related signals of AbstractStorage to the model's slots.
     */
    void handleStorageChanged(AbstractStorage *old) override;

    /*!
     * \brief Returns the list of Folder objects in the model.
     */
    QList<Folder*> folders() const;

    /*!
     * \brief Clears the complete model and will destroy all Folder objects.
     */
    void clear() override;

protected Q_SLOTS:
    /*!
     * \brief Takes and processes data after folders have been requested.
     *
     * handleStorageChanged() will connect AbstractStorage::requestedFolders() signal to this slot.
     */
    void foldersRequested(const QList<QPair<qint64, QString> > &updatedFolders, const QList<QPair<qint64, QString> > &newFolders, const Fuoten::IdList &deletedFolders);

    /*!
     * \brief Takes and processes data after a folder has been renamed.
     *
     * handleStorageChanged() will connect AbstractStorage::renamedFolder() signal to this slot.
     */
    void folderRenamed(qint64 id, const QString &newName);

    /*!
     * \brief Takes and processes data after a folder has been created.
     *
     * Will add the new folder to the model. handleStorageChanged() will connect AbstractStorage::createdFolder() to this slot.
     */
    void folderCreated(qint64 id, const QString &name);

    /*!
     * \brief Takes and processes data after a folder has been delted.
     *
     * Will delete the folder identified by \a id from the model. handleStorageChanged() will connect AbstractStorage::deletedFolder() to this slot.
     */
    void folderDeleted(qint64 id);

    /*!
     * \brief Takes and processes data after a folder has been marked as read.
     *
     * Will set the \link Folder::unreadCount unreadCount \endlink of the folder identified by \a id to \c 0.
     * handleStorageChanged() will connect the AbstractStorage::markedReadFolder() and AbstractStorage::markedReadFolderInQueue() signals to this slot.
     */
    void folderMarkedRead(qint64 id, qint64 newestItem);

    /*!
     * \brief Takes and processes data after the feeds have been requeste from the server.
     *
     * handleStorageChanged() will connect the AbstractStorage::requestedFeeds() signal to this slot.
     */
    void feedsRequested(const Fuoten::IdList &updatedFeeds, const Fuoten::IdList &newFeeds, const Fuoten::IdList &deletedFeeds);

    /*!
     * \brief Takes and processes data after a new feed has been created.
     *
     * handleStorageChanged() will connect the AbstractStorage::createdFeed() signal to this slot.
     */
    void feedCreated(qint64 feedId, qint64 folderId);

    /*!
     * \brief Takes and processes data after a feed has been created.
     *
     * handleStorageChanged() will connect the AbstractStorage::markedReadFeed() and AbstractStorage::markedReadFeedInQueue() signals to this slot.
     */
    void feedMarkedRead(qint64 id);

    /*!
     * \brief Will reload feedCount and unreadCount for every folder.
     *
     * handleStorageChanged() will connect the following signals to this slot:
     * \li AbstractStorage::deletedFeed()
     * \li AbstractStorage::movedFeed()
     * \li AbstractStorage::requestedItems()
     * \li AbstractStorage::updatedItems()
     * \li AbstractStorage::markedItems()
     * \li AbstractStorage::markedAllItemsRead()
     * \li AbstractStorage::markedAllItemsReadInQueue()
     */
    void updateCountValues();

    /*!
     * \brief Takes and processes data after an item/article has been marked as read.
     *
     * handleStorageChanged() will connnect the AbstractStorage::markedItem() signal to this slot.
     *
     * \param itemId    ID of the item/article that has been marked as read or unread
     * \param unread    \c true if the article has been marked as unread, \c false if it has been marked as read
     */
    void itemMarked(qint64 itemId, bool unread);

private:
    Q_DISABLE_COPY(AbstractFolderModel)
    Q_DECLARE_PRIVATE(AbstractFolderModel)
};

}

#endif // FUOTENABSTRACTFOLDERMODEL_H
