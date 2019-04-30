/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016-2019 Hüssenbergnetz/Matthias Fehring
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

#ifndef FUOTENARTICLE_H
#define FUOTENARTICLE_H

#include <QObject>
#include <QUrl>
#include <QDateTime>
#include "baseitem.h"
#include "fuoten_global.h"
#include "fuoten.h"

namespace Fuoten {

class ArticlePrivate;

/*!
 * \brief Contains information about a single article/item.
 *
 * \headerfile "" <Fuoten/Article>
 */
class FUOTENSHARED_EXPORT Article : public BaseItem
{
    Q_OBJECT
    /*!
     * \brief ID of the Feed this Article belongs to.
     *
     * \par Access functions:
     * <TABLE><TR><TD>qint64</TD><TD>feedId() const</TD></TR><TR><TD>void</TD><TD>setFeedId(qint64 nFeedId)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>feedIdChanged(qint64 feedId)</TD></TR></TABLE>
     */
    Q_PROPERTY(qint64 feedId READ feedId WRITE setFeedId NOTIFY feedIdChanged)
    /*!
     * \brief Title of the feed this article belongs to.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>feedTitle() const</TD></TR><TR><TD>void</TD><TD>setFeedTitle(const QString &nFeedTitle)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>feedTitleChanged(const QString &feedTitle)</TD></TR></TABLE>
     */
    Q_PROPERTY(QString feedTitle READ feedTitle WRITE setFeedTitle NOTIFY feedTitleChanged)
    /*!
     * \brief Global unique identifier of the article.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>guid() const</TD></TR><TR><TD>void</TD><TD>setGuid(const QString &nGuid)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>guidChanged(const QString &guid)</TD></TR></TABLE>
     */
    Q_PROPERTY(QString guid READ guid WRITE setGuid NOTIFY guidChanged)
    /*!
     * \brief Hash of the global uniquie identifier of the article.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>guidHash() const</TD></TR><TR><TD>void</TD><TD>setGuidHash(const QString &nGuidHash)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>guidHashChanged(const QString &guidHash)</TD></TR></TABLE>
     */
    Q_PROPERTY(QString guidHash READ guidHash WRITE setGuidHash NOTIFY guidHashChanged)
    /*!
     * \brief URL of the article.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QUrl</TD><TD>url() const</TD></TR><TR><TD>void</TD><TD>setUrl(const QUrl &nUrl)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>urlChanged(const QUrl &url)</TD></TR></TABLE>
     */
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    /*!
     * \brief Title of the article.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>title() const</TD></TR><TR><TD>void</TD><TD>setTitle(const QString &nTitle)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>titleChanged(const QString &title)</TD></TR></TABLE>
     */
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    /*!
     * \brief The author of the article.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>author() const</TD></TR><TR><TD>void</TD><TD>setAuthor(const QString &nAuthor)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>authorChanged(const QString &author)</TD></TR></TABLE>
     */
    Q_PROPERTY(QString author READ author WRITE setAuthor NOTIFY authorChanged)
    /*!
     * \brief Date and time the article was published.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QDateTime</TD><TD>pubDate() const</TD></TR><TR><TD>void</TD><TD>setPubDate(const QDateTime &nPubDate)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>pubDateChanged(const QDateTime &pubDate)</TD></TR></TABLE>
     */
    Q_PROPERTY(QDateTime pubDate READ pubDate WRITE setPubDate NOTIFY pubDateChanged)
    /*!
     * \brief The text body of the article.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>body() const</TD></TR><TR><TD>void</TD><TD>setBody(const QString &nBody)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>bodyChanged(const QString &body)</TD></TR></TABLE>
     */
    Q_PROPERTY(QString body READ body WRITE setBody NOTIFY bodyChanged)
    /*!
     * \brief MIME type of an optional enclosure.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>enclosureMime() const</TD></TR><TR><TD>void</TD><TD>setEnclosureMime(const QString &nEnclosureMime)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>enclosureMimeChanged(const QString &enclosureMime)</TD></TR></TABLE>
     */
    Q_PROPERTY(QString enclosureMime READ enclosureMime WRITE setEnclosureMime NOTIFY enclosureMimeChanged)
    /*!
     * \brief URL of the enclosure.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QUrl</TD><TD>enclosureLink() const</TD></TR><TR><TD>void</TD><TD>setEnclosureLink(const QUrl &nEnclosureLink)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>enclosureLinkChanged(const QUrl &enclosureLink)</TD></TR></TABLE>
     */
    Q_PROPERTY(QUrl enclosureLink READ enclosureLink WRITE setEnclosureLink NOTIFY enclosureLinkChanged)
    /*!
     * \brief True if the article has not been read.
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>unread() const</TD></TR><TR><TD>void</TD><TD>setUnread(bool nUnread)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>unreadChanged(bool unread)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool unread READ unread WRITE setUnread NOTIFY unreadChanged)
    /*!
     * \brief True if the user has starred this article.
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>starred() const</TD></TR><TR><TD>void</TD><TD>setStarred(bool nStarred)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>starredChanged(bool starred)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool starred READ starred WRITE setStarred NOTIFY starredChanged)
    /*!
     * \brief Date and time this articles has been modified the last time.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QDateTime</TD><TD>lastModified() const</TD></TR><TR><TD>void</TD><TD>setLastModified(const QDateTime &nLastModified)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>lastModifiedChanged(const QDateTime &lastModified)</TD></TR></TABLE>
     */
    Q_PROPERTY(QDateTime lastModified READ lastModified WRITE setLastModified NOTIFY lastModifiedChanged)
    /*!
     * \brief Hash over title, enclosure, body and url.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>fingerprint() const</TD></TR><TR><TD>void</TD><TD>setFingerprint(const QString &nFingerprint)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>fingerprintChanged(const QString &fingerprint)</TD></TR></TABLE>
     */
    Q_PROPERTY(QString fingerprint READ fingerprint WRITE setFingerprint NOTIFY fingerprintChanged)
    /*!
     * \brief ID of the folder this article belongs to.
     *
     * \par Access functions:
     * <TABLE><TR><TD>qint64</TD><TD>folderId() const</TD></TR><TR><TD>void</TD><TD>setFolderId(qint64 nFolderId)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>folderIdChanged(qint64 folderId)</TD></TR></TABLE>
     */
    Q_PROPERTY(qint64 folderId READ folderId WRITE setFolderId NOTIFY folderIdChanged)
    /*!
     * \brief Name of the folder this article belongs to.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>folderName() const</TD></TR><TR><TD>void</TD><TD>setFolderName(const QString &nFolderName)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>folderNameChanged(const QString &folderName)</TD></TR></TABLE>
     */
    Q_PROPERTY(QString folderName READ folderName WRITE setFolderName NOTIFY folderNameChanged)
    /*!
     * \brief Returns a human readable string of the publication date.
     *
     * Will be automatically created from pubDate.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>getHumanPubDate() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>humanPubDateChanged(const QString &humanPubDate)</TD></TR></TABLE>
     */
    Q_PROPERTY(QString humanPubDate READ humanPubDate NOTIFY humanPubDateChanged)
    /*!
     * \brief Return a human readable string of the publication time.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>humanPubTime() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>humanPubTimeChanged(const QString &humanPubTime)</TD></TR></TABLE>
     */
    Q_PROPERTY(QString humanPubTime READ humanPubTime NOTIFY humanPubTimeChanged)

public:
    /*!
     * \brief Constructs a new Article object with default values and the given \a parent.
     */
    explicit Article(QObject *parent = nullptr);

