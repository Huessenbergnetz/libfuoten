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

#ifndef FUOTENABSTRACTSTORAGE_H
#define FUOTENABSTRACTSTORAGE_H

#include <QObject>
#include "../fuoten.h"
#include "../fuoten_global.h"

namespace Fuoten {

/*!
 * \brief Helper struct containing query arguments.
 *
 * Used by AbstractStrorage::getFolders(), AbstractStroage::getFeeds() and AbstractStorage::getArticles() to set the arguments used to query a list
 * of objects. Not all members of this struct are used by every query.
 */
struct FUOTENSHARED_EXPORT QueryArgs {
    FuotenEnums::SortingRole sortingRole = FuotenEnums::ID; /**< The role/value used to sort the result by. */
    Qt::SortOrder sortOrder = Qt::AscendingOrder;           /**< The sorting order. */
    qint64 parentId = -1;                                   /**< ID of a parent object (Feed or Folder). Used by feed and article queries, defaults to -1. */
    FuotenEnums::Type parentIdType = FuotenEnums::All;      /**< Type of the parent ID. On feeds can only be FuotenEnums::Folder, on articles can be FuotenEnums::Folder and FuotenEnums::Feed. Defaults to FuotenEnums::All */
    IdList inIds;                                           /**< A list of IDs the records are compared with. Defaults to an empty list. */
    FuotenEnums::Type inIdsType = FuotenEnums::All;         /**< Type of of the inIds list to compare. Related to the object to query, it can be FuotenEnums::Folder, FuotenEnums::Feed or FuotenEnums::Item. Defaults to FuotenEnums::All. */
    bool unreadOnly = false;                                /**< If true, only unread articles or folders and feeds without unread content are returned. */
    bool starredOnly = false;                               /**< Only valid for article queries. If true, only starrred articles are returned. */
    int limit = 0;                                          /**< Limits the result to the specified number of objects. Defaults to \c 0 to return all objects. */
    int bodyLimit = -1;                                     /**< Only valid for article queries. Limits the size of the body text in number of characters. Values lower than \c 0 will return no body text, \c 0 will return the full body text, any other positive value will return a body stripped from HTML tags and limited to the amount of characters. */
};

class Folder;
class Feed;
class Error;
class Article;
class AbstractStoragePrivate;

/*!
 * \brief Abstract class to handle the storage of requested News App data.
 *
 * Reimplement this class to store the data requested from the News App API. You can set your
 * derived class to the Component::storage property, that all classes use that derive
 * from Component. Also BaseItem hast the BaseItem::storage property for derived classes,
 * that is used to set the storage handler to API classes that perfrom API actions.
 *
 * If you derive from AbstractStorage, initialize the storage in init() and set setReady() to true
 * when your storage is ready to handle data.
 *
 * \headerfile "" <Fuoten/Storage/AbstractStorage>
 */
class FUOTENSHARED_EXPORT AbstractStorage : public QObject
{
    Q_OBJECT
    /*!
     * \brief Returns true when the storage handler has finished its initialization.
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>ready() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>readyChanged(bool ready)</TD></TR></TABLE>
     *
     * \sa setReady()
     */
    Q_PROPERTY(bool ready READ ready NOTIFY readyChanged)
    /*!
     * \brief Returns a pointer to an Error object, if any error occures, otherwise a nullptr.
     *
     * \par Access functions:
     * <TABLE><TR><TD>Error*</TD><TD>error() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>errorChanged(Error *error)</TD></TR></TABLE>
     *
     * \sa setError()
     */
    Q_PROPERTY(Fuoten::Error *error READ error NOTIFY errorChanged)
    /*!
     * \brief Total amount of unread items in the storage.
     *
     * \par Access functions:
     * <TABLE><TR><TD>quint16</TD><TD>totalUnread() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>totalUnreadChanged(quint16 totalUnread)</TD></TR></TABLE>
     *
     * \sa setTotalUnread()
     */
    Q_PROPERTY(quint16 totalUnread READ totalUnread NOTIFY totalUnreadChanged)
    /*!
     * \brief Amount of starred items.
     *
     * \par Access functions:
     * <TABLE><TR><TD>quint16</TD><TD>starred() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>starredChanged(quint16 starred)</TD></TR></TABLE>
     *
     * \sa setStarred()
     */
    Q_PROPERTY(quint16 starred READ starred NOTIFY starredChanged)
public:
    /*!
     * \brief Constructs a new abstract local storage with the given \a parent.
     */
    explicit AbstractStorage(QObject *parent = nullptr);

