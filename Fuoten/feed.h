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

#ifndef FUOTENFEED_H
#define FUOTENFEED_H

#include <QObject>
#include <QUrl>
#include <QDateTime>
#include "baseitem.h"
#include "fuoten_global.h"

namespace Fuoten {

class FeedPrivate;

/*!
 * \brief Contains information about a single feed.
 *
 * \headerfile "" <Fuoten/Feed>
 */
class FUOTENSHARED_EXPORT Feed : public BaseItem
{
    Q_OBJECT
    /*!
     * \brief The URL of the feed.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QUrl</TD><TD>url() const</TD></TR><TR><TD>void</TD><TD>setUrl(const QUrl &nUrl)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>urlChanged(const QUrl &url)</TD></TR></TABLE>
     */
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    /*!
     * \brief The title of the feed.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>title() const</TD></TR><TR><TD>void</TD><TD>setTitle(const QString &nTitle)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>titleChanged(const QString &title)</TD></TR></TABLE>
     */
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    /*!
     * \brief Date and time the feed has been added.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QDateTime</TD><TD>added() const</TD></TR><TR><TD>void</TD><TD>setAdded(const QDateTime &nAdded)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>addedChanged(const QDateTime &added)</TD></TR></TABLE>
     */
    Q_PROPERTY(QDateTime added READ added WRITE setAdded NOTIFY addedChanged)
    /*!
     * \brief The ID of the folder, the feed belongs to.
     *
     * \par Access functions:
     * <TABLE><TR><TD>qint64</TD><TD>folderId() const</TD></TR><TR><TD>void</TD><TD>setFolderId(qint64 nFolderId)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>folderIdChanged(qint64 folderId)</TD></TR></TABLE>
     */
    Q_PROPERTY(qint64 folderId READ folderId WRITE setFolderId NOTIFY folderIdChanged)
    /*!
     * \brief The name of the folder the feed belongs to.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>folderName() const</TD></TR><TR><TD>void</TD><TD>setFolderName(const QString &nFolderName)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>folderNameChanged(const QString &folderName)</TD></TR></TABLE>
     */
    Q_PROPERTY(QString folderName READ folderName WRITE setFolderName NOTIFY folderNameChanged)
    /*!
     * \brief The number of unread items in this feed.
     *
     * \par Access functions:
     * <TABLE><TR><TD>uint</TD><TD>unreadCount() const</TD></TR><TR><TD>void</TD><TD>setUnreadCount(uint nUnreadCount)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>unreadCountChanged(uint unreadCount)</TD></TR></TABLE>
     */
    Q_PROPERTY(uint unreadCount READ unreadCount WRITE setUnreadCount NOTIFY unreadCountChanged)
    /*!
     * \brief The ordering of the articles in this feed on the server.
     *
     * \par Access functions:
     * <TABLE><TR><TD>FeedOrdering</TD><TD>ordering() const</TD></TR><TR><TD>void</TD><TD>setOrdering(FeedOrdering nOrdering)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>orderingChanged(FeedOrdering ordering)</TD></TR></TABLE>
     */
    Q_PROPERTY(Fuoten::Feed::FeedOrdering ordering READ ordering WRITE setOrdering NOTIFY orderingChanged)
    /*!
     * \brief Link to the homepage of the feed.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QUrl</TD><TD>link() const</TD></TR><TR><TD>void</TD><TD>setLink(const QUrl &nLink)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>linkChanged(const QUrl &link)</TD></TR></TABLE>
     */
    Q_PROPERTY(QUrl link READ link WRITE setLink NOTIFY linkChanged)
    /*!
     * \brief True if the feed is pinned.
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>pinned() const</TD></TR><TR><TD>void</TD><TD>setPinned(bool nPinned)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>pinnedChanged(bool pinned)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool pinned READ pinned WRITE setPinned NOTIFY pinnedChanged)
    /*!
     * \brief Indicating that the feed has faild to update that many times.
     *
     * \par Access functions:
     * <TABLE><TR><TD>uint</TD><TD>updateErrorCount() const</TD></TR><TR><TD>void</TD><TD>setUpdateErrorCount(uint nUpdateErrorCount)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>updateErrorCountChanged(uint updateErrorCount)</TD></TR></TABLE>
     */
    Q_PROPERTY(uint updateErrorCount READ updateErrorCount WRITE setUpdateErrorCount NOTIFY updateErrorCountChanged)
    /*!
     * \brief Contains the last update error message, if any.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>lastUpdateError() const</TD></TR><TR><TD>void</TD><TD>setLastUpdateError(const QString &nLastUpdateError)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>lastUpdateErrorChanged(const QString &lastUpdateError)</TD></TR></TABLE>
     */
    Q_PROPERTY(QString lastUpdateError READ lastUpdateError WRITE setLastUpdateError NOTIFY lastUpdateErrorChanged)
    /*!
     * \brief The URL to the feed's favicon.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QUrl</TD><TD>faviconLink() const</TD></TR><TR><TD>void</TD><TD>setFaviconLink(const QUrl &nFaviconLink)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>faviconLinkChanged(const QUrl &faviconLink)</TD></TR></TABLE>
     */
    Q_PROPERTY(QUrl faviconLink READ faviconLink WRITE setFaviconLink NOTIFY faviconLinkChanged)
public:
    /*!
     * \brief Type of the article ordering in this feed on the server.
     */
    enum FeedOrdering {
        NoSpecial   = 0,    /**< No special ordering */
        OldestFirst = 1,    /**< Oldest first */
        NewestFirst = 2,    /**< Newest first */
    };
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
    Q_ENUM(FeedOrdering)
#else
    Q_ENUMS(FeedOrdering)
#endif


