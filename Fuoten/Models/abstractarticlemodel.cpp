#include "abstractarticlemodel_p.h"
#include "../Storage/abstractstorage.h"
#ifdef QT_DEBUG
#include <QtDebug>
#endif

using namespace Fuoten;

AbstractArticleModel::AbstractArticleModel(QObject *parent) :
    BaseModel(* new AbstractArticleModelPrivate, parent)
{
}


AbstractArticleModel::AbstractArticleModel(AbstractArticleModelPrivate &dd, QObject *parent) :
    BaseModel(dd, parent)
{
}


FuotenEnums::Type AbstractArticleModel::parentIdType() const { Q_D(const AbstractArticleModel); return d->parentIdType; }

void AbstractArticleModel::setParentIdType(FuotenEnums::Type nParentIdType)
{
    Q_D(AbstractArticleModel);
    if (nParentIdType != d->parentIdType) {
        d->parentIdType = nParentIdType;
#ifdef QT_DEBUG
        qDebug() << "Changed parentIdType to" << d->parentIdType;
#endif
        Q_EMIT parentIdTypeChanged(parentIdType());
    }
}




void AbstractArticleModel::handleStorageChanged()
{
    AbstractStorage *s = storage();
    connect(s, &AbstractStorage::gotArticlesAsync, this, &AbstractArticleModel::gotArticlesAsync);
}



void AbstractArticleModel::load()
{
    if (!storage()) {
        qWarning("Can not load articles, no storage available.");
        return;
    }

    if (!storage()->ready() || loaded() || inOperation()) {
        return;
    }

    setInOperation(true);

    QueryArgs qa;
    qa.sortingRole = FuotenEnums::Time;
    qa.sortOrder = Qt::DescendingOrder;
    qa.parentId = parentId();
    qa.parentIdType = parentIdType();

    if ((parentId() < 0) && (parentIdType() == FuotenEnums::Starred)) {
        qa.starredOnly = true;
    }

    storage()->getArticlesAsync(qa);
}



void AbstractArticleModel::gotArticlesAsync(const ArticleList &articles)
{
    if (!articles.isEmpty()) {

        Q_D(AbstractArticleModel);

        beginInsertRows(QModelIndex(), rowCount(), rowCount() + articles.count() -1);

        for (Article *a : articles) {
            if (a->thread() != this->thread()) {
                d->articles.append(new Article(a));
                delete a;
            } else {
                d->articles.append(a);
            }
        }

        endInsertRows();
    }

    setLoaded(true);

    setInOperation(false);
}



QModelIndex AbstractArticleModel::findByID(qint64 id) const
{
    Q_D(const AbstractArticleModel);

    if (d->articles.isEmpty()) {
        return QModelIndex();
    }

    int idx = -1;

    for (int i = 0; i < d->articles.count(); ++i) {
        if (d->articles.at(i)->id() == id) {
            idx = i;
            break;
        }
    }

    return (idx > -1) ? index(idx, 0) : QModelIndex();
}



QHash<qint64, QModelIndex> AbstractArticleModel::findByIDs(const QList<qint64> &ids) const
{
    Q_D(const AbstractArticleModel);

    QHash<qint64, QModelIndex> idxs;

    if (d->articles.isEmpty()) {
        return idxs;
    }

    for (int i = 0; i < d->articles.count(); ++i) {
        Article *a = d->articles.at(i);
        if (ids.contains(a->id())) {
            idxs.insert(a->id(), index(i, 0));
        }
    }

    return idxs;
}



QList<Article*> AbstractArticleModel::articles() const
{
    Q_D(const AbstractArticleModel);
    return d->articles;
}




void AbstractArticleModel::clear()
{
    Q_D(AbstractArticleModel);

    if (!d->articles.isEmpty()) {

        beginRemoveRows(QModelIndex(), 0, rowCount() - 1);

        qDeleteAll(d->articles);
        d->articles.clear();

        endRemoveRows();
    }
}



void AbstractArticleModel::itemsRequested(const IdList &updatedItems, const IdList &newItems, const IdList &deletedItems)
{
    if (!storage()) {
        qWarning("Can not load articles, no storage available.");
        return;
    }

    if (rowCount() == 0) {
        reload();
        return;
    }

    if (!updatedItems.isEmpty()) {

        QHash<qint64, QModelIndex> idxs = findByIDs(updatedItems);

        if (!idxs.isEmpty()) {

            QueryArgs qa;
            qa.parentId = parentId();
            qa.parentIdType = parentIdType();
            qa.inIds = idxs.keys();
            qa.inIdsType = FuotenEnums::Item;
            const QList<Article*> upits = storage()->getArticles(qa);
//            const QList<Article*> upits = storage()->getArticles(FuotenEnums::ID, Qt::DescendingOrder, idxs.keys(), FuotenEnums::Item);

            if (!upits.isEmpty()) {

            }
        }

    }

    if (!newItems.isEmpty()) {

//        const QList<Article*> upits = storage()->getArticles(FuotenEnums::Time, Qt::AscendingOrder, newItems, FuotenEnums::Item);
        QueryArgs qa;
        qa.parentId = parentId();
        qa.parentIdType = parentIdType();
        qa.inIds = newItems;
        qa.inIdsType = FuotenEnums::Item;
        const QList<Article*> newits = storage()->getArticles(qa);

        if (!newits.isEmpty()) {

            for (Article *a : newits) {

                if (a->feedId()) {

                }
            }
        }
    }
}
