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

#ifndef FUOTENABSTRACTFOLDERMODEL_H
#define FUOTENABSTRACTFOLDERMODEL_H

#include <QObject>
#include "basemodel.h"
#include "../fuoten_global.h"

namespace Fuoten {

class AbstractFolderModelPrivate;
class Folder;

/*!
 * \brief Abstract folder model that provides basic functionalities for folder models.
 *
 * \headerfile "" <Fuoten/Model/AbstractFolderModel>
 */
class FUOTENSHARED_EXPORT AbstractFolderModel : public BaseModel
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs a new empty abstract folder model with the given \a parent.
     */
    explicit AbstractFolderModel(QObject *parent = nullptr);

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
    void handleStorageChanged() override;

    /*!
     * \brief Returns the list of Folder objects in the model.
     */
    QList<Folder*> folders() const;

protected Q_SLOTS:
    /*!
     * \brief Takes and processes data after folders have been requested.
     *
     * handleStorageChanged() will connect AbstractStorage::requestedFolders() signal to this slot.
     */
    void foldersRequested(const QList<QPair<qint64, QString> > &updatedFolders, const QList<QPair<qint64, QString> > &newFolders, const QList<qint64> &deletedFolders);

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
     * Will set the \link Folder::unreadCount unreadCount \endlink of the folder identified by \id to \c 0. handleStorageChanged() will connect AbstractStorage::markedReadFolder() to this slot.
     */
    void folderMarkedRead(qint64 id, qint64 newestItem);

    /*!
     * \brief Takes and processes data after the feeds have been requeste from the server.
     *
     * handleStorageChanged() will connect the AbstractStorage::requestedFeeds() signal to this slot.
     */
    void feedsRequested(QList<qint64> &updatedFeeds, QList<qint64> &newFeeds, QList<qint64> &deletedFeeds);

    /*!
     * \brief Takes and processes data after a feed has been created.
     *
     * handleStorageChanged() will connectthe AbstractStorage::createdFeed() and the AbstractStorage::markedReadFeed() signal to this slot.
     */
    void feedCreatedOrMarkedRead(qint64 id);

    /*!
     * \brief Will reload feedCount and unreadCount for every folder.
     *
     * handleStorageChanged() will connect the following signals to this slot:
     * \li AbstractStorage::deletedFeed()
     * \li AbstractStorage::movedFeed()
     * \li AbstractStorage::requestedItems()
     * \li AbstractStorage::updatedItems()
     * \li AbstractStorage::markedItems()
     * \li AbstractStorage::starredItems()
     */
    void updateCountValues();

private:
    Q_DISABLE_COPY(AbstractFolderModel)
    Q_DECLARE_PRIVATE(AbstractFolderModel)
};

}

#endif // FUOTENABSTRACTFOLDERMODEL_H
