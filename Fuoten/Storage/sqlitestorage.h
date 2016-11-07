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
    void itemsMarked(const IdList &idsMarkedRead, const IdList &idsMarkedUnread) override;
    void itemsStarred(const QList<QPair<qint64, QString>> &articlesStarred, const QList<QPair<qint64, QString>> &articlesUnstarred) override;
    void itemMarked(qint64 itemId, bool unread) override;
    void itemStarred(qint64 itemId, const QString &guidHash, bool starred) override;

private:
    Q_DECLARE_PRIVATE(SQLiteStorage)
    Q_DISABLE_COPY(SQLiteStorage)
};

}

#endif // SQLITESTORAGE_H
