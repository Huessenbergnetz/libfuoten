/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016-2018 Hüssenbergnetz/Matthias Fehring
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

#ifndef SQLITESTORAGE_P
#define SQLITESTORAGE_P

#include "sqlitestorage.h"
#include "abstractstorage_p.h"
#include "../Helpers/abstractconfiguration.h"
#include "../Helpers/abstractnotificator.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QStringList>
#include <QThread>
#include <QJsonDocument>
#include <QSqlQuery>

namespace Fuoten {


class SQLiteStorageManager : public QThread {
    Q_OBJECT
public:
    explicit SQLiteStorageManager(const QString &dbpath, QObject *parent = nullptr);

private:
    QSqlDatabase m_db;
    quint16 m_currentDbVersion;
    void setFailed(const QSqlError &sqlError, const QString &text);

protected:
    void run() override;

Q_SIGNALS:
    void succeeded();
    void failed(Fuoten::Error *error);
};






class SQLiteStoragePrivate : public AbstractStoragePrivate {
public:
    SQLiteStoragePrivate(const QString &_dbpath) : AbstractStoragePrivate()
    {
        if (!QSqlDatabase::connectionNames().contains(QStringLiteral("fuotendb"))) {
            db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), QStringLiteral("fuotendb"));
            db.setDatabaseName(_dbpath);
        } else {
            db = QSqlDatabase::database(QStringLiteral("fuotendb"));
        }
    }

    QStringList intListToStringList(const IdList &ints) const
    {
        QStringList sl;

        if (!ints.isEmpty()) {
            sl.reserve(ints.size());

            for (qint64 i : ints) {
                sl.append(QString::number(i));
            }

        }

        return sl;
    }

    QString intListToString(const IdList &ints) const
    {
        if (ints.isEmpty()) {
            return QString();
        } else if (ints.count() == 1) {
            return QString::number(ints.first());
        } else {
            return intListToStringList(ints).join(QChar(','));
        }
    }

    QSqlQuery getQuery() const
    {
        return QSqlQuery(db);
    }

    QSqlDatabase db;
    QThread worker;
};





class ItemsRequestedWorker : public QThread
{
    Q_OBJECT
public:
    ItemsRequestedWorker(const QString &dbpath, const QJsonDocument &json, AbstractConfiguration *config = nullptr, AbstractNotificator *notificator = nullptr, QObject *parent = nullptr);

Q_SIGNALS:
    void requestedItems(const Fuoten::IdList &updatedItems, const Fuoten::IdList &newItems, const Fuoten::IdList &deletedItems);
    void gotTotalUnread(quint16 tu);
    void gotStarred(quint16 st);
    void failed(Fuoten::Error *e);

protected:
    void run() override;

private:
    QSqlDatabase m_db;
    QJsonDocument m_json;
    AbstractConfiguration *m_config;
    AbstractNotificator *m_notificator;
};




class GetArticlesAsyncWorker : public QThread
{
    Q_OBJECT
public:
    GetArticlesAsyncWorker(const QString &dbpath, const QueryArgs &args, QObject *parent = nullptr);

Q_SIGNALS:
    void gotArticles(const Fuoten::ArticleList &articles);
    void failed(Fuoten::Error *e);

protected:
    void run() override;

private:
    QSqlDatabase m_db;
    QueryArgs m_args;
};


class EnqueueMarkReadWorker : public QThread
{
    Q_OBJECT
public:
    EnqueueMarkReadWorker(const QString &dbpath, qint64 id, FuotenEnums::Type idType, qint64 newestItemId = -1, QObject *parent = nullptr);

Q_SIGNALS:
    void failed(Fuoten::Error *e);
    void markedReadFeedInQueue(qint64 feedId, qint64 newestItemId);
    void markedReadFolderInQueue(qint64 folderId, qint64 newestItemId);
    void markedAllItemsReadInQueue();
    void gotTotalUnread(quint16 tu);

protected:
    void run() override;

private:
    qint64 m_id;
    qint64 m_newestItemId;
    QSqlDatabase m_db;
    FuotenEnums::Type m_idType;
};



class ClearQueueWorker : public QThread
{
    Q_OBJECT
public:
    ClearQueueWorker(const QString &dbpath, QObject *parent = nullptr);

Q_SIGNALS:
    void failed(Fuoten::Error *e);
    void queueCleared();

protected:
    void run() override;

private:
    QSqlDatabase m_db;
};


}

#endif // SQLITESTORAGE_P

