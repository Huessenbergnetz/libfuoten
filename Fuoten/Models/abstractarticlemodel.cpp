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
}



void AbstractArticleModel::load()
{
    if (!storage()) {
        qWarning("Can not laod articles, no storage available.");
        return;
    }

    if (!storage()->ready() || loaded() || inOperation()) {
        return;
    }

    setInOperation(true);

    Q_D(AbstractArticleModel);

    QList<Article*> as;
    if (parentId() < 0) {
        if (parentIdType() != FuotenEnums::Starred) {
            as = storage()->getArticles();
        } else {
            as = storage()->getArticles(FuotenEnums::Time, Qt::DescendingOrder, QList<qint64>(), FuotenEnums::Feed, false, true);
        }
    } else {
        QList<qint64> ids({parentId()});
        as = storage()->getArticles(FuotenEnums::Time, Qt::DescendingOrder, ids, parentIdType());
    }

    if (!as.isEmpty()) {

        beginInsertRows(QModelIndex(), 0, as.count() - 1);

        d->articles = as;

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