    /*!
     * \brief Deconstructs the AbstractStorage object.
     */
    ~AbstractStorage();

    /*!
     * \brief Returns true when the storage is ready to handle data, otherwise false.
     */
    bool ready() const;

    /*!
     * \brief Retruns a pointer to an Error object, if any error occures, otherwise returns a nullptr.
     */
    Error *error() const;

    /*!
     * \brief Initializes the storage handler.
     *
     * For example, create your database layout. When finished set setReady() to \a true.
     */
    virtual void init() = 0;

    /*!
     * \brief Returns the total number of unread articles.
     * \sa totalUnread
     */
    virtual quint16 totalUnread() const;

    /*!
     * \brief Returns the total number of starred articles.
     * \sa starred
     */
    virtual quint16 starred() const;

    /*!
     * \brief Returns the newest item/article ID for the given type.
     *
     * Supported Types: FuotenEnums::Feed, FuotenEnums::Folder, FuotenEnums::All. For folder and feed type
     * a valid \a id has to be provided that identifieds the folder or feed.
     *
     * If the type does not match one of the supported or if there are not items, \c -1 is returned.
     */
    virtual qint64 getNewestItemId(FuotenEnums::Type type = FuotenEnums::All, qint64 id = -1) = 0;
    
    /*!
     * \brief Returns a list of Folder objects from the local storage.
     *
     * The returned list will be sorted by \a sortingRole and \a sortOrder. If \a ids is not empty,
     * only folders with IDs of \a idType from the list will be returned. The \a idType specifies the
     * id the folder is compared with. If the \a idType is not one of out of the table below, it will
     * be treated as FuotenEnums::Folder.
     *
     * <TABLE>
     * <TR><TD>FuotenEnums::Folder</TD><TD>Only folders with an ID in \a ids will be returned</TD></TR>
     * <TR><TD>FuotenEnums::Feed</TD><TD>Only folders will be returned that contain feeds with an ID in \a ids</TD></TR>
     * <TR><TD>FuotenEnums::Item</TD><TD>Only folders will be returned that contain items with an ID in \a ids</TD></TR>
     * </TABLE>
     *
     * Setting \a limit > \c 0 returns only the amount of items up to that limit.
     *
     * The Folder objects in the returned list will have their parent set to \c nullptr.
     *
     * \par Examples
     *
     * \code{.cpp}
     *
     * \endcode
     */
    virtual QList<Folder*> getFolders(FuotenEnums::SortingRole sortingRole = FuotenEnums::Name, Qt::SortOrder sortOrder = Qt::AscendingOrder, const IdList &ids = IdList(), FuotenEnums::Type idType = FuotenEnums::Folder, int limit = 0) = 0;
    

    /*!
     * \brief Returns a list of Feed objects from the local storage.
     *
     * See QueryArgs for a list of possible query arguments.
     */
    virtual QList<Feed*> getFeeds(const QueryArgs &args) = 0;


    /*!
     * \brief Returns a list of Article objects from the local storage.
     *
     * See QueryArgs for a list of possible query arguments.
     */
    virtual QList<Article*> getArticles(const QueryArgs &args) = 0;

    /*!
     * \brief Invokes a query for Article objects from the local storage, limited by \a args.
     *
     * This should emit the gotArticlesAsync() signal containing a list of Article objects. The
     * default implementation is not really asynchronous, it simply calls getArticles() and emits
     * gotArticlesSync() with the return value of that function.
     *
     * When reimplementing this and connecting to the gotArticlesSync() signal, be aware that the Article
     * objects in the list might have been created in a different thread.
     */
    virtual void getArticlesAsync(const QueryArgs &args);



    /*!
     * \brief Returns the Feed identified by \a id.
     *
     * Returns a \c nullptr if the Feed can not be found.
     */
    virtual Feed *getFeed(qint64 id) = 0;

