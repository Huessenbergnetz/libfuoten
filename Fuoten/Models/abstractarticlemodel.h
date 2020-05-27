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

#ifndef FUOTENABSTRACTARTICLEMODEL_H
#define FUOTENABSTRACTARTICLEMODEL_H

#include <QObject>
#include "basemodel.h"
#include "../fuoten.h"
#include "../fuoten_global.h"

namespace Fuoten {

class AbstractArticleModelPrivate;
class Article;


/*!
 * \brief Abstract article model that provides basic functionalities for Article models.
 *
 * \headerfile "" <Fuoten/Models/AbstractArticleModel>
 */
class FUOTENSHARED_EXPORT AbstractArticleModel : public BaseModel
{
    Q_OBJECT
    /*!
     * \brief Defines the type of the parentId. Can be Feed, Folder or All.
     *
     * Default: FuotenEnums::All
     *
     * \par Access functions:
     * <TABLE><TR><TD>FuotenEnums::Type</TD><TD>parentIdType() const</TD></TR><TR><TD>void</TD><TD>setParentIdType(FuotenEnums::Type nParentIdType)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>parentIdTypeChanged(FuotenEnums::Type parentIdType)</TD></TR></TABLE>
     */
    Q_PROPERTY(Fuoten::FuotenEnums::Type parentIdType READ parentIdType WRITE setParentIdType NOTIFY parentIdTypeChanged)
    /*!
     * \brief If true, only starred articles will be returned.
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>starredOnly() const</TD></TR><TR><TD>void</TD><TD>setStarredOnly(bool nStarredOnly)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>starredOnlyChanged(bool starredOnly)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool starredOnly READ starredOnly WRITE setStarredOnly NOTIFY starredOnlyChanged)
    /*!
     * \brief Limits the size of the body text in number of characters.
     *
     * Values lower than \c 0 will return no body text, \c 0 will return the full body text, any other positive
     * value will return a body stripped from HTML tags and limited to the amount of characters. Defaults to \c -1.
     *
     * \par Access functions:
     * <TABLE><TR><TD>int</TD><TD>bodyLimit() const</TD></TR><TR><TD>void</TD><TD>setBodyLimit(int nBodyLimit)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>bodyLimitChanged(int bodyLimit)</TD></TR></TABLE>
     */
    Q_PROPERTY(int bodyLimit READ bodyLimit WRITE setBodyLimit NOTIFY bodyLimitChanged)
public:
    /*!
     * \brief Constructs a new empty abstract Article model with the given \a parent.
     */
    explicit AbstractArticleModel(QObject *parent = nullptr);

    /*!
     * \brief Destroys the %AbstractArticleModel object.
     */
    ~AbstractArticleModel() override;

    /*!
     * \brief Getter function for the \link AbstractArticleModel::parentIdType parentIdType \endlink property.
     * \sa AbstractArticleModel::setParentIdType(), AbstractArticleModel::parentIdTypeChanged()
     */
    FuotenEnums::Type parentIdType() const;
    /*!
     * \brief Getter function for the \link AbstractArticleModel::starredOnly starredOnly \endlink property.
     * \sa AbstractArticleModel::setStarredOnly(), AbstractArticleModel::starredOnlyChanged()
     */
    bool starredOnly() const;
    /*!
     * \brief Getter function for the \link AbstractArticleModel::bodyLimit bodyLimit \endlink property.
     * \sa AbstractArticleModel::setBodyLimit(), AbstractArticleModel::bodyLimitChanged()
     */
    int bodyLimit() const;




    /*!
     * \brief Setter function for the \link AbstractArticleModel::parentIdType parentIdType \endlink property.
     * Emits the parentIdTypeChanged() signal if \a nParentIdType is not equal to the stored value.
     * \sa AbstractArticleModel::parentIdType(), AbstractArticleModel::parentIdTypeChanged()
     */
    void setParentIdType(FuotenEnums::Type nParentIdType);
    /*!
     * \brief Setter function for the \link AbstractArticleModel::starredOnly starredOnly \endlink property.
     * Emits the starredOnlyChanged() signal if \a nStarredOnly is not equal to the stored value.
     * \sa AbstractArticleModel::starredOnly(), AbstractArticleModel::starredOnlyChanged()
     */
    void setStarredOnly(bool nStarredOnly);
    /*!
     * \brief Setter function for the \link AbstractArticleModel::bodyLimit bodyLimit \endlink property.
     * Emits the bodyLimitChanged() signal if \a nBodyLimit is not equal to the stored value.
     * \sa AbstractArticleModel::bodyLimit(), AbstractArticleModel::bodyLimitChanged()
     */
    void setBodyLimit(int nBodyLimit);



