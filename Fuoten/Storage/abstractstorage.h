/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENABSTRACTSTORAGE_H
#define FUOTENABSTRACTSTORAGE_H

#include <QObject>
#include "../fuoten.h"
#include "../fuoten_global.h"
#include "../Helpers/abstractnotificator.h"
#include "fuoten_export.h"

namespace Fuoten {

/*!
 * \brief Helper struct containing query arguments.
 *
 * Used by AbstractStrorage::getFolders(), AbstractStroage::getFeeds() and AbstractStorage::getArticles() to set the arguments used to query a list
 * of objects. Not all members of this struct are used by every query.
 */
struct FUOTEN_EXPORT QueryArgs {
    FuotenEnums::SortingRole sortingRole = FuotenEnums::ID; /**< The role/value used to sort the result by. */
    Qt::SortOrder sortOrder = Qt::AscendingOrder;           /**< The sorting order. */
    qint64 parentId = -1;                                   /**< ID of a parent object (Feed or Folder). Used by feed and article queries, defaults to -1. */
    FuotenEnums::Type parentIdType = FuotenEnums::All;      /**< Only usable on articles where it can can be FuotenEnums::Folder or FuotenEnums::Feed. Defaults to FuotenEnums::All */
    IdList inIds;                                           /**< A list of IDs the records are compared with. Defaults to an empty list. */
    FuotenEnums::Type inIdsType = FuotenEnums::All;         /**< Type of of the inIds list to compare. Related to the object to query, it can be FuotenEnums::Folder, FuotenEnums::Feed or FuotenEnums::Item. Defaults to FuotenEnums::All. */
    bool unreadOnly = false;                                /**< If true, only unread articles or folders and feeds without unread content are returned. */
    bool starredOnly = false;                               /**< Only valid for article queries. If true, only starrred articles are returned. */
    int limit = 0;                                          /**< Limits the result to the specified number of objects. Defaults to \c 0 to return all objects. */
    int bodyLimit = -1;                                     /**< Only valid for article queries. Limits the size of the body text in number of characters. Values lower than \c 0 will return no body text, \c 0 will return the full body text, any other positive value will return a body stripped from HTML tags and limited to the amount of characters. */
    bool queuedOnly = false;                                /**< Only valid for article queries. Will only return items/articles that are queued. */
};

class Folder;
class Feed;
class Error;
class Article;
class AbstractConfiguration;
class AbstractStoragePrivate;
class AbstractNotificator;

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
class FUOTEN_EXPORT AbstractStorage : public QObject
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
     * <TABLE><TR><TD>int</TD><TD>totalUnread() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>totalUnreadChanged(int totalUnread)</TD></TR></TABLE>
     *
     * \sa setTotalUnread()
     */
    Q_PROPERTY(int totalUnread READ totalUnread NOTIFY totalUnreadChanged)
    /*!
     * \brief Amount of starred items.
     *
     * \par Access functions:
     * <TABLE><TR><TD>int</TD><TD>starred() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>starredChanged(int starred)</TD></TR></TABLE>
     *
     * \sa setStarred()
     */
    Q_PROPERTY(int starred READ starred NOTIFY starredChanged)
    /*!
     * \brief Returns true while the storage is in operation.
     *
     * This will only be set to true by pure storage functions (like queue functions), not when processing requested data.
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>inOperation() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>inOperationChanged(bool inOperation)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool inOperation READ inOperation NOTIFY inOperationChanged)
    /*!
     * \brief Pointer to an AbstractConfiguration derived class.
     *
     * \par Access functions:
     * <TABLE><TR><TD>AbstractConfiguration*</TD><TD>configuration() const</TD></TR><TR><TD>void</TD><TD>setConfiguration(AbstractConfiguration *nConfiguration)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>configurationChanged(AbstractConfiguration *configuration)</TD></TR></TABLE>
     */
    Q_PROPERTY(AbstractConfiguration *configuration READ configuration WRITE setConfiguration NOTIFY configurationChanged)
    /*!
     * \brief Pointer to an object derived from AbstractNotificator.
     *
     * Set a notificator to notify users about errors and events. This is not mandatory. You have to derive your own notificator that uses
     * the notification system of the target platform.
     *
     * If no notificator has been set via setNotificator(), the one set via Component::setDefaultConfigurator() will be used - if any has
     * been set. If you do not set a notificator either per instance or global, this property will hold a \c nullptr. If a notificator is
     * set to an instance of this class via setNotificator(), this notificator will take precedence over the global default notificator
     * object (if any set).
     *
     * \par Access functions:
     * \li AbstractNotificator* notificator() const
     * \li void setNotificator(AbstractNotificator *notificator);
     *
     * \par Notifier signal:
     * \li void notificatorChanged(AbstractNotificator *notificator);
     *
     * \sa notify()
     */
    Q_PROPERTY(Fuoten::AbstractNotificator *notificator READ notificator WRITE setNotificator NOTIFY notificatorChanged)
public:
    /*!
     * \brief Constructs a new abstract local storage with the given \a parent.
     */
    explicit AbstractStorage(QObject *parent = nullptr);