    /*!
     * \brief Returns the Article identified by \a id.
     *
     * \a bodyLimit limits the size of the body text in number of characters. Values lower than \c 0 will return no body text,
     * \c 0 will return the full body text, any other positive value will return a body stripped from HTML tags and limited to the amount of
     * characters.
     *
     * Returns a \c nullptr if the Article can not be found.
     */
    virtual Article *getArticle(qint64 id, int bodyLimit = 0) = 0;

public Q_SLOTS:
    /*!
     * \brief Receives the reply data of the GetFolders request.
     *
     * Implement this in a derived class to store folder data, for example in a local SQL database.
     * You may want to emit requestedFolders() in your implementation after you processed the data.
     *
     * \par Example JSON response data
     *
     * \code{.json}
     * {
     *     "folders": [
     *         {
     *             "id": 1,
     *             "name": "Interersting Folder"
     *         },
     *         {
     *             "id": 2,
     *             "name": "Other Folder"
     *         }, // etc
     *     ]
     * }
     * \endcode
     */
    virtual void foldersRequested(const QJsonDocument &json) = 0;

    /*!
     * \brief Receives the reply data of the CreateFolder request.
     *
     * Implement this in a derived class to store new folder data, for example in a local SQL databse.
     * You my want to emit createdFolder() in your implementation after you processed the data.
     *
     * \par Example JSON response data
     *
     * \code{.json}
     * {
     *     "folders": [
     *         {
     *             "id": 3,
     *             "name": "New Folder"
     *         }
     *     ]
     * }
     * \endcode
     */
    virtual void folderCreated(const QJsonDocument &json) = 0;

    /*!
     * \brief Receives the reply data of the RenameFolder request.
     *
     * Will rename the folder identified by \a id in the local storage to \a newName and emits the renamedFolder() signal.
     */
    virtual void folderRenamed(qint64 id, const QString &newName) = 0;

    /*!
     * \brief Receives the reply data of the DeleteFolder request.
     *
     * Will delete the folder identified by \a id in the local storage and emits the deletedFolder() signal.
     */
    virtual void folderDeleted(qint64 id) = 0;

    /*!
     * \brief Receives the reply data of the MarkFolderRead request.
     *
     * Will mark all items in the folder identified by \a id that have a lower ID than \a newestItem as read in the local storage.
     * Will than emit the markedReadFolder() signal.
     */
    virtual void folderMarkedRead(qint64 id, qint64 newestItem) = 0;

    /*!
     * \brief Receives the reply data of the GetFeeds request.
     *
     * Implement this in a derived class to store feed data, for example in a local SQL database.
     * You may want to emit requestedFeeds() in your implementation after you processed the data.
     *
     * \par Example JSON response data
     *
     * \code{.json}
     * {
     *   "feeds": [
     *     {
     *       "id": 39,
     *       "url": "http://feeds.feedburner.com/oatmealfeed",
     *       "title": "The Oatmeal - Comics, Quizzes, & Stories",
     *       "faviconLink": "http://theoatmeal.com/favicon.ico",
     *       "added": 1367063790,
     *       "folderId": 4,
     *       "unreadCount": 9,
     *       "ordering": 0, // 0 means no special ordering, 1 means oldest first, 2 newest first, new in 5.1.0
     *       "link": "http://theoatmeal.com/",
     *       "pinned": true // if a feed should be sorted before other feeds, added in 6.0.3,
     *       "updateErrorCount": 0,
     *       "lastUpdateError": "error message here"
     *     }, // etc
     *   ],
     *   "starredCount": 2,
     *   "newestItemId": 3443  // only sent if there are items
     * }
     * \endcode
     */
    virtual void feedsRequested(const QJsonDocument &json) = 0;

    /*!
     * \brief Receives the reply data of the CreateFeed request.
     *
     * Implement this in a derived class to store new feed data, for example in a local SQL databse.
     * You my want to emit createdFeed() in your implementation after you processed the data.
     *
     * \par Example JSON response data
     *
     * \code{.json}
     * {
     *   "feeds": [
     *     {
     *       "id": 39,
     *       "url": "http://feeds.feedburner.com/oatmealfeed",
     *       "title": "The Oatmeal - Comics, Quizzes, & Stories",
     *       "faviconLink": "http://theoatmeal.com/favicon.ico",
     *       "added": 1367063790,
     *       "folderId": 4,
     *       "unreadCount": 9,
     *       "ordering": 0, // 0 means no special ordering, 1 means oldest first, 2 newest first, new in 5.1.0
     *       "link": "http://theoatmeal.com/",
     *       "pinned": true // if a feed should be sorted before other feeds, added in 6.0.3
     *     }
     *   ],
     *   "newestItemId": 23 // only sent if there are items
     * }
     * \endcode
     */
    virtual void feedCreated(const QJsonDocument &json) = 0;

