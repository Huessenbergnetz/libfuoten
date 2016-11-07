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
#if QT_VERSION < QT_VERSION_CHECK(5, 5, 0)
    Q_ENUMS(Fuoten::FuotenEnums::Type)
#endif
public:
    /*!
     * \brief Constructs a new empty abstract Article model with the given \a parent.
     */
    explicit AbstractArticleModel(QObject *parent = nullptr);


    /*!
     * \brief Getter function for the \link AbstractArticleModel::parentIdType parentIdType \endlink property.
     * \sa AbstractArticleModel::setParentIdType(), AbstractArticleModel::parentIdTypeChanged()
     */
    FuotenEnums::Type parentIdType() const;

    /*!
     * \brief Setter function for the \link AbstractArticleModel::parentIdType parentIdType \endlink property.
     * Emits the parentIdTypeChanged() signal if \a nParentIdType is not equal to the stored value.
     * \sa AbstractArticleModel::parentIdType(), AbstractArticleModel::parentIdTypeChanged()
     */
    void setParentIdType(FuotenEnums::Type nParentIdType);


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

protected Q_SLOTS:
    void gotArticlesAsync(const ArticleList &articles);

    /*!
     * \brief Takes and processes data after items/articles have been requested.
     *
     * handleStorageChanged() will connect the AbstractStorage::requestedItems() signal to this slot.
     *
     * \param updatedItems  list of IDs of items that have been updated
     * \param newItems      list of IDs of items that are new to the local storage
     * \param deletedItems  list of IDs of items that have benn updated in the local storage
     */
    void itemsRequested(const IdList &updatedItems, const IdList &newItems, const IdList &deletedItems);

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
     * \brief Takes and processes data after a feed has been marked as read.
     *
     * handleStorageChanged() will connect the AbstractStorage::markedReadFeed() signal to this slot.
     *
     * \param feedId        ID of the feed that has been marked as read
     * \param newestItemId  ID of the newest item that has been marked as read
     */
    void feedMarkedRead(qint64 feedId, qint64 newestItemId);

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

protected:
    AbstractArticleModel(AbstractArticleModelPrivate &dd, QObject *parent = nullptr);

    /*!
     * \brief Connets the article related signals of AbstractStorage to the model's slots.
     */
    void handleStorageChanged() override;

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