    /*!
     * \brief Returns the model index of the Article identified by \a id.
     * Returns an invalid index if the ID could not be found in the mode.
     * \param id ID of the Article to find in the model
     */
    QModelIndex findByID(qint64 id) const override;

    /*!
     * \brief Returns IDs and model indices of the Article objects in the model identified by their ID in \a ids.
     *
     * The \a key of the returned hash table will contain the ID of the Article, the \a value will contain the article's model index.
     * Only articles will be returned of that the ID is part of the model.
     *
     * \param ids list of article IDs to find in the model
     */
    QHash<qint64, QModelIndex> findByIDs(const IdList &ids) const override;

public Q_SLOTS:
    /*!
     * \brief Populates the model with data from the local storage.
     *
     * Use AbstractStorage::getArticles() to ge Article objects from the local storage that will be added to the model.
     * If the BaseModel::parentId property is set to a value gerater than \c -1, only Articles will be loaded that are part
     * of the type defined in AbstractArticleMode::parentIdType.
     */
    void load() override;

Q_SIGNALS:
    /*!
     * \brief This is emitted if the value of the \link AbstractArticleModel::parentIdType parentIdType \endlink property changes.
     * \sa AbstractArticleModel::parentIdType(), AbstractArticleModel::setParentIdType()
     */
    void parentIdTypeChanged(FuotenEnums::Type parentIdType);
    /*!
     * \brief This is emitted if the value of the \link AbstractArticleModel::starredOnly starredOnly \endlink property changes.
     * \sa AbstractArticleModel::starredOnly(), AbstractArticleModel::setStarredOnly()
     */
    void starredOnlyChanged(bool starredOnly);
    /*!
     * \brief This is emitted if the value of the \link AbstractArticleModel::bodyLimit bodyLimit \endlink property changes.
     * \sa AbstractArticleModel::bodyLimit(), AbstractArticleModel::setBodyLimit()
     */
    void bodyLimitChanged(int bodyLimit);

protected Q_SLOTS:
    void gotArticlesAsync(const Fuoten::ArticleList &articles);

    /*!
     * \brief Takes and processes data after items/articles have been requested.
     *
     * handleStorageChanged() will connect the AbstractStorage::requestedItems() signal to this slot.
     *
     * \param updatedItems  list of IDs of items that have been updated
     * \param newItems      list of IDs of items that are new to the local storage
     * \param deletedItems  list of IDs of items that have benn updated in the local storage
     */
    void itemsRequested(const Fuoten::IdList &updatedItems, const Fuoten::IdList &newItems, const Fuoten::IdList &deletedItems);

    /*!
     * \brief Takes and processses data after a folder has been marked as read.
     *
     * handleStorageChanged() will connect the AbstractStorage::markedFolderRead() signal to this slot.
     *
     * \param folderId      ID of the folder that has been marked as read
     * \param newestItemId  ID of the newest item that has been marked as read
     */
    void folderMarkedRead(qint64 folderId, qint64 newestItemId);

    /*!
     * \brief Takes and processses data after a folder has been marked as read in the queue.
     *
     * handleStorageChanged() will connect the AbstractStorage::markedReadFolderInQueue() signal to this slots.
     *
     * \param folderId      ID of the folder that has been marked as read
     * \param newestItemId  ID of the newest item that has been marked as read
     */
    void folderMarkedReadInQueue(qint64 folderId, qint64 newestItemId);

    /*!
     * \brief Takes and processes data after a feed has been marked as read.
     *
     * handleStorageChanged() will connect the AbstractStorage::markedReadFeed() signal to this slot.
     *
     * \param feedId        ID of the feed that has been marked as read
     * \param newestItemId  ID of the newest item that has been marked as read
     */
    void feedMarkedRead(qint64 feedId, qint64 newestItemId);