    /*!
     * \brief Constructs a new Article object with the given arguments and \a parent.
     */
    Article(qint64 id, qint64 feedId, const QString &feedTitle, const QString &guid, const QString &guidHash, const QUrl &url, const QString &title, const QString &author, const QDateTime &pubDate, const QString &body, const QString &enclosureMime, const QUrl &enclosureLink, bool unread, bool starred, const QDateTime &lastModified, const QString &fingerprint, qint64 folderId, const QString &folderName, FuotenEnums::QueueActions queue, QObject *parent = nullptr);

    /*!
     * \brief Constructs a new Article object with the given \a parent by copying the properties of \a other to the new object.
     */
    explicit Article(Article *other, QObject *parent = nullptr);


    /*!
     * \brief Getter function for the \link Article::feedId feedId \endlink property.
     * \sa Article::setFeedId(), Article::feedIdChanged()
     */
    qint64 feedId() const;
    /*!
     * \brief Getter function for the \link Article::feedTitle feedTitle \endlink property.
     * \sa Article::setFeedTitle(), Article::feedTitleChanged()
     */
    QString feedTitle() const;
    /*!
     * \brief Getter function for the \link Article::guid guid \endlink property.
     * \sa Article::setGuid(), Article::guidChanged()
     */
    QString guid() const;
    /*!
     * \brief Getter function for the \link Article::guidHash guidHash \endlink property.
     * \sa Article::setGuidHash(), Article::guidHashChanged()
     */
    QString guidHash() const;
    /*!
     * \brief Getter function for the \link Article::url url \endlink property.
     * \sa Article::setUrl(), Article::urlChanged()
     */
    QUrl url() const;
    /*!
     * \brief Getter function for the \link Article::title title \endlink property.
     * \sa Article::setTitle(), Article::titleChanged()
     */
    QString title() const;
    /*!
     * \brief Getter function for the \link Article::author author \endlink property.
     * \sa Article::setAuthor(), Article::authorChanged()
     */
    QString author() const;
    /*!
     * \brief Getter function for the \link Article::pubDate pubDate \endlink property.
     * \sa Article::setPubDate(), Article::pubDateChanged()
     */
    QDateTime pubDate() const;
    /*!
     * \brief Getter function for the \link Article::body body \endlink property.
     * \sa Article::setBody(), Article::bodyChanged()
     */
    QString body() const;
    /*!
     * \brief Getter function for the \link Article::enclosureMime enclosureMime \endlink property.
     * \sa Article::setEnclosureMime(), Article::enclosureMimeChanged()
     */
    QString enclosureMime() const;
    /*!
     * \brief Getter function for the \link Article::enclosureLink enclosureLink \endlink property.
     * \sa Article::setEnclosureLink(), Article::enclosureLinkChanged()
     */
    QUrl enclosureLink() const;
    /*!
     * \brief Getter function for the \link Article::unread unread \endlink property.
     * \sa Article::setUnread(), Article::unreadChanged()
     */
    bool unread() const;
    /*!
     * \brief Getter function for the \link Article::starred starred \endlink property.
     * \sa Article::setStarred(), Article::starredChanged()
     */
    bool starred() const;
    /*!
     * \brief Getter function for the \link Article::lastModified lastModified \endlink property.
     * \sa Article::setLastModified(), Article::lastModifiedChanged()
     */
    QDateTime lastModified() const;
    /*!
     * \brief Getter function for the \link Article::fingerprint fingerprint \endlink property.
     * \sa Article::setFingerprint(), Article::fingerprintChanged()
     */
    QString fingerprint() const;
    /*!
     * \brief Getter function for the \link Article::folderId folderId \endlink property.
     * \sa Article::setFolderId(), Article::folderIdChanged()
     */
    qint64 folderId() const;
    /*!
     * \brief Getter function for the \link Article::folderName folderName \endlink property.
     * \sa Article::setFolderName(), Article::folderNameChanged()
     */
    QString folderName() const;
    /*!
     * \brief Getter function for the \link Article::humanPubDate humanPubDate \endlink property.
     * \sa Article::humanPubDateChanged()
     */
    QString humanPubDate() const;
    /*!
     * \brief Getter function for the \link Article::humanPubTime humanPubTime \endlink property.
     * \sa Article::humanPubTimeChanged()
     */
    QString humanPubTime() const;

