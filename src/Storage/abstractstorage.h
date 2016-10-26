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

class Folder;
class Feed;
class Error;
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
     * \brief Returns a list of Folder objects from the local storage.
     *
     * The returned list will be sorted by \a sortingRole and \a sortOrder. If \a ids is not empty,
     * only folders with IDs of \a idType from the list will be returned. The \a idType specifies the
     * id the folder is compared with. If the \a idType is not one of out of the table below, it will
     * be treated as FuotenEnums::Folder.
     *
     * <TABLE>
     * <TR><TD>FuotenEnums::Folder</TD><TD>Only folders with and ID in \a ids will be returned</TD></TR>
     * <TR><TD>FuotenEnums::Feed</TD><TD>Only folders will be returned that contain feeds with an ID in \a ids</TD></TR>
     * <TR><TD>FuotenEnums::Item</TD><TD>Only folders will be returned that contain items with an ID in \a ids</TD></TR>
     * </TABLE>
     *
     * The Folder objects in the returned list will have their parent set to \c nullptr.
     */
    virtual QList<Folder*> getFolders(FuotenEnums::SortingRole sortingRole = FuotenEnums::Name, Qt::SortOrder sortOrder = Qt::AscendingOrder, const QList<qint64> &ids = QList<qint64>(), FuotenEnums::Type idType = FuotenEnums::Folder) = 0;
    
    /*!
     * \brief Returns a list of Feed objects from the local storage.
     *
     * The returned list will be sorted by \a sortingRole and \a sortOrder. If \a ids is not empty,
     * only folders with IDs from the list will be returned. The Feed objects in the returned list will
     * have their parent set to \c nullptr.
     */
    virtual QList<Feed*> getFeeds(FuotenEnums::SortingRole sortingRole = FuotenEnums::Name, Qt::SortOrder sortOrder = Qt::AscendingOrder, const QList<qint64> &ids = QList<qint64>()) = 0;

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
     * \brief Sets the total number of unread articles.
     * \sa totalUnread
     */
    virtual void setTotalUnread(quint16 nTotalUnread);

    /*!
     * \brief Sets the total number of starred articles.
     * \sa starred
     */
    virtual void setStarred(quint16 nStarred);

    const QScopedPointer<AbstractStoragePrivate> d_ptr;
    AbstractStorage(AbstractStoragePrivate &dd, QObject *parent = nullptr);

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
    void requestedFolders(const QList<QPair<qint64, QString> > &updatedFolders, const QList<QPair<qint64, QString> > &newFolders, const QList<qint64> &deletedFolders);

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
     * \brief Emit this after have have been received and processed.
     *
     * Best location to emit this signal is your implementation of feedsRequested().
     *
     * Every argument of the signal should contain a list of feed IDs that are either updated, new or deleted.
     */
    void requestedFeeds(QList<qint64> &updatedFeeds, QList<qint64> &newFeeds, QList<qint64> &deletedFeeds);

    /*!
     * \brief Emit this after a new feed has been created.
     *
     * Best loaction to emit this signal is your implementation of feedCreated(). The signal
     * has to contain the \a id of the new feed.
     */
    void createdFeed(qint64 id);

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


private:
    Q_DISABLE_COPY(AbstractStorage)
    Q_DECLARE_PRIVATE(AbstractStorage)
};

}

#endif // FUOTENABSTRACTSTORAGE_H
