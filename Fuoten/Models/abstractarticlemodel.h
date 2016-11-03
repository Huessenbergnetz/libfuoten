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
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
    Q_ENUM(Fuoten::FuotenEnums::Type)
#else
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
    QHash<qint64, QModelIndex> findByIDs(const QList<qint64> &ids) const override;

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

private:
    Q_DECLARE_PRIVATE(AbstractArticleModel)
    Q_DISABLE_COPY(AbstractArticleModel)
};

}

#endif // FUOTENABSTRACTARTICLEMODEL_H