    FuotenEnums::QueueActions queue() const;


    /*!
     * \brief Setter function for the \link Article::feedId feedId \endlink property.
     * Emits the feedIdChanged() signal if \a nFeedId is not equal to the stored value.
     * \sa Article::feedId(), Article::feedIdChanged()
     */
    void setFeedId(qint64 nFeedId);
    /*!
     * \brief Setter function for the \link Article::feedTitle feedTitle \endlink property.
     * Emits the feedTitleChanged() signal if \a nFeedTitle is not equal to the stored value.
     * \sa Article::feedTitle(), Article::feedTitleChanged()
     */
    void setFeedTitle(const QString &nFeedTitle);
    /*!
     * \brief Setter function for the \link Article::guid guid \endlink property.
     * Emits the guidChanged() signal if \a nGuid is not equal to the stored value.
     * \sa Article::guid(), Article::guidChanged()
     */
    void setGuid(const QString &nGuid);
    /*!
     * \brief Setter function for the \link Article::guidHash guidHash \endlink property.
     * Emits the guidHashChanged() signal if \a nGuidHash is not equal to the stored value.
     * \sa Article::guidHash(), Article::guidHashChanged()
     */
    void setGuidHash(const QString &nGuidHash);
    /*!
     * \brief Setter function for the \link Article::url url \endlink property.
     * Emits the urlChanged() signal if \a nUrl is not equal to the stored value.
     * \sa Article::url(), Article::urlChanged()
     */
    void setUrl(const QUrl &nUrl);
    /*!
     * \brief Setter function for the \link Article::title title \endlink property.
     * Emits the titleChanged() signal if \a nTitle is not equal to the stored value.
     * \sa Article::title(), Article::titleChanged()
     */
    void setTitle(const QString &nTitle);
    /*!
     * \brief Setter function for the \link Article::author author \endlink property.
     * Emits the authorChanged() signal if \a nAuthor is not equal to the stored value.
     * \sa Article::author(), Article::authorChanged()
     */
    void setAuthor(const QString &nAuthor);
    /*!
     * \brief Setter function for the \link Article::pubDate pubDate \endlink property.
     * Emits the pubDateChanged() signal if \a nPubDate is not equal to the stored value.
     * \sa Article::pubDate(), Article::pubDateChanged()
     */
    void setPubDate(const QDateTime &nPubDate);
    /*!
     * \brief Setter function for the \link Article::body body \endlink property.
     * Emits the bodyChanged() signal if \a nBody is not equal to the stored value.
     * \sa Article::body(), Article::bodyChanged()
     */
    void setBody(const QString &nBody);
    /*!
     * \brief Setter function for the \link Article::enclosureMime enclosureMime \endlink property.
     * Emits the enclosureMimeChanged() signal if \a nEnclosureMime is not equal to the stored value.
     * \sa Article::enclosureMime(), Article::enclosureMimeChanged()
     */
    void setEnclosureMime(const QString &nEnclosureMime);
    /*!
     * \brief Setter function for the \link Article::enclosureLink enclosureLink \endlink property.
     * Emits the enclosureLinkChanged() signal if \a nEnclosureLink is not equal to the stored value.
     * \sa Article::enclosureLink(), Article::enclosureLinkChanged()
     */
    void setEnclosureLink(const QUrl &nEnclosureLink);
    /*!
     * \brief Setter function for the \link Article::unread unread \endlink property.
     * Emits the unreadChanged() signal if \a nUnread is not equal to the stored value.
     * \sa Article::unread(), Article::unreadChanged()
     */
    void setUnread(bool nUnread);
    /*!
     * \brief Setter function for the \link Article::starred starred \endlink property.
     * Emits the starredChanged() signal if \a nStarred is not equal to the stored value.
     * \sa Article::starred(), Article::starredChanged()
     */
    void setStarred(bool nStarred);
    /*!
     * \brief Setter function for the \link Article::lastModified lastModified \endlink property.
     * Emits the lastModifiedChanged() signal if \a nLastModified is not equal to the stored value.
     * \sa Article::lastModified(), Article::lastModifiedChanged()
     */
    void setLastModified(const QDateTime &nLastModified);
    /*!
     * \brief Setter function for the \link Article::fingerprint fingerprint \endlink property.
     * Emits the fingerprintChanged() signal if \a nFingerprint is not equal to the stored value.
     * \sa Article::fingerprint(), Article::fingerprintChanged()
     */
    void setFingerprint(const QString &nFingerprint);
    /*!
     * \brief Setter function for the \link Article::folderId folderId \endlink property.
     * Emits the folderIdChanged() signal if \a nFolderId is not equal to the stored value.
     * \sa Article::folderId(), Article::folderIdChanged()
     */
    void setFolderId(qint64 nFolderId);
    /*!
     * \brief Setter function for the \link Article::folderName folderName \endlink property.
     * Emits the folderNameChanged() signal if \a nFolderName is not equal to the stored value.
     * \sa Article::folderName(), Article::folderNameChanged()
     */
    void setFolderName(const QString &nFolderName);

