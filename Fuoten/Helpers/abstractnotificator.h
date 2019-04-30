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

#ifndef FUOTENABSTRACTNOTIFICATOR_H
#define FUOTENABSTRACTNOTIFICATOR_H

#include <QObject>
#include "../fuoten_global.h"

class QJsonObject;
class QVariant;
class QString;

namespace Fuoten {

class AbstractNotificatorPrivate;
class Article;
class Error;

/*!
 * \brief Abstract class to implement platform specific notifications about errors and successful operations.
 *
 * Reimplement this class if you want to have notifications specific for your target platform. All API class
 * can notify about occured errors - but not about success. Sucess notifications should be done by the a storage
 * class derived from AbstractStorage, or separate from the API classes after processing the API results.
 *
 * \par Setting a notificator
 *
 * You can set a notificator via different methods. For all API classes there is the Component::notificator property,
 * the same is true for storage classes via the AbstractStorage::notificator property and for the synchronizer via
 * Synchronizer::notificator property. Beside these instance based properties there is also Component::setDefaultNotificator()
 * to set a global default notificator that is used if no notificator is set for the current object. If no notificator
 * has been set, there will be no notifications but programm will run normal. So notifications are totally optional.
 *
 * \par Creating a new notificator
 *
 * Notifications are platform dependent, so you should create notificators for your target platform by creating a
 * new class derived from AbstractNotificator. The main method for notifications you have to reimplement in your class
 * is notify(Type type, QtMsgType severity, const QVariant &data). This method handles all the default notifications
 * about occured errors and success notifications.
 *
 * When reimplementing the default notification method, you can use appIcon() and appName() to populate default fields,
 * optionally you can use isEnabled() to enable or disable notifications globally. You could for example disable notifications
 * if your application is currently active and visible to the user and there are places in the application that show the same
 * information. But it is on you how you implement it. You can also simply rely on the notification \a type and \a severity
 * to decide when and how to show notifications.
 *
 * Only the error notification \link AbstractNotificator::Type types \endlink will provide ready to display error strings
 * in the \a data argument. All other \link AbstractNotificator::Type types \endlink will have meta data in the \a data
 * argument that can be used to create custom notification messages. See the \link AbstractNotificator::Type type \endlink
 * enum description to learn what data you can expect for the different types.
 *
 * The \a severity argument is an additional hint for displaying the notifications. Most error \link AbstractNotificator::Type types \endlink
 * will have QtCriticalMsg as their \a severity, only a few will have QtFatalMsg, some other might have QtWarningMsg. Informational
 * notifications - like success notifications - will have a \a severity of QtInfoMsg.
 *
 * \headerfile "" <Fuoten/Helpers/AbstractNotificator>
 */
class FUOTENSHARED_EXPORT AbstractNotificator : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(AbstractNotificator)
    Q_DECLARE_PRIVATE(AbstractNotificator)
public:
    /*!
     * \brief Constructs a new AbstractNotifier object with the given \a parent.
     */
    explicit AbstractNotificator(QObject *parent = nullptr);

    /*!
     * \brief Destroys the AbstractNotifier object.
     */
    ~AbstractNotificator();

    /*!
     * \brief Returns the path to the currently set application icon.
     * The default is an empty string.
     * \sa setAppIcon(), appIconChanged()
     */
    virtual QString appIcon() const;
    /*!
     * \brief Sets the path to an application icon.
     * The default is an empty string.
     * \sa appIcon(), appIconChanged()
     */
    void setAppIcon(const QString &appIcon);

    /*!
     * \brief Returns the currently set application name.
     * The default is an empty string.
     * \sa setAppName(), appNameChanged()
     */
    virtual QString appName() const;
    /*!
     * \brief Sets the application name.
     * The default is an empty string.
     * \sa appName(), appNameChanged()
     */
    void setAppName(const QString &appName);

    /*!
     * \brief Returns \c true if notifications are enabled.
     * The default is \c false.
     * \sa setEnabled(), enabledChanged()
     */
    virtual bool isEnabled() const;
    /*!
     * \brief Set to \c true to enable notifications
     * The default is \c false.
     * \sa isEnabled(), enabledChanged()
     */
    void setEnabled(bool enabled);

    /*!
     * \brief Returns \c true if article publishing is enabled.
     * The default is \c false.
     * \sa setArticlePublishingEnabled(), articlePublishingChanged()
     */
    virtual bool isArticlePublishingEnabled() const;
    /*!
     * \brief Set to \c true to enable article publishing.
     * The default is \c false.
     * \sa isArticlePublishingEnabled(), articlePublishingChanged()
     */
    void setArticlePublishingEnabled(bool enabled);