    /*!
     * \brief Receives the reply data of the DeleteFeed request.
     *
     * Will delete the feed identified by \a id in the local storage and emits the deletedFeed() signal.
     */
    virtual void feedDeleted(qint64 id) = 0;

    /*!
     * \brief Receives the reply data of the MoveFeed request.
     *
     * Will move the feed identified by \a id to the \a targetFolder id and emits the movedFeed() signal.
     */
    virtual void feedMoved(qint64 id, qint64 targetFolder) = 0;

    /*!
     * \brief Receives the reply data of the RenameFeed request.
     *
     * Will rename the feed identified by \a id to the \a newTitle and emits the renamedFeed() signal.
     */
    virtual void feedRenamed(qint64 id, const QString &newTitle) = 0;

    /*!
     * \brief Receives the reply data of the MarkFeedRead request.
     *
     * Will mark all items in the feed identified by \a id that have a lower ID than \a newestItem as read in the local storage.
     * Will than emit the markedReadFeed() signal.
     */
    virtual void feedMarkedRead(qint64 id, qint64 newestItem) = 0;

    /*!
     * \brief Receives the reply data of the GetItems request.
     *
     * Implement this in a derived class to store item data, for example in a local SQL database.
     * You may want to emit requestedItems() in your implementation after you processed the data.
     *
     * \par Example JSON response data
     * {
     *    "items": [
     *       {
     *          "id": 3443,
     *          "guid": "http://grulja.wordpress.com/?p=76",
     +          "guidHash": "3059047a572cd9cd5d0bf645faffd077",
     *          "url": "http://grulja.wordpress.com/2013/04/29/plasma-nm-after-the-solid-sprint/",
     *          "title": "Plasma-nm after the solid sprint",
     *          "author": "Jan Grulich (grulja)",
     *          "pubDate": 1367270544,
     *          "body": "<p>At first I have to say...</p>",
     *          "enclosureMime": null,
     *          "enclosureLink": null,
     *          "feedId": 67,
     *          "unread": true,
     *          "starred": false,
     *          "lastModified": 1367273003,
     *          "fingerprint": "aeaae2123"
     *       }, // etc
     *    ]
     * }
     * \code{.json}
     *
     * \endcode
     */
    virtual void itemsRequested(const QJsonDocument &json) = 0;

    /*!
     * \brief Receives the reply data for the MarkItems request.
     *
     * Will mark the items identified by their ID in the \a itemIds as read if \a unread is set to \c false,
     * otherwise it will mark them as unread.
     * You should emit markedItems() in your implementation after you processed the data to update connected models.
     *
     * \param itemIds   IDs of articles that have been marked as read or unread
     * \param unread    \c true if the articles have been marked as unread, \c false if marked as read
     */
    virtual void itemsMarked(const IdList &itemIds, bool unread) = 0;

    /*!
     * \brief Receives the reply data for the StarItems request.
     *
     * The lists contains the item/article IDs and the guid hash of the items/articles that have been either starred or unstarred.
     * You should emit starredItems() in your implementation after you processed the data to update connected models.
     *
     * \param articles  list of pairs of feed ID and guid hash of starred/unstarred articles
     * \param star      \c true if the articles have been starred, \c false if they have been unstarred
     */
    virtual void itemsStarred(const QList<QPair<qint64, QString>> &articles, bool star) = 0;

    /*!
     * \brief Receives the reply data for the MarkItem request.
     *
     * Will mark the item identified by \a itemId in the local storage as read or unread.
     *
     * \param itemId    ID of the item/article that has been marked
     * \param unread    \c true if the item has been marked as unread, \c false if marked as read
     */
    virtual void itemMarked(qint64 itemId, bool unread) = 0;