    void setQueue(FuotenEnums::QueueActions queue);

    /*!
     * \brief Makes a deep copy of \a other.
     *
     * \a other has to be an Article object.
     */
    void copy(BaseItem *other) override;

    /*!
     * \brief Marks this article as \a read or \a unread on the server.
     *
     * Will also mark the article as read or unread in the local \a storage, if a valid AbstractStorage object is available.
     *
     * \param unread    set to \c true to mark the item as unread, set to \c false to mark it as read
     * \param config    pointer to an AbstractConfiguration object that containts the authentication credentials
     * \param storage   pointer to an AbstractStorage object to update the local storage after successful request
     * \param enqueue   \c true to enqueue the marking local up to the next sync, valid \c storage has to be available
     */
    Q_INVOKABLE void mark(bool unread, Fuoten::AbstractConfiguration *config, Fuoten::AbstractStorage *storage = nullptr, bool enqueue = false);

    /*!
     * \brief Marks this article as \a read or \a unread on the server.
     * \overload
     *
     * This uses Component::defaultConfiguration() and Component::defaultStorage().
     */
    Q_INVOKABLE void mark(bool unread, bool enqueue = false);


    /*!
     * \brief Marks this article as \a starred or \a unstarred on the remote server.
     *
     * Will also mark the article as starred/unstarred in the local \a storage if a valid AbstractStorage object is available.
     *
     * \param starred   ste to \c true to mark the item as starred, set to \c false to mark it as unstarred
     * \param config    pointer to an AbstractConfiguration object that containts the authentication credentials
     * \param storage   pointer to an AbstractStorage object to update the local storage after successful request
     * \param enqueue   \c true to enqueue the un/starring local up to the next sync, valid \c storage has to be available
     */
    Q_INVOKABLE void star(bool starred, Fuoten::AbstractConfiguration *config, Fuoten::AbstractStorage *storage = nullptr, bool enqueue = false);