    /*!
     * \brief Takes and processes data after a feeds has been marked as read in local queue.
     *
     * Will do the same as feedMarkedRead() but will also update the queue property of the article objects.
     *
     * handleStorageChanged() will connect the AbstractStorage::markedReadFeedInQueue() signal to this slot.
     *
     * \param feedId        ID of the feed that has been marked as read
     * \param newestItemId  ID of the newest item that has been marked as read
     */
    void feedMarkedReadInQueue(qint64 feedId, qint64 newestItemId);

    /*!
     * \brief Takes and processes data after a folder has been deleted.
     *
     * handleStorageChanged() will connect the AbstractStorage::deletedFolder() signal to this slots.
     *
     * \param folderId ID of the folder that has been deleted.
     */
    void folderDeleted(qint64 folderId);

    /*!
     * \brief Takes and processes data after a feed has been deleted.
     *
     * handleStorageChanged() will connect the AbstractStorage::deletedFeed() signal to this slot.
     *
     * \param feedId ID of the feed that has been deleted.
     */
    void feedDeleted(qint64 feedId);

    /*!
     * \brief Takes and processes data after an item/article has been marked as read/unread.
     *
     * handleStorageChanged() will connect the AbstractStorage::markedItem() signal to this slot.
     *
     * \param itemId    ID of the article that has been marked as read or unread
     * \param unread    \c true if the article has been marked as unread, \c false if it has been marked as read
     */
    void itemMarked(qint64 itemId, bool unread);

    /*!
     * \brief Takes and processes data after a list of items/articles has been marked as read/unread.
     *
     * handleStorageChanged() will connect the AbstractStorage::markedItems() signal to this slot.
     *
     * \param itemIds   list of IDs of items/articles that have been marked as read/unread
     * \param unread    \c true if the items/articles in the list have been marked as unread, \c false if they have been marked as read
     */
    void itemsMarked(const Fuoten::IdList &itemIds, bool unread);

    /*!
     * \brief Takes and processes data after an item/article has been starred/unstarred.
     *
     * handleStorageChanged() will connect the AbstractStorage::starredItem() signal to this slot.
     *
     * \param feedId    ID of the feed the article/item that has been starred/unstarred belongs to
     * \param guidHash  GUID hash of the article/item that has been starred/unstarred
     * \param starred   \c true if the article/item has been starred, \c false if it has been unstarred
     */
    void itemStarred(qint64 feedId, const QString &guidHash, bool starred);

    /*!
     * \brief Takes and processes data after a list of items/articles has been starred/unstarred.
     *
     * handleStorageChanged() will connect the AbstractStorage::starredItems() signal to this slot.
     *
     * \param articles  list of pairs of feed ID and article/item guid hash of articles that have been starred/unstarred
     * \param starred   \c true if the articles in the list have been starred, \c false if the have been unstarred
     */
    void itemsStarred(const QList<QPair<qint64,QString>> &articles, bool starred);

    /*!
     * \brief Takes and processes data after all items/articles have been marked as read.
     *
     * handleStorageChanged() will connect the AbstractStorage::markedAllItemsRead() signal to this slot.
     *
     * \param newestItemId  highest/newest ID of local available items/articles
     */
    void allItemsMarkedRead(qint64 newestItemId);

    /*!
     * \brief Takes and processes data after all items/articles have been marked as read in local queue.
     *
     * handleStorageChanged() will connect the AbstractStorage::markedAllItemsReadInQueue() signal to this slot.
     */
    void allItemsMarkedReadInQueue();

    /*!
     * \brief Updates the model after the local queue has been cleared.
     *
     * Will set the Article::queue property of every article in the model to FuotenEnums::NoQueueAction.
     * handleStorageChanged() will connect the AbstractStorage::queueCleared() signal to this slot.
     */
    void queueCleared();

protected:
    AbstractArticleModel(AbstractArticleModelPrivate &dd, QObject *parent = nullptr);

    /*!
     * \brief Connets the article related signals of AbstractStorage to the model's slots.
     */
    void handleStorageChanged(AbstractStorage *old) override;

    /*!
     * \brief Returns the list of Article objects in the model.
     */
    QList<Article*> articles() const;

    /*!
     * \brief Clears the model and destroy all Article objects.
     */
    void clear() override;

private:
    Q_DECLARE_PRIVATE(AbstractArticleModel)
    Q_DISABLE_COPY(AbstractArticleModel)
};

}

#endif // FUOTENABSTRACTARTICLEMODEL_H