    /*!
     * \brief Receives the reply data from the StarItem request.
     *
     * Will star or unstar the item/article identified by \a itemId and \a guidHash in the local storage.
     *
     * \param feedId    ID of the feed the item/article that has been starred or unstarred belongs to
     * \param guidHash  global unique ID hash of the item/article that has been starred or unstarred
     * \param star      \c true if the article has been starred, \c false if it has been unstarred
     */
    virtual void itemStarred(qint64 feedId, const QString &guidHash, bool star) = 0;

    /*!
     * \brief Receives the reply data from the MarkAllItemsRead request.
     *
     * Will mark all items in the local database as read which have an ID lower or equal to \a newestItemId.
     */
    virtual void allItemsMarkedRead(qint64 newestItemId) = 0;

protected:
    /*!
     * \brief Set this to \a true when the storage has finished it's initialization.
     * \sa ready
     */
    void setReady(bool nReady);

    /*!
     * \brief Sets the Error object.
     * \sa error
     */
    void setError(Error *nError);

    virtual QString limitBody(const QString &body, int limit) const;

    const QScopedPointer<AbstractStoragePrivate> d_ptr;
    AbstractStorage(AbstractStoragePrivate &dd, QObject *parent = nullptr);

protected Q_SLOTS:
    /*!
     * \brief Sets the total number of unread articles.
     * \sa totalUnread
     */
    virtual void setTotalUnread(quint16 nTotalUnread);

    /*!
     * \brief Sets the total number of starred articles.
     * \sa starred
     */
    virtual void setStarred(quint16 nStarred);

Q_SIGNALS:
    /*!
     * \brief Emit this after folders have been received and processed.
     *
     * Best location to emit this signal is your implementation of foldersRequested().
     *
     * \c updatedFolders should contain a list of the database IDs and names of updated folders, \c newFolders should
     * contain a list of databaes IDs and names of new folders, and \c deletedFolders should contain a list of database IDs
     * of deleted folders.
     */
    void requestedFolders(const QList<QPair<qint64, QString> > &updatedFolders, const QList<QPair<qint64, QString> > &newFolders, const IdList &deletedFolders);

    /*!
     * \brief Emit this after a new folder has been created.
     *
     * Best loaction to emit this signal is your implementation of folderCreated(). The signal
     * has to contain the \a id and the \a name of the new folder.
     */
    void createdFolder(qint64 id, const QString &name);

    /*!
     * \brief Emit this after a folder has been renamed.
     *
     * Best location to emit this signal is your implementation of folderRenamed(). The signal
     * has to contain the \a id and the \a newName of the folder.
     */
    void renamedFolder(qint64 id, const QString &newName);

    /*!
     * \brief Emit this after a folder has been deleted.
     *
     * Best location to emit this signal is your implementation of folderDeleted(). The signal
     * has to contain the \a id fo the deleted folder.
     */
    void deletedFolder(qint64 id);

    /*!
     * \brief Emit this after a folder has been marked as read.
     *
     * Best location to emit this signal is your implementation of folderMarkedRead(). The signal
     * has to contain the \a id of the folder that has been marked as read as well as the ID of
     * the \a newestItem that has been marked as read.
     */
    void markedReadFolder(qint64 id, qint64 newestItem);

    /*!
     * \brief Emitted whenever the ready property changes.
     */
    void readyChanged(bool ready);

    /*!
     * \brief Emitted whenever the error property changes.
     */
    void errorChanged(Error *error);

    /*!
     * \brief This signal is emitted if the amount of total unread articles changes.
     * \sa totalUnread
     */
    void totalUnreadChanged(quint16 totalUnread);

    /*!
     * \brief This signal is emitted if the amount of total starred articles changes.
     * \sa starred
     */
    void starredChanged(quint16 starred);

    /*!
     * \brief Emit this after feeds have been received and processed.
     *
     * Best location to emit this signal is your implementation of feedsRequested().
     *
     * Every argument of the signal should contain a list of feed IDs that are either updated, new or deleted.
     */
    void requestedFeeds(const IdList &updatedFeeds, const IdList &newFeeds, const IdList &deletedFeeds);

    /*!
     * \brief Emit this after a new feed has been created.
     *
     * Best loaction to emit this signal is your implementation of feedCreated(). The signal
     * has to contain the \a id of the new feed it's parent \a folderId.
     */
    void createdFeed(qint64 id, qint64 folderId);