    /*!
     * \brief Deconstructs the %AbstractStorage object.
     */
    ~AbstractStorage() override;

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
    virtual int totalUnread() const;

    /*!
     * \brief Returns the total number of starred articles.
     * \sa starred
     */
    virtual int starred() const;

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

    /*!
     * \brief Returns the full body of an Article identified by \a id.
     */
    Q_INVOKABLE virtual QString getArticleBody(qint64 id) = 0;

    /*!
     * \brief Enqueues an \a action for the given \a article.
     *
     * When enqueueing an article, the performed action should already be saved in the local storage
     * together with the queue action. If the article is marked as read for example, add the action to
     * your local queue and also mark the article as read in your local storage.
     *
     * The default implementation does nothing and returns \c false.
     *
     * \param action    the action that should be enqueued
     * \param article   pointer to the Article object that should be enqueued
     * \return \c true if the enqueue was successful, otherwise \c false
     */
    virtual bool enqueueItem(FuotenEnums::QueueAction action, Article *article);

    /*!
     * \brief Adds all articles older than \a newestItemId in the feed identified by \a feedId as read to the local queue.
     *
     * After successfully adding the items/articles to the local queue, emit the markedReadFeedInQueue() signal.
     * The default implementation does nothing and returns \c false.
     *
     * \param feedId        ID of the feed to be marked as read
     * \param newestItemId  ID of the newest item in the feed
     * \return  \c true on success, otherwise \c false
     */
    virtual bool enqueueMarkFeedRead(qint64 feedId, qint64 newestItemId);

    /*!
     * \brief Adds all articles older than \a newestItemId in the folder identified \a folderId as read to the local queue.
     *
     * After successfully adding the items/articles to the local queue, emit the markedReadFolderInQueue() signal.
     * The default implementation does nothing and returns \c false.
     *
     * \param folderId      ID of the folder to be marked as read
     * \param newestItemId  ID of the newest item in the folder
     * \return \c true on success, otherwise \c false
     */
    virtual bool enqueueMarkFolderRead(qint64 folderId, qint64 newestItemId);

    /*!
     * \brief Adds all local articles that are unread to the queue and marks them as read.
     *
     * After successfully addint the items/articles to the local queue, emit the markedAllItemsReadInQueue() signal.
     * The default implementation does nothing and returns \c false.
     *
     * \return \c true on success, otherwise \c false
     */
    Q_INVOKABLE virtual bool enqueueMarkAllItemsRead();

    /*!
     * \brief Getter function for the \link AbstractStorage::inOperation inOperation \endlink property.
     * \sa AbstractStorage::setInOperation(), AbstractStorage::inOperationChanged()
     */
    bool inOperation() const;

    /*!
     * \brief Getter function for the \link AbstractStorage::configuration configuration \endlink property.
     * \sa AbstractStorage::setConfiguration(), AbstractStorage::configurationChanged()
     */
    AbstractConfiguration *configuration() const;

    /*!
     * \brief Setter function for the \link AbstractStorage::configuration configuration \endlink property.
     * Emits the configurationChanged() signal if \a nConfiguration is not equal to the stored value.
     * \sa AbstractStorage::configuration(), AbstractStorage::configurationChanged()
     */
    void setConfiguration(AbstractConfiguration *nConfiguration);

    /*!
     * \brief Getter function for the \link AbstractStorage::notificator notificator \endlink property.
     * \sa setNotificator(), notificatorChanged()
     */
    AbstractNotificator *notificator() const;

    /*!
     * \brief Setter function for the \link AbstractStorage::notificator notificator \endlink property.
     * \sa notificator(), notificatorChanged()
     */
    void setNotificator(AbstractNotificator *notificator);

    /*!
     * \brief Clears the local queue. Does not revert the action itself.
     *
     * The default implementation does nothing. When reimplementing this function, only remove the queue entry
     * but do not revert the action already done locally. It will be called after working the queue.
     */
    virtual void clearQueue();

    /*!
     * \brief Removes all data from the storage.
     *
     * The default implementation does nothing. When reimplementing this, emit storageCleared() after
     * successfully clear all stored data. Do not forget to also reset last sync time with
     * AbstractConfiguration::setLastSync().
     */
    Q_INVOKABLE virtual void clearStorage();

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
     *
     * \code{.json}
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
    virtual void itemsMarked(const Fuoten::IdList &itemIds, bool unread) = 0;

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

    /*!
     * \brief Convert a JSON value into an integer id.
     *
     * Based on the News App API version, IDs are either returned as numbers
     * or as strings. This will check the JSON value type and convert it into
     * an appropriate id value.
     *
     * \since 0.8.2
     */
    static qint64 getIdFromJson(const QJsonValue &value);

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

    /*!
     * \brief Setter function for the \link AbstractStorage::inOperation inOperation \endlink property.
     * Emits the inOperationChanged() signal if \a nInOperation is not equal to the stored value.
     * \sa AbstractStorage::inOperation(), AbstractStorage::inOperationChanged()
     */
    void setInOperation(bool nInOperation);