    /*!
     * \brief Constructs a new empty Feed object with the given \a parent.
     */
    explicit Feed(QObject *parent = nullptr);


    /*!
     * \brief Constructs a new Feed object with the given arguments.
     */
    Feed(qint64 id, qint64 folderId, const QString &title, const QUrl &url, const QUrl &link, const QDateTime &added, uint unreadCount, Feed::FeedOrdering ordering, bool pinned, uint updateErrorCount, const QString &lastUpdateError, const QUrl &faviconLink, const QString &folderName, QObject *parent = nullptr);

    /*!
     * \brief Returns the URL of the feed.
     * \sa url
     */
    QUrl url() const;
    /*!
     * \brief Returns the title of the feed.
     * \sa title
     */
    QString title() const;
    /*!
     * \brief Returns the date and time the feed has been added.
     * \sa added
     */
    QDateTime added() const;
    /*!
     * \brief Return the ID of the Folder the feed belongs to.
     * \sa folderId
     */
    qint64 folderId() const;
    /*!
     * \brief Returns the name of the Folder the feed belongs to.
     * \sa folderName
     */
    QString folderName() const;
    /*!
     * \brief Returns the count of unread items in this feed.
     * \sa unreadCount
     */
    uint unreadCount() const;
    /*!
     * \brief Returns the ordering of the items in this feed on the server.
     * \sa ordering
     */
    FeedOrdering ordering() const;
    /*!
     * \brief Return the URL of the feed's homepage.
     * \sa link
     */
    QUrl link() const;
    /*!
     * \brief Returns true if this feed is pinned.
     * \sa pinned
     */
    bool pinned() const;
    /*!
     * \brief Returns the count of update errors.
     * \sa updateErrorCount
     */
    uint updateErrorCount() const;
    /*!
     * \brief Returns the message of the last update error.
     * \sa lastUpdateError
     */
    QString lastUpdateError() const;
    /*!
     * \brief Returns the URL of the feed's favion.
     * \sa faviconLink
     */
    QUrl faviconLink() const;

    /*!
     * \brief Sets the URL of the feed.
     * \sa url
     */
    void setUrl(const QUrl &nUrl);
    /*!
     * \brief Sets the title of the feed.
     * \sa title
     */
    void setTitle(const QString &nTitle);
    /*!
     * \brief Sets the date and time the feed has been added.
     * \sa added
     */
    void setAdded(const QDateTime &nAdded);
    /*!
     * \brief Sets the ID of the Folder the feed belongs to.
     * \sa folderId
     */
    void setFolderId(qint64 nFolderId);
    /*!
     * \brief Sets the name of the Folder the feed belongs to.
     * \sa folderName
     */
    void setFolderName(const QString &nFolderName);
    /*!
     * \brief Sets the count of unread items in this feed.
     * \sa unreadCount
     */
    void setUnreadCount(uint nUnreadCount);
    /*!
     * \brief Sets the ordering of the items in this feed on the server.
     * \sa ordering
     */
    void setOrdering(Fuoten::Feed::FeedOrdering nOrdering);
    /*!
     * \brief Sets the URL to the feed's homepage.
     * \sa link
     */
    void setLink(const QUrl &nLink);
    /*!
     * \brief Set to true if this feed is pinned.
     * \sa pinned
     */
    void setPinned(bool nPinned);
    /*!
     * \brief Sets the count of update errors.
     * \sa updateErrorCount
     */
    void setUpdateErrorCount(uint nUpdateErrorCount);
    /*!
     * \brief Sets the message of the last update error.
     * \sa lastUpdateError
     */
    void setLastUpdateError(const QString &nLastUpdateError);
    /*!
     * \brief Sets the URL of the feed's favicon.
     * \sa faviconLink
     */
    void setFaviconLink(const QUrl &nFaviconLink);