    /*!
     * \brief Emit this after a feed has been deleted.
     *
     * Best location to emit this signal is your implementation of feedDeleted(). The signal
     * has to contain the \a id fo the deleted feed.
     */
    void deletedFeed(qint64 id);

    /*!
     * \brief Emit this signal after a feed has been moved to another folder.
     *
     * Best location to emit this signal is your implementation of feedMoved(). The signal
     * has to contain the \a id of the moved feed and the \a targetFolder id.
     */
    void movedFeed(qint64 id, qint64 targetFolder);

    /*!
     * \brief Emit this signal after a feed has been renamed.
     *
     * Best location to emit this signal is your implementation of feedRename(). The signal
     * has to contain the \a id of the renamed feed and the \a newName.
     */
    void renamedFeed(qint64 id, const QString &newName);

    /*!
     * \brief Emit this after a feed has been marked as read.
     *
     * Best location to emit this signal is your implementation of feedMarkedRead(). The signal
     * has to contain the \a id of the feed that has been marked as read as well as the ID of
     * the \a newestItem that has been marked as read.
     */
    void markedReadFeed(qint64 id, qint64 newestItem);

    /*!
     * \brief Emit this after items/articles have been received and processed.
     *
     * Best location to emit this signal is your implementation of itemsRequested().
     *
     * Every argument of the signal should contain a list of item IDs that are either updated, new or deleted.
     *
     * \param updatedItems  list of IDs from items that have been updated
     * \param newItems      list of IDs from items that are new in the local storage
     * \param deletedItems  list of IDs from items that have been remove from the local storage
     */
    void requestedItems(const IdList &updatedItems, const IdList &newItems, const IdList &deletedItems);

    /*!
     * \brief Emit this after items/articles have been marked as read or unread.
     *
     * Best location to emit this signal is your implementation of itemsMarked().
     *
     * The lists have to conatin the item/article IDs that haven been marked as read or unread.
     *
     * \param itemIds   IDs of articles that have been marked as read or unread
     * \param unread    \c true if the articles have been marked as unread, \c false if marked as read
     */
    void markedItems(const IdList &itemIds, bool unread);

    /*!
     * \brief Emit this after items/articles have been starred or unstarred.
     *
     * Best location to emit this signal is your implementation of itemsStarred().
     *
     * \param articles  list of pairs of feed ID and guid hash of starred/unstarred articles
     * \param star      \c true if the articles have been starred, \c false if they have been unstarred
     */
    void starredItems(const QList<QPair<qint64, QString>> &articles, bool star);

    /*!
     * \brief Emit this after an item/article has been marked read or unread.
     *
     * Best location to emit this signal is your implementation of itemMarked().
     *
     * \param itemId    the ID of the item/article that has been marked as read or unread
     * \param unread    \c true if the article has been marked as unread, \c false if marked as read
     */
    void markedItem(qint64 itemId, bool unread);

    /*!
     * \brief Emit this after an item/article has been starred or unstarred.
     *
     * Best location to emit this signal is your implementation of itemStarred().
     *
     * \param feedId    the ID of the feed the item/article that has been starred or unstarred belongs to
     * \param guidHash  the global unique ID hash of the article that has been starred or unstarred
     * \param starred   \c true if the article has been starred, \c false if it has been unstarred
     */
    void starredItem(qint64 feedId, const QString &guidHash, bool starred);

    /*!
     * \brief Emit this signal after all items/articles have been marked as read.
     *
     * Best location to emit this signal is your implementation of allItemsMarkedRead().
     *
     * \param newestItemId  highest/newest ID of the local available items/articles
     */
    void markedAllItemsRead(qint64 newestItemId);

    /*!
     * \brief Emit this after getArticlesAsync() has been called and articles have been queried.
     *
     * Be aware that the objects in the list might have been created in a different thread when you
     * connect to this signal.
     *
     * \param articles list of Article objects
     */
    void gotArticlesAsync(const ArticleList &articles);


private:
    Q_DISABLE_COPY(AbstractStorage)
    Q_DECLARE_PRIVATE(AbstractStorage)
};

}

#endif // FUOTENABSTRACTSTORAGE_H