    /*!
     * \brief The type of the notification.
     */
    enum Type {
        GeneralError = 0,   /**< A general error, normally not used, only there as fallback. The \a data will contain a null QVariant. */
        RequestError,       /**< The request was not setup correctly or failed. The \a data will contain an error string. */
        ParsingError,       /**< There was an error when parsing the JSON response. The \a data will contain an error string. */
        InputError,         /**< An error occurred while providing data to the library methods. The \a data will conatin an error string. */
        OutputError,        /**< An error occurred while processing the returned data from the API. The \a data will contain an error string. */
        ServerError,        /**< An error occurred on the server. The \a data will contain an error string. */
        ApplicationError,   /**< An error occurred in the local application. The \a data will contain an error string. */
        StorageError,       /**< An error occurred on the storage layer. The \a data will contain an error string. */
        AuthorizationError, /**< Authentication credentials missing or not valid. The \a data will contain an error string. */
        SyncComplete,       /**< Synchronization with the server was successful performed. The \a data will contain the number of seconds needed to perform the sync as an integer. */
        FoldersRequested,   /**< Folders have been requested from the server. The \a data will contain a QVariantList of QStringList that contains the following data: 0: names of new folders, 1: names of updated folders, 2: names of deleted folders */
        FolderCreated,      /**< A new folder has succesfully been created on the server. The \a data will contain the name of the folder as a QString. */
        FolderDeleted,      /**< A Folder has been successfully deleted on the server. The \a data will contain the name of the deleted folder as a QString. */
        FolderRenamed,      /**< A folder has successfully been renamed on the server. The \a data will contain a QStringList with two entries. 0: older folder name, 1: new folder name */
        FolderMarkedRead,   /**< A folder has successfully been marked as read. The \a data will contain the name of the folder as a QString. */
        FeedsRequested,     /**< Feeds have been requested from the server. The \a data will contain a QVariantList of QStringList that conatins the following data: 0: titles of new feeds, 1: titles of updated feeds, 2: titles of deleted feeds */
        FeedCreated,        /**< A new feed has successfully been created on the server. The \a data will contain the title of the new feed as a QString. */
        FeedDeleted,        /**< A feed has successfully been deleted on the server. The \a data will contain the title of the deleted feed ad a QString. */
        FeedMoved,          /**< A feed has successfully been moved on the server. The \a data will contain a QVariantList containing the following information as QString: 0: title of the feed, 1: name of the old folder, 2: name of the new folder */
        FeedRenamed,        /**< A feed has successfully been renamed on the server. The \a data will contain a QVariantList containing the following information as QString: 0: old title, 1: new title */
        FeedMarkedRead,     /**< A feed has been marked as read. The \a data will contain the title of the feed as a QString. */
        ItemsRequested      /**< Items have been requested from the server. The \a data will contain the amount of new unread items as an integer value. */
    };

    /*!
     * \brief Creates a new notifications.
     *
     * Reimplement this in your derived class to create platform specific notifications.
     *
     * The \a type will define the message type, have a look at the enum description to learn more about it -
     * it will also provide information about the provided \a data. The \a severity can be used to further
     * decide when and how to display the notification.
     */
    virtual void notify(Type type, QtMsgType severity, const QVariant &data) const = 0;

    /*!
     * \brief Creates a new notification from an \a error object.
     *
     * \overload
     *
     * The default implementation converts the \a error information like type, severity and error text
     * into data that will be used by the main notify method. So to use the default implementation, you
     * have to reimplement the main notify method.
     */
    virtual void notify(const Error *error) const;

    /*!
     * \brief Publishes information about an \a article to the notification system.
     *
     * Some platforms - like Meego 1.2 Harmattan - provide global notification streams for different services
     * and news. You can reimplement this method to publish received articles to this streams.
     *
     * The default implementation does nothing.
     */
    virtual void publishArticle(const QJsonObject &article, const QString &feedName = QString()) const;

    /*!
     * \brief Publishes information about an \a article to the notification system.
     *
     * Some platforms - like Meego 1.2 Harmattan - provide global notification streams for different services
     * and news. You can reimplement this method to publish received articles to this streams.
     *
     * The default implementation does nothing.
     */
    virtual void publishArticle(const Article* article) const;

    /*!
     * \brief Returns \c true if the \a article should be published to notifications.
     *
     * You can reimplement this to decide if an article should be published to the notifications system
     * via publishArticle(). The default implementation returns always \c false. You have to reimplement
     * your own checks, for example based on the \a feedId the article belongs to or on some keywords
     * in the article title. Best place to use is in a class derived from AbstractStorage, when you iterate
     * over new articles. In the SQLiteStorage class it is for example used in the worker that processes
     * requested items from the API.
     */
    virtual bool checkForPublishing(const QJsonObject &article) const;

    /*!
     * \brief Returns \c true if the \a article should be published to notifications.
     * \overload
     */
    virtual bool checkForPublishing(const Article* article) const;

Q_SIGNALS:
    /*!
     * \brief This signal is emitted if the path to the application icon changes.
     * \sa setAppIcon(), appIcon()
     */
    void appIconChanged(const QString &appIcon);
    /*!
     * \brief This signal is emitted if the application name changes.
     * \sa setAppName(), appName()
     */
    void appNameChanged(const QString &appName);
    /*!
     * \brief This signal is emitted if the enabled status changes.
     * \sa setEnabled(), isEnabled()
     */
    void enabledChanged(bool enabled);
    /*!
     * \brief This signal is emitted if the article publishing changes.
     * \sa setArticlePublishingEnabeled(), isArticlePublishingEnabled()
     */
    void articlePublishingChanged(bool enabled);

protected:
    const QScopedPointer<AbstractNotificatorPrivate> d_ptr;
};

}

#endif // FUOTENABSTRACTNOTIFICATOR_H
