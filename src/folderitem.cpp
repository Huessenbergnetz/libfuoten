#include "folderitem_p.h"
#include <QJsonDocument>
#include <QJsonObject>
#ifdef QT_DEBUG
#include <QtDebug>
#endif

using namespace Fuoten;


FolderItem::FolderItem(QObject *parent) :
    BaseItem(* new FolderItemPrivate, parent)
{
}


FolderItem::FolderItem(const QJsonDocument &json, QObject *parent) :
    BaseItem(* new FolderItemPrivate, parent)
{
    loadFromJson(json);
}


FolderItem::FolderItem(const QJsonObject &json, QObject *parent) :
    BaseItem(* new FolderItemPrivate, parent)
{
    loadFromJson(json);
}


FolderItem::FolderItem(quint64 id, const QString &name, uint feedCount, uint unreadCount, uint itemCount, QObject *parent) :
    BaseItem(* new FolderItemPrivate(id, name, feedCount, unreadCount, itemCount), parent)
{

}


FolderItem::FolderItem(Fuoten::FolderItemPrivate &dd, QObject *parent) :
    BaseItem(dd, parent)
{

}


QString FolderItem::name() const { Q_D(const FolderItem); return d->name; }

void FolderItem::setName(const QString &nName)
{
    Q_D(FolderItem); 
    if (nName != d->name) {
        d->name = nName;
#ifdef QT_DEBUG
        qDebug() << "Changed name to" << d->name;
#endif
        Q_EMIT nameChanged(name());
    }
}




uint FolderItem::feedCount() const { Q_D(const FolderItem); return d->feedCount; }

void FolderItem::setFeedCount(uint nFeedCount)
{
    Q_D(FolderItem);
    if (nFeedCount != d->feedCount) {
        d->feedCount = nFeedCount;
#ifdef QT_DEBUG
        qDebug() << "Changed feedCount to" << d->feedCount;
#endif
        Q_EMIT feedCountChanged(feedCount());
    }
}




uint FolderItem::unreadCount() const { Q_D(const FolderItem); return d->unreadCount; }

void FolderItem::setUnreadCount(uint nUnreadCount)
{
    Q_D(FolderItem);
    if (nUnreadCount != d->unreadCount) {
        d->unreadCount = nUnreadCount;
#ifdef QT_DEBUG
        qDebug() << "Changed unreadCount to" << d->unreadCount;
#endif
        Q_EMIT unreadCountChanged(unreadCount());
    }
}




uint FolderItem::itemCount() const { Q_D(const FolderItem); return d->itemCount; }

void FolderItem::setItemCount(const uint &nItemCount)
{
    Q_D(FolderItem);
    if (nItemCount != d->itemCount) {
        d->itemCount = nItemCount;
#ifdef QT_DEBUG
        qDebug() << "Changed itemCount to" << d->itemCount;
#endif
        Q_EMIT itemCountChanged(itemCount());
    }
}







void FolderItem::rename(const QString &nName)
{
    Q_D(FolderItem);

    if (d->id == 0) {
        qWarning("Can not change the folder name. No ID available.");
        return;
    }

    if (!d->configuration) {
        qWarning("Can not change the folder name. No configuration available.");
        return;
    }

    if (d->comp) {
        qWarning("Can not change the name. There is still another operation running.");
        return;
    }

    d->newName = nName;
}


void FolderItem::loadFromJson(const QJsonDocument &json)
{
    loadFromJson(json.object());
}


void FolderItem::loadFromJson(const QJsonObject &json)
{
    if (json.isEmpty()) {
        return;
    }
}
