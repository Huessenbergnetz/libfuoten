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
    BaseItem(*new FolderItemPrivate, parent)
{
    loadFromJson(json);
}


FolderItem::FolderItem(const QJsonObject &json, QObject *parent) :
    BaseItem(*new FolderItemPrivate, parent)
{
    loadFromJson(json);
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





void FolderItem::changeName(const QString &nName)
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