    virtual QString limitBody(const QString &body, int limit) const;

    /*!
     * \brief Checks if a \link AbstractStorage::notificator notificator \endlink has been set and will use it to notify about an occured error.
     * \sa AbstractNotificator::notify(const Error *e)
     */
    void notify(const Error *e) const;

    /*!
     * \brief Checks if a \link AbstractStorage::notificator notificator \endlink has been set and will use it to notify the user.
     * \sa AbstractNotificator::notify(AbstractNotificator::Type type, QtMsgType severity, const QVariant &data)
     */
    void notify(AbstractNotificator::Type type, QtMsgType severity, const QVariant &data) const;

    const QScopedPointer<AbstractStoragePrivate> d_ptr;
    AbstractStorage(AbstractStoragePrivate &dd, QObject *parent = nullptr);

protected Q_SLOTS:
    /*!
     * \brief Sets the total number of unread articles.
     * \sa totalUnread
     */
    virtual void setTotalUnread(int nTotalUnread);

    /*!
     * \brief Sets the total number of starred articles.
     * \sa starred
     */
    virtual void setStarred(int nStarred);

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
    void requestedFolders(const QList<QPair<qint64, QString> > &updatedFolders, const QList<QPair<qint64, QString> > &newFolders, const Fuoten::IdList &deletedFolders);

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
     * \brief Emit this after a folder has been marked as read in the local queue.
     *
     * Best location to emit this signal is your implementation of enqueueMarkFolderRead().
     *
     * \param folderId      ID of the folder that has been marked as read in the local queue
     * \param newestItemId  ID of the newest item in folder that has been marked as read in the local queue
     */
    void markedReadFolderInQueue(qint64 folderId, qint64 newestItemId);

    /*!
     * \brief Emitted whenever the ready property changes.
     */
    void readyChanged(bool ready);

    /*!
     * \brief Emitted whenever the error property changes.
     */
    void errorChanged(Fuoten::Error *error);

    /*!
     * \brief This signal is emitted if the amount of total unread articles changes.
     * \sa totalUnread
     */
    void totalUnreadChanged(int totalUnread);

    /*!
     * \brief This signal is emitted if the amount of total starred articles changes.
     * \sa starred
     */
    void starredChanged(int starred);

    /*!
     * \brief Emit this after feeds have been received and processed.
     *
     * Best location to emit this signal is your implementation of feedsRequested().
     *
     * Every argument of the signal should contain a list of feed IDs that are either updated, new or deleted.
     */
    void requestedFeeds(const Fuoten::IdList &updatedFeeds, const Fuoten::IdList &newFeeds, const Fuoten::IdList &deletedFeeds);

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
     * \brief Emit this after a feed has been marked as read in the local queue.
     * \param feedId        ID of the feed that has been marked as read
     * \param newestItemId  ID of the newest item in the feed
     */
    void markedReadFeedInQueue(qint64 feedId, qint64 newestItemId);

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
    void requestedItems(const Fuoten::IdList &updatedItems, const Fuoten::IdList &newItems, const Fuoten::IdList &deletedItems);

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
    void markedItems(const Fuoten::IdList &itemIds, bool unread);

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
     * \brief Emit this signal after all items/articles have been marked as read in the local queue.
     *
     * Best location to emit this signal is your implemetation of enqueueMarkAllItemsRead().
     */
    void markedAllItemsReadInQueue();

    /*!
     * \brief Emit this after getArticlesAsync() has been called and articles have been queried.
     *
     * Be aware that the objects in the list might have been created in a different thread when you
     * connect to this signal.
     *
     * \param articles list of Article objects
     */
    void gotArticlesAsync(const Fuoten::ArticleList &articles);

    /*!
     * \brief This is emitted if the value of the \link AbstractStorage::inOperation inOperation \endlink property changes.
     * \sa AbstractStorage::inOperation(), AbstractStorage::setInOperation()
     */
    void inOperationChanged(bool inOperation);

    /*!
     * \brief This is emitted if the value of the \link AbstractStorage::configuration configuration \endlink property changes.
     * \sa AbstractStorage::configuration(), AbstractStorage::setConfiguration()
     */
    void configurationChanged(Fuoten::AbstractConfiguration *configuration);

    /*!
     * \brief Notifier signal for the \link AbstractStorage::notificator notificator \endlink property.
     * \sa setNotificator(), notificator()
     */
    void notificatorChanged(Fuoten::AbstractNotificator *notificator);

    /*!
     * \brief This is emitted after the local queue has been cleared.
     *
     * Emit this in your implementation of clearQueue().
     */
    void queueCleared();

    /*!
     * \brief This is emitted after the local storage has been cleared.
     *
     * Emit this in your implementation of clearStorage() after all local data
     * has been deleted successfully.
     */
    void storageCleared();

private:
    Q_DISABLE_COPY(AbstractStorage)
    Q_DECLARE_PRIVATE(AbstractStorage)
};

}

#endif // FUOTENABSTRACTSTORAGE_H
