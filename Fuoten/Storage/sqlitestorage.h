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

#ifndef SQLITESTORAGE_H
#define SQLITESTORAGE_H

#include <QObject>
#include "abstractstorage.h"
#include "../fuoten.h"
#include "../fuoten_global.h"

namespace Fuoten {

class SQLiteStoragePrivate;
class Folder;
class Feed;
class Article;

/*!
 * \brief Storage using a local SQLite database.
 *
 * To use this storage, simply set the path to the SQLite database file in the constructor and call init().
 * The path to the database file will not be created automatically. It has to be created before calling init().
 *
 * If you want to have a custom storage class, derive from AbstractStorage.
 *
 * \headerfile "" <Fuoten/Storage/SQLiteStorage>
 */
class FUOTENSHARED_EXPORT SQLiteStorage final : public AbstractStorage
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs a new SQLiteStorage object with given \a dbpath and \a parent.
     */
    SQLiteStorage(const QString &dbpath, QObject *parent = nullptr);

    /*!
     * \brief Initializes the SQLite database.
     *
     * This will create/check the table layout.
     */
    void init() override;

    /*!
     * \brief Returns a list of Folder objects from the \a folders table.
     */
    QList<Folder*> getFolders(FuotenEnums::SortingRole sortingRole = FuotenEnums::Name, Qt::SortOrder sortOrder = Qt::AscendingOrder, const IdList &ids = IdList(), FuotenEnums::Type idType = FuotenEnums::Folder, int limit = 0) override;

    /*!
     * \brief Returns a list of Feed objects from the \a feeds table.
     */
    QList<Feed*> getFeeds(const QueryArgs &args) override;

    /*!
     * \brief Returns a list of Article objects from the \a items table.
     */
    QList<Article*> getArticles(const QueryArgs &args) override;

    /*!
     * \brief Invokes an asynchronous query for articles in a different thread.
     *
     * Will emit the AbstractStorage::gotArticlesAsync() signal after the query finished. The signal
     * will contain a list of Article objects. When connecting to this signal, be aware that the objects
     * in the list have been created in a different thread.
     *
     * \param args query arguments
     */
    void getArticlesAsync(const QueryArgs &args) override;

    /*!
     * \brief Returns the Feed identified by \a id.
     *
     * Returns a \c nullptr if the Feed can not be found.
     */
    Feed *getFeed(qint64 id) override;

    /*!
     * \brief Returns the Article identified by \a id.
     *
     * Returns a \c nullptr if the Article can not be found.
     */
    Article *getArticle(qint64 id, int bodyLimit = 0) override;

    /*!
     * \brief Returns the newest/highest item/article ID fo the given \a type.
     *
     * Supported Types: FuotenEnums::Feed, FuotenEnums::Folder, FuotenEnums::All. For folder and feed type
     * a valid \a id has be provided that identifieds the folder or feed.
     *
     * If the type does not match one of the supported or if there are not items, \c -1 is returned.
     */
    qint64 getNewestItemId(FuotenEnums::Type type = FuotenEnums::All, qint64 id = -1) override;

    /*!
     * \brief Returns the full body of an Article identified by \a id.
     */
    Q_INVOKABLE QString getArticleBody(qint64 id) override;

    /*!
     * \brief Enqueues the \a action for the given \a article in the local SQLite database.
     *
     * Will update the queue column in the items table and also will perform the action locally.
     *
     * \param action    the action to be performed on the Article object
     * \param article   the Article object the action should be performed on
     * \return          \c true if the enqueue was successful, otherwise \c false
     */
    bool enqueueItem(FuotenEnums::QueueAction action, Article *article) override;

    /*!
     * \brief Adds all articles older than \a newestItemId in the feed identified by \a feedId as read to the local queue.
     *
     * Will update the queue column for every item in the feed and will also perform the action locally. Will
     * emit the AbstractStorage::markedReadFeedInQueue() signal on success. The action will be performed in a separate
     * thread, so the return value only indicates if the threaded action has been started successfully.
     *
     * \param feedId        ID of the feed to be marked as read
     * \param newestItemId  ID of the newest item in the feed
     * \return \c true on success, otherwise \c false
     */
    bool enqueueMarkFeedRead(qint64 feedId, qint64 newestItemId) override;

    /*!
     * \brief Adds all articles older than \a newestItemId in the folder identified by \a folderId as read to the local queue.
     *
     * Will update the queue column for every item in the folder and will also perform the action locally. Will
     * emit the AbstractStorage::markedReadFolderInQueue() signal on success. The action will be performed in a separate
     * thread, so the return value only indicates if the threaded action has been started successfully.
     *
     * \param folderId      ID of the folder to be marked as read
     * \param newestItemId  ID of the newest item in the folder
     * \return \c true on success, otherwise \c false
     */
    bool enqueueMarkFolderRead(qint64 folderId, qint64 newestItemId) override;

    /*!
     * \brief Adds all local articles that are unread to the queue and marks them as read.
     *
     * Will update the queue column for every item in the database and will also perform the cation locally. Will
     * emit the AbstractStorage::markedAllItemsReadInQueue() signal on success The action will be performed in a separate
     * thread, so the return value only indicates if the threaded action has been started successfully.
     *
     * \return \c true on success, otherwise \c false
     */
    bool enqueueMarkAllItemsRead() override;

    /*!
     * \brief Resets the queue column after the queue has been worked.
     */
    void clearQueue() override;

public Q_SLOTS:
    void foldersRequested(const QJsonDocument &json) override;
    void folderCreated(const QJsonDocument &json) override;
    void folderRenamed(qint64 id, const QString &newName) override;
    void folderDeleted(qint64 id) override;
    void folderMarkedRead(qint64 id, qint64 newestItem) override;

    void feedsRequested(const QJsonDocument &json) override;
    void feedCreated(const QJsonDocument &json) override;
    void feedDeleted(qint64 id) override;
    void feedMoved(qint64 id, qint64 targetFolder) override;
    void feedRenamed(qint64 id, const QString &newTitle) override;
    void feedMarkedRead(qint64 id, qint64 newestItem) override;

    void itemsRequested(const QJsonDocument &json) override;
    void itemsMarked(const Fuoten::IdList &itemIds, bool unread) override;
    void itemsStarred(const QList<QPair<qint64, QString>> &articles, bool star) override;
    void itemMarked(qint64 itemId, bool unread) override;
    void itemStarred(qint64 feedId, const QString &guidHash, bool star) override;
    void allItemsMarkedRead(qint64 newestItemId) override;

private:
    Q_DECLARE_PRIVATE(SQLiteStorage)
    Q_DISABLE_COPY(SQLiteStorage)
};

}

#endif // SQLITESTORAGE_H