    /*!
     * \brief Marks this article as \a starred or \a unstarred on the remote server.
     * \overload
     *
     * This uses Component::defaultConfiguration() and Component::defaultStorage().
     */
    Q_INVOKABLE void star(bool starred, bool enqueue = false);


Q_SIGNALS:
    /*!
     * \brief This is emitted if the value of the \link Article::feedId feedId \endlink property changes.
     * \sa Article::feedId(), Article::setFeedId()
     */
    void feedIdChanged(qint64 feedId);
    /*!
     * \brief This is emitted if the value of the \link Article::feedTitle feedTitle \endlink property changes.
     * \sa Article::feedTitle(), Article::setFeedTitle()
     */
    void feedTitleChanged(const QString &feedTitle);
    /*!
     * \brief This is emitted if the value of the \link Article::guid guid \endlink property changes.
     * \sa Article::guid(), Article::setGuid()
     */
    void guidChanged(const QString &guid);
    /*!
     * \brief This is emitted if the value of the \link Article::guidHash guidHash \endlink property changes.
     * \sa Article::guidHash(), Article::setGuidHash()
     */
    void guidHashChanged(const QString &guidHash);
    /*!
     * \brief This is emitted if the value of the \link Article::url url \endlink property changes.
     * \sa Article::url(), Article::setUrl()
     */
    void urlChanged(const QUrl &url);
    /*!
     * \brief This is emitted if the value of the \link Article::title title \endlink property changes.
     * \sa Article::title(), Article::setTitle()
     */
    void titleChanged(const QString &title);
    /*!
     * \brief This is emitted if the value of the \link Article::author author \endlink property changes.
     * \sa Article::author(), Article::setAuthor()
     */
    void authorChanged(const QString &author);
    /*!
     * \brief This is emitted if the value of the \link Article::pubDate pubDate \endlink property changes.
     * \sa Article::pubDate(), Article::setPubDate()
     */
    void pubDateChanged(const QDateTime &pubDate);
    /*!
     * \brief This is emitted if the value of the \link Article::body body \endlink property changes.
     * \sa Article::body(), Article::setBody()
     */
    void bodyChanged(const QString &body);
    /*!
     * \brief This is emitted if the value of the \link Article::enclosureMime enclosureMime \endlink property changes.
     * \sa Article::enclosureMime(), Article::setEnclosureMime()
     */
    void enclosureMimeChanged(const QString &enclosureMime);
    /*!
     * \brief This is emitted if the value of the \link Article::enclosureLink enclosureLink \endlink property changes.
     * \sa Article::enclosureLink(), Article::setEnclosureLink()
     */
    void enclosureLinkChanged(const QUrl &enclosureLink);
    /*!
     * \brief This is emitted if the value of the \link Article::unread unread \endlink property changes.
     * \sa Article::unread(), Article::setUnread()
     */
    void unreadChanged(bool unread);
    /*!
     * \brief This is emitted if the value of the \link Article::starred starred \endlink property changes.
     * \sa Article::starred(), Article::setStarred()
     */
    void starredChanged(bool starred);
    /*!
     * \brief This is emitted if the value of the \link Article::lastModified lastModified \endlink property changes.
     * \sa Article::lastModified(), Article::setLastModified()
     */
    void lastModifiedChanged(const QDateTime &lastModified);
    /*!
     * \brief This is emitted if the value of the \link Article::fingerprint fingerprint \endlink property changes.
     * \sa Article::fingerprint(), Article::setFingerprint()
     */
    void fingerprintChanged(const QString &fingerprint);
    /*!
     * \brief This is emitted if the value of the \link Article::folderId folderId \endlink property changes.
     * \sa Article::folderId(), Article::setFolderId()
     */
    void folderIdChanged(qint64 folderId);
    /*!
     * \brief This is emitted if the value of the \link Article::folderName folderName \endlink property changes.
     * \sa Article::folderName(), Article::setFolderName()
     */
    void folderNameChanged(const QString &folderName);
    /*!
     * \brief This is emitted if the value of the \link Article::humanPubDate humanPubDate \endlink property changes.
     * \sa Article::getHumanPubDate()
     */
    void humanPubDateChanged(const QString &humanPubDate);
    /*!
     * \brief This is emitted if the value of the \link Article::humanPubTime humanPubTime \endlink property changes.
     * \sa Article::humanPubTime()
     */
    void humanPubTimeChanged(const QString &humanPubTime);

protected:
    Article(ArticlePrivate &dd, QObject *parent = nullptr);

private:
    Q_DISABLE_COPY(Article)
    Q_DECLARE_PRIVATE(Article)

};

}


#endif // FUOTENARTICLE_H