    /*!
     * \brief Makes a deep copy of \a other.
     *
     * \a other has to be a Feed object.
     */
    void copy(BaseItem *other) override;


    /*!
     * \brief Sets a new name for the feed on the remote server.
     *
     * Will also rename the feed in the local \a storage if a valid AbstractStorage object has been set.
     *
     * \param newName the new name for the feed
     * \param config pointer to an AbstractConfiguration subclass to get the account configuration
     * \param storage pointer to an AbstractStorage subclass to rename the feed in the local storage
     */
    Q_INVOKABLE void rename(const QString &newName, Fuoten::AbstractConfiguration *config, Fuoten::AbstractStorage *storage = nullptr);


    /*!
     * \brief Removes this feed from the remote server.
     *
     * Will also remove the feed from the local \a storage if a valid AbstractStorage object has been set.
     *
     * \param config    pointer to an AbstractConfiguration subclass to get the account configuration
     * \param storage   pointer to an AbstractStorage subclass to remove the feed from the local storage
     */
    Q_INVOKABLE void remove(Fuoten::AbstractConfiguration *config, Fuoten::AbstractStorage *storage = nullptr);


    /*!
     * \brief Moves this feed to a different folder on the remote server.
     *
     * Will also move the feed in the local, \a storage if a valid AbstractStorage object has been set.
     *
     * \param targetFolderId    ID of the folder the feed should be moved to
     * \param config            pointer to an AbstractConfiguration subclass to get the account configuration
     * \param storage           pointer to an AbstractStorage subclass to move the feed in the local storage
     */
    Q_INVOKABLE void move(qint64 targetFolderId, Fuoten::AbstractConfiguration *config, Fuoten::AbstractStorage *storage = nullptr);


    /*!
     * \brief Marks the complete feed as read on the remote server and local.
     *
     * \param config    pointer to an AbstractConfiguration subclass to get the account configuration
     * \param storage   pointer to an AbstractStorage subclass to query the newest item ID in the feed and update the local storage
     */
    Q_INVOKABLE void markAsRead(Fuoten::AbstractConfiguration * config, Fuoten::AbstractStorage *storage);

Q_SIGNALS:
    /*!
     * \brief This signal is emitted if the feed's URL changes.
     * \sa url
     */
    void urlChanged(const QUrl &url);
    /*!
     * \brief This signal is emitted if the feed's title changes.
     * \sa title
     */
    void titleChanged(const QString &title);
    /*!
     * \brief This signal is emitted if the date and time changes the feed was added.
     * \sa added
     */
    void addedChanged(const QDateTime &added);
    /*!
     * \brief This signal is emitted if the id of the Folder changes the feed belongs to.
     * \sa folderId
     */
    void folderIdChanged(qint64 folderId);
    /*!
     * \brief This signal is emitted if the name of the Folder changes the feed belongs to.
     * \sa folderName
     */
    void folderNameChanged(const QString &folderName);
    /*!
     * \brief This signal is emitted if the count of unread items/articles in this feed changes.
     * \sa unreadCount
     */
    void unreadCountChanged(uint unreadCount);
    /*!
     * \brief This signal is emitted if the server side ordering of this feed changes.
     * \sa ordering
     */
    void orderingChanged(FeedOrdering ordering);
    /*!
     * \brief This signal is emitted if the URL of the homepage of this feed changes.
     * \sa link
     */
    void linkChanged(const QUrl &link);
    /*!
     * \brief This signal is emitted if the pinned status of the feed changes.
     * \sa pinned
     */
    void pinnedChanged(bool pinned);
    /*!
     * \brief This signal is emitted if the count of update errors of this feed changes.
     * \sa updateErrorCount
     */
    void updateErrorCountChanged(uint updateErrorCount);
    /*!
     * \brief This signal is emitted if the message of the last update error changes.
     * \sa lastUpdateError
     */
    void lastUpdateErrorChanged(const QString &lastUpdateError);
    /*!
     * \brief This signal is emitted if the URL to the favicon of this feed changes.
     * \sa favicon
     */
    void faviconLinkChanged(const QUrl &faviconLink);

protected:
    Feed(FeedPrivate &dd, QObject *parent = nullptr);

private:
    Q_DISABLE_COPY(Feed)
    Q_DECLARE_PRIVATE(Feed)

};

}

#if QT_VERSION < QT_VERSION_CHECK(5, 5, 0)
Q_DECLARE_METATYPE(Fuoten::Feed::FeedOrdering)
#endif


#endif // FUOTENFEED_H
