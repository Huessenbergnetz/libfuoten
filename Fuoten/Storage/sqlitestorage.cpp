/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016 Buschtrommel / Matthias Fehring
 * https://www.buschmann23.de/entwicklung/bibliotheken/libfuoten/
 * https://github.com/Buschtrommel/libfuoten
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

#include "sqlitestorage_p.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QHash>
#include <QSqlQuery>
#include <QDateTime>
#include <QVariant>
#include <QRegularExpression>
#include "../folder.h"
#include "../feed.h"
#include "../article.h"

#ifdef QT_DEBUG
#include <QtDebug>
#endif

using namespace Fuoten;



SQLiteStorageManager::SQLiteStorageManager(const QString &dbpath, QObject *parent) :
    QThread(parent), m_currentDbVersion(0)
{
    if (!QSqlDatabase::connectionNames().contains(QStringLiteral("fuotendb"))) {
        m_db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), QStringLiteral("fuotendb"));
        m_db.setDatabaseName(dbpath);
    } else {
        m_db = QSqlDatabase::database(QStringLiteral("fuotendb"));
    }
}


void SQLiteStorageManager::setFailed(const QSqlError &sqlError, const QString &text)
{
    Error *e = new Error(sqlError, text);
    e->moveToThread(this->thread());
    e->setParent(this);
    Q_EMIT failed(e);
}



void SQLiteStorageManager::run()
{
    if (!m_db.open()) {
        //% "Failed to open the SQLite database."
        setFailed(m_db.lastError(), qtTrId("libfuoten-err-failed-open-db"));
        return;
    }

#ifdef QT_DEBUG
    qDebug() << "Start checking database scheme";
#endif

    QSqlQuery q(m_db);

    if (!q.exec(QStringLiteral("PRAGMA foreign_keys = ON"))) {
        //% "Failed to execute database query."
        setFailed(q.lastError(), qtTrId("fuoten-error-failed-execute-query"));
        return;
    }

    if (!m_db.transaction()) {
        //% "Failed to begin a database transaction."
        setFailed(q.lastError(), qtTrId("fuoten-error-transaction-begin"));
        return;
    }

    if (!q.exec(QStringLiteral("CREATE TABLE IF NOT EXISTS system "
                               "(id INTEGER PRIMARY KEY NOT NULL, "
                               "key TEXT NOT NULL, "
                               "value TEXT NO NULL)"
                               ))) {
        //% "Failed to execute database query."
        setFailed(q.lastError(), qtTrId("fuoten-error-failed-execute-query"));
        return;
    }

    if (!q.exec(QStringLiteral("CREATE TABLE IF NOT EXISTS folders "
                               "(id INTEGER PRIMARY KEY NOT NULL, "
                               "name TEXT NOT NULL, "
                               "unreadCount INTEGER DEFAULT 0, "
                               "feedCount INTEGER DEFAULT 0)"
                               ))) {
        //% "Failed to execute database query."
        setFailed(q.lastError(), qtTrId("fuoten-error-failed-execute-query"));
        return;
    }

    if (!q.exec(QStringLiteral("INSERT OR IGNORE INTO folders (id, name) VALUES (0, '')"))) {
        //% "Failed to execute database query."
        setFailed(q.lastError(), qtTrId("fuoten-error-failed-execute-query"));
        return;
    }

    if (!q.exec(QStringLiteral("CREATE TABLE IF NOT EXISTS feeds "
                               "(id INTEGER PRIMARY KEY NOT NULL, "
                               "folderId INTEGER NOT NULL, "
                               "title TEXT NOT NULL, "
                               "url TEXT NOT NULL, "
                               "link TEXT NOT NULL, "
                               "added INTEGER NOT NULL, "
                               "unreadCount INTEGER DEFAULT 0, "
                               "ordering INTEGER NOT NULL, "
                               "pinned INTEGER NOT NULL, "
                               "updateErrorCount INTEGER NOT NULL, "
                               "lastUpdateError TEXT, "
                               "faviconLink TEXT, "
                               "FOREIGN KEY(folderId) REFERENCES folders(id) ON DELETE CASCADE)"
                               ))) {
        //% "Failed to execute database query."
        setFailed(q.lastError(), qtTrId("fuoten-error-failed-execute-query"));
        return;
    }

    if (!q.exec(QStringLiteral("CREATE TABLE IF NOT EXISTS items "
                               "(id INTEGER PRIMARY KEY NOT NULL, "
                               "feedId INTEGER NOT NULL, "
                               "guid TEXT NOT NULL, "
                               "guidHash TEXT NOT NULL, "
                               "url TEXT NOT NULL, "
                               "title TEXT NOT NULL, "
                               "author TEXT NOT NULL, "
                               "pubDate INTEGER NOT NULL, "
                               "body TEXT NOT NULL, "
                               "enclosureMime TEXT, "
                               "enclosureLink TEXT, "
                               "unread INTEGER NOT NULL, "
                               "starred INTEGER NOT NULL, "
                               "lastModified INTEGER NOT NULL, "
                               "fingerprint TEXT NOT NULL, "
                               "queue INTEGER DEFAULT 0, "
                               "FOREIGN KEY(feedId) REFERENCES feeds(id) ON DELETE CASCADE)"
                               ))) {
        //% "Failed to execute database query."
        setFailed(q.lastError(), qtTrId("fuoten-error-failed-execute-query"));
        return;
    }

    if (!q.exec(QStringLiteral("CREATE INDEX IF NOT EXISTS feeds_folder_id_index ON feeds (folderId)"))) {
        //% "Failed to execute database query."
        setFailed(q.lastError(), qtTrId("fuoten-error-failed-execute-query"));
        return;
    }

    if (!q.exec(QStringLiteral("CREATE INDEX IF NOT EXISTS items_item_guid ON items (guidHash, feedId)"))) {
        //% "Failed to execute database query."
        setFailed(q.lastError(), qtTrId("fuoten-error-failed-execute-query"));
        return;
    }

    if (!q.exec(QStringLiteral("CREATE INDEX IF NOT EXISTS items_feed_id_index ON items (feedId)"))) {
        //% "Failed to execute database query."
        setFailed(q.lastError(), qtTrId("fuoten-error-failed-execute-query"));
        return;
    }

    if (!q.exec(QStringLiteral("CREATE TRIGGER IF NOT EXISTS feeds_unreadCount_update_item AFTER UPDATE OF unread ON items "
                               "BEGIN "
                               "UPDATE feeds SET unreadCount = (SELECT COUNT(id) FROM items WHERE unread = 1 AND feedId = old.feedId) WHERE id = old.feedId; "
                               "END"
                               ))) {
        //% "Failed to execute database query."
        setFailed(q.lastError(), qtTrId("fuoten-error-failed-execute-query"));
        return;
    }

    if (!q.exec(QStringLiteral("CREATE TRIGGER IF NOT EXISTS feeds_unreadCount_delete_item AFTER DELETE ON items "
                               "BEGIN "
                               "UPDATE feeds SET unreadCount = (SELECT COUNT(id) FROM items WHERE unread = 1 AND feedId = old.feedId) WHERE id = old.feedId; "
                               "END"
                               ))) {
        //% "Failed to execute database query."
        setFailed(q.lastError(), qtTrId("fuoten-error-failed-execute-query"));
        return;
    }

    if (!q.exec(QStringLiteral("CREATE TRIGGER IF NOT EXISTS feeds_unreadCount_insert_item AFTER INSERT ON items "
                               "BEGIN "
                               "UPDATE feeds SET unreadCount = (SELECT COUNT(id) FROM items WHERE unread = 1 AND feedId = new.feedId) WHERE id = new.feedId; "
                               "END"
                               ))) {
        //% "Failed to execute database query."
        setFailed(q.lastError(), qtTrId("fuoten-error-failed-execute-query"));
        return;
    }

    if (!q.exec(QStringLiteral("CREATE TRIGGER IF NOT EXISTS folders_unreadCount_update_feed AFTER UPDATE OF unreadCount ON feeds "
                               "BEGIN "
                               "UPDATE folders SET unreadCount = (SELECT SUM(unreadCount) FROM feeds WHERE folderId = old.folderId) WHERE id = old.folderId; "
                               "END"
                               ))) {
        //% "Failed to execute database query."
        setFailed(q.lastError(), qtTrId("fuoten-error-failed-execute-query"));
        return;
    }

    if (!q.exec(QStringLiteral("CREATE TRIGGER IF NOT EXISTS folders_counts_delete_feed AFTER DELETE ON feeds "
                               "BEGIN "
                               "UPDATE folders SET unreadCount = (SELECT SUM(unreadCount) FROM feeds WHERE folderId = old.folderId) WHERE id = old.folderId; "
                               "UPDATE folders SET feedCount = (SELECT COUNT(id) FROM feeds WHERE folderId = old.folderId) WHERE id = old.folderId; "
                               "END"))) {
        //% "Failed to execute database query."
        setFailed(q.lastError(), qtTrId("fuoten-error-failed-execute-query"));
        return;
    }

    if (!q.exec(QStringLiteral("CREATE TRIGGER IF NOT EXISTS folders_counts_insert_feed AFTER INSERT ON feeds "
                               "BEGIN "
                               "UPDATE folders SET unreadCount = (SELECT SUM(unreadCount) FROM feeds WHERE folderId = new.folderId) WHERE id = new.folderId; "
                               "UPDATE folders SET feedCount = (SELECT COUNT(id) FROM feeds WHERE folderId = new.folderId) WHERE id = new.folderId; "
                               "END"
                               ))) {
        //% "Failed to execute database query."
        setFailed(q.lastError(), qtTrId("fuoten-error-failed-execute-query"));
        return;
    }

    if (!q.exec(QStringLiteral("CREATE TRIGGER IF NOT EXISTS folders_counts_move_feed AFTER UPDATE OF folderId ON feeds "
                               "BEGIN "
                               "UPDATE folders SET unreadCount = (SELECT SUM(unreadCount) FROM feeds WHERE folderId = new.folderId) WHERE id = new.folderId; "
                               "UPDATE folders SET unreadCount = (SELECT SUM(unreadCount) FROM feeds WHERE folderId = old.folderId) WHERE id = old.folderId; "
                               "UPDATE folders SET feedCount = (SELECT COUNT(id) FROM feeds WHERE folderId = new.folderId) WHERE id = new.folderId; "
                               "UPDATE folders SET feedCount = (SELECT COUNT(id) FROM feeds WHERE folderId = old.folderId) WHERE id = old.folderId; "
                               "END"))) {
        //% "Failed to execute database query."
        setFailed(q.lastError(), qtTrId("fuoten-error-failed-execute-query"));
        return;
    }

    if (!m_db.commit()) {
        //% "Failed to commit a database transaction."
        setFailed(m_db.lastError(), qtTrId("fuoten-error-transaction-commit"));
        return;
    }

    if (q.exec(QStringLiteral("SELECT value FROM system WHERE key = 'schema_version'"))) {
        if (q.next()) {
            m_currentDbVersion = q.value(0).toUInt();
        }
    } else {
        //% "Failed to execute database query."
        setFailed(q.lastError(), qtTrId("fuoten-error-failed-execute-query"));
        return;
    }

    if (m_currentDbVersion == 0) {
        if (!q.exec(QStringLiteral("INSERT INTO system (key, value) VALUES ('schema_version', '1')"))) {
            //% "Failed to execute database query."
            setFailed(q.lastError(), qtTrId("fuoten-error-failed-execute-query"));
            return;
        }
    }

    Q_EMIT succeeded();

#ifdef QT_DEBUG
    qDebug() << "Finished checking database scheme";
#endif
}



SQLiteStorage::SQLiteStorage(const QString &dbpath, QObject *parent) :
    AbstractStorage(* new SQLiteStoragePrivate(dbpath), parent)
{
}


void SQLiteStorage::init()
{
    Q_D(SQLiteStorage);

    SQLiteStorageManager *sm = new SQLiteStorageManager(d->db.databaseName(), this);
    connect(sm, &SQLiteStorageManager::succeeded, [=] () {
        if (d->db.open()) {
            QSqlQuery q(d->db);
            q.exec(QStringLiteral("PRAGMA foreign_keys = ON"));
            if (!q.exec(QStringLiteral("SELECT COUNT(id) FROM items WHERE unread = 1"))) {
                //% "Failed to execute database query."
                setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query"), this));
                return;
            }

            if (q.next()) {
                setTotalUnread(q.value(0).toInt());
            }

            if (!q.exec(QStringLiteral("SELECT COUNT(id) FROM items WHERE starred = 1"))) {
                //% "Failed to execute database query."
                setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query"), this));
                return;
            }

            if (q.next()) {
                setStarred(q.value(0).toInt());
            }

            setReady(true);
        } else {
            //% "Failed to open the SQLite database."
            setError(new Error(d->db.lastError(), qtTrId("libfuoten-err-failed-open-db"), this));
        }
    });
    connect(sm, &SQLiteStorageManager::failed, this, &SQLiteStorage::setError);
    connect(sm, &SQLiteStorageManager::finished, sm, &QObject::deleteLater);
    sm->start(QThread::LowPriority);
}




qint64 SQLiteStorage::getNewestItemId(FuotenEnums::Type type, qint64 id)
{
    if (!ready()) {
        //% "SQLite database not ready. Can not process requested data."
        setError(new Error(Error::StorageError, Error::Warning, qtTrId("libfuoten-err-sqlite-db-not-ready"), QString(), this));
        return -1;
    }

    if ((type == FuotenEnums::Folder) && (id <= 0)) {
        //% "The folder ID is not valid."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-invalid-folder-id"), QString(), this));
        return -1;
    }

    if ((type == FuotenEnums::Feed) && (id <= 0)) {
        //% "The feed ID is not valid."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-invalid-feed-id"), QString(), this));
        return -1;
    }

    QString qs;

    switch (type) {
    case FuotenEnums::Feed:
        qs = QStringLiteral("SELECT id FROM items WHERE feedId = ? ORDER BY id DESC LIMIT 1");
        break;
    case FuotenEnums::All:
        qs = QStringLiteral("SELECT id FROM items ORDER BY id DESC LIMIT 1");
    case FuotenEnums::Folder:
        qs = QStringLiteral("SELECT id FROM items WHERE feedId IN (SELECT id FROM feeds WHERE folderId = ?) ORDER BY id DESC LIMIT 1");
        break;
    default:
        return -1;
    }

    Q_D(SQLiteStorage);

    QSqlQuery q(d->db);

    if (type == FuotenEnums::All) {

        if (!q.exec(qs)) {
            //% "Failed to execute database query."
            setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query"), this));
            return -1;
        }

    } else {

        if (!q.prepare(qs)) {
            //% "Failed to prepare database query."
            setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-prepare-query"), this));
            return -1;
        }

        q.addBindValue(id);

        if (!q.exec()) {
            //% "Failed to execute database query."
            setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query"), this));
            return -1;
        }
    }

    if (q.next()) {
        return q.value(0).toLongLong();
    } else {
        return -1;
    }
}



void SQLiteStorage::foldersRequested(const QJsonDocument &json)
{
    Q_D(SQLiteStorage);

    if (!ready()) {
        //% "SQLite database not ready. Can not process requested data."
        setError(new Error(Error::StorageError, Error::Warning, qtTrId("libfuoten-err-sqlite-db-not-ready"), QString(), this));
        return;
    }

    if (json.isEmpty() || json.isNull()) {
        return;
    }

    const QJsonArray folders = json.object().value(QStringLiteral("folders")).toArray();

#ifdef QT_DEBUG
    qDebug() << "Processing" << folders.size() << "folders requested from the remote server.";
#endif

    QHash<qint64, QString> reqFolders({{0, QStringLiteral("")}});

    for (const QJsonValue &f : folders) {
        QJsonObject o = f.toObject();
        if (!o.isEmpty()) {
            reqFolders.insert(o.value(QStringLiteral("id")).toVariant().toLongLong(), o.value(QStringLiteral("name")).toString());
        }
    }

    QSqlQuery q(d->db);

    // query the currently local available folders in the database
    QHash<qint64, QString> currentFolders;

    if (!q.exec(QStringLiteral("SELECT id, name FROM folders"))) {
        //% "Failed to query the folders from the local database."
        setError(new Error(q.lastError(), qtTrId("fuoten-failed-query-folders"), this));
        return;
    }

    while (q.next()) {
        currentFolders.insert(q.value(0).toLongLong(), q.value(1).toString());
    }

    if (reqFolders.isEmpty() && currentFolders.isEmpty()) {
#ifdef QT_DEBUG
        qDebug() << "Nothing to do. Returning.";
#endif
        return;
    }

    IdList deletedIds;
    QList<QPair<qint64, QString>> newFolders;
    QList<QPair<qint64, QString>> updatedFolders;

    if (currentFolders.isEmpty()) {

#ifdef QT_DEBUG
        qDebug() << "No local folders. Adding all requested folders as new.";
#endif

        QHash<qint64, QString>::const_iterator i = reqFolders.constBegin();
        while (i != reqFolders.constEnd()) {
            newFolders.append(QPair<qint64, QString>(i.key(), i.value()));
            ++i;
        }

    } else if (reqFolders.isEmpty()) {

#ifdef QT_DEBUG
        qDebug() << "Requested folders list is empty. Adding all local folders to deleted.";
#endif

        deletedIds = currentFolders.keys();

    } else {

#ifdef QT_DEBUG
        qDebug() << "Checking for updated and deleted folders.";
#endif

        for (QHash<qint64, QString>::const_iterator i = currentFolders.constBegin(); i != currentFolders.constEnd(); ++i) {
            if (reqFolders.contains(i.key())) {
                if (reqFolders.value(i.key()) != i.value()) {
                    updatedFolders.append(qMakePair(i.key(), reqFolders.value(i.key())));
                }
            } else {
                deletedIds << i.key();
            }
        }

#ifdef QT_DEBUG
        qDebug() << "Checking for new folders.";
#endif
        for (QHash<qint64, QString>::const_iterator i = reqFolders.constBegin(); i != reqFolders.constEnd(); ++i) {
            if (!currentFolders.contains(i.key())) {
                newFolders.append(qMakePair(i.key(), i.value()));
            }
        }
    }


    if (!deletedIds.isEmpty() || !newFolders.isEmpty() || !updatedFolders.isEmpty()) {

#ifdef QT_DEBUG
        qDebug() << "Start updating the folders table.";
#endif

        if (!d->db.transaction()) {
            //% "Failed to begin a database transaction."
            setError(new Error(q.lastError(), qtTrId("fuoten-error-transaction-begin"), this));
            return;
        }

        if (!deletedIds.isEmpty()) {

#ifdef QT_DEBUG
            qDebug() << "Deleting folders with IDs" << deletedIds << "from local database.";
#endif

            if (!q.exec(QStringLiteral("DELETE FROM folders WHERE id IN (%1)").arg(d->intListToString(deletedIds)))) {
                //% "Failed to execute database query."
                setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query"), this));
                return;
            }
        }

        if (!updatedFolders.isEmpty()) {

            for (int i = 0; i < updatedFolders.size(); ++i) {

#ifdef QT_DEBUG
                qDebug() << "Updating name of folder with ID" << updatedFolders.at(i).first << "in local database to:" << updatedFolders.at(i).second;
#endif

                if (!q.prepare(QStringLiteral("UPDATE folders SET name = ? WHERE id = ?"))) {
                    //% "Failed to prepare database query."
                    setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-prepare-query"), this));
                    return;
                }

                q.addBindValue(updatedFolders.at(i).second);
                q.addBindValue(updatedFolders.at(i).first);

                if (!q.exec()) {
                    //% "Failed to execute database query."
                    setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query"), this));
                    return;
                }
            }

        }


        if (!newFolders.isEmpty()) {

            for (int i = 0; i < newFolders.size(); ++i) {

#ifdef QT_DEBUG
                qDebug() << "Adding folder" << newFolders.at(i).second << "with ID" << newFolders.at(i).first << "to the local database.";
#endif
                if (!q.prepare(QStringLiteral("INSERT INTO folders (id, name) VALUES (?, ?)"))) {
                    //% "Failed to prepare database query."
                    setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-prepare-query"), this));
                    return;
                }

                q.addBindValue(newFolders.at(i).first);
                q.addBindValue(newFolders.at(i).second);

                if (!q.exec()) {
                    //% "Failed to execute database query."
                    setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query"), this));
                    return;
                }
            }
        }



        if (!d->db.commit()) {
            //% "Failed to commit a database transaction."
            setError(new Error(q.lastError(), qtTrId("fuoten-error-transaction-commit"), this));
            return;
        }

    }

    Q_EMIT requestedFolders(updatedFolders, newFolders, deletedIds);
}



void SQLiteStorage::folderCreated(const QJsonDocument &json)
{
    if (json.isEmpty() || json.isNull()) {
        qWarning("Can not add folder to SQLite database. JSON data is not valid.");
        return;
    }

    QJsonArray a = json.object().value(QStringLiteral("folders")).toArray();

    if (a.isEmpty()) {
        qWarning("Can not add folder to SQLite database. JSON array is empty.");
        return;
    }


    QJsonObject o = a.first().toObject();

    if (o.isEmpty()) {
        qWarning("Can not add folder to SQLite databse. JSON object is empty.");
        return;
    }

    const qint64 id = o.value(QStringLiteral("id")).toVariant().toLongLong();
    if (id == 0) {
        qWarning("Can not add folder to SQLite database. Invalid ID.");
        return;
    }

    const QString name = o.value(QStringLiteral("name")).toString();
    if (name.isEmpty()) {
        qWarning("Can not add folder to SQLite database. Empty name.");
    }

    Q_D(SQLiteStorage);

    QSqlQuery q(d->db);

    if (!q.prepare(QStringLiteral("INSERT INTO folders (id, name) VALUES (?, ?)"))) {
        //% "Failed to prepare database query."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-prepare-query"), this));
        return;
    }

    q.addBindValue(id);
    q.addBindValue(name);

    if (!q.exec()) {
        //% "Failed to execute database query."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query"), this));
        return;
    }

    Q_EMIT createdFolder(id, name);

}



void SQLiteStorage::folderRenamed(qint64 id, const QString &newName)
{
    if (!ready()) {
        //% "SQLite database not ready. Can not process requested data."
        setError(new Error(Error::StorageError, Error::Warning, qtTrId("libfuoten-err-sqlite-db-not-ready"), QString(), this));
        return;
    }

    if (newName.isEmpty()) {
        //% "The folder name can not be empty."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-empty-folder-name"), QString(), this));
        return;
    }

    if (id == 0) {
        //% "The folder ID is not valid."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-invalid-folder-id"), QString(), this));
        return;
    }

    Q_D(SQLiteStorage);

    QSqlQuery q(d->db);

    if (!q.prepare(QStringLiteral("UPDATE folders SET name = ? WHERE id = ?"))) {
        //% "Failed to prepare database query."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-prepare-query"), this));
        return;
    }

    q.addBindValue(newName);
    q.addBindValue(id);

    if (!q.exec()) {
        //% "Failed to execute database query."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query"), this));
        return;
    }

    Q_EMIT renamedFolder(id, newName);
}



QList<Folder*> SQLiteStorage::getFolders(FuotenEnums::SortingRole sortingRole, Qt::SortOrder sortOrder, const IdList &ids, Fuoten::FuotenEnums::Type idType, int limit)
{
    QList<Fuoten::Folder*> folders;

    if (!ready()) {
        qWarning("SQLite database not ready. Can not query folders from database.");
        return folders;
    }

    Q_D(SQLiteStorage);

    QSqlQuery q(d->db);

    QString qs = QStringLiteral("SELECT id, name, feedCount, unreadCount FROM folders ORDER BY ");

    if (!ids.isEmpty()) {
        if (idType == FuotenEnums::Feed) {
            qs = QStringLiteral("SELECT id, name, feedCount, unreadCount FROM folders WHERE id IN (SELECT folderId FROM feeds WHERE id IN (%1)) ORDER BY ").arg(d->intListToString(ids));
        } else {
            qs = QStringLiteral("SELECT id, name, feedCount, unreadCount FROM folders WHERE id IN (%1) ORDER BY ").arg(d->intListToString(ids));
        }
    }

    switch(sortingRole) {
    case FuotenEnums::Name:
        qs.append(QStringLiteral("name"));
        break;
    case FuotenEnums::ID:
        qs.append(QStringLiteral("id"));
        break;
    case FuotenEnums::UnreadCount:
        qs.append(QStringLiteral("unreadCount"));
        break;
    case FuotenEnums::FeedCount:
        qs.append(QStringLiteral("feedCount"));
        break;
    default:
        qs.append(QStringLiteral("name"));
        break;
    }

    if (sortOrder == Qt::AscendingOrder) {
        qs.append(QStringLiteral(" ASC"));
    } else {
        qs.append(QStringLiteral(" DESC"));
    }

    if (limit > 0) {
        qs.append(QLatin1String(" LIMIT ")).append(QString::number(limit));
    }

    if (!q.exec(qs)) {
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query"), this));
        return folders;
    }

    while (q.next()) {
        qint64 id = q.value(0).toLongLong();
        if (id > 0) {
            folders.append(new Folder(
                               q.value(0).toLongLong(),
                               q.value(1).toString(),
                               q.value(2).toUInt(),
                               q.value(3).toUInt()
                               ));
        }
    }

    return folders;
}



void SQLiteStorage::folderDeleted(qint64 id)
{
    if (!ready()) {
        //% "SQLite database not ready. Can not process requested data."
        setError(new Error(Error::StorageError, Error::Warning, qtTrId("libfuoten-err-sqlite-db-not-ready"), QString(), this));
        return;
    }

    if (id <= 0) {
        //% "The folder ID is not valid."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-invalid-folder-id"), QString(), this));
        return;
    }

    Q_D(SQLiteStorage);

    QSqlQuery q(d->db);

    if (!q.prepare(QStringLiteral("DELETE FROM folders WHERE id = ?"))) {
        //% "Failed to prepare database query."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-prepare-query"), this));
        return;
    }

    q.addBindValue(id);

    if (!q.exec()) {
        //% "Failed to execute database query."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query"), this));
        return;
    }

    Q_EMIT deletedFolder(id);
}



void SQLiteStorage::folderMarkedRead(qint64 id, qint64 newestItem)
{
    Q_D(SQLiteStorage);

    QSqlQuery q(d->db);

    if (!q.prepare(QStringLiteral("UPDATE items SET unread = 0, lastModified = ? WHERE feedId IN (SELECT id FROM feeds WHERE folderId = ?)"))) {
        //% "Failed to prepare database query."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-prepare-query"), this));
        return;
    }
    q.addBindValue(QDateTime::currentDateTimeUtc().toTime_t());
    q.addBindValue(id);

    if (!q.exec()) {
        //% "Failed to execute database query."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query"), this));
        return;
    }

    if (!q.exec(QStringLiteral("SELECT COUNT(id) FROM items WHERE unread = 1"))) {
        //% "Failed to execute database query."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query"), this));
        return;
    }

    if (q.next()) {
        setTotalUnread(q.value(0).toInt());
    }

    Q_EMIT markedReadFolder(id, newestItem);
}





QList<Feed*> SQLiteStorage::getFeeds(const QueryArgs &args)
{
    QList<Fuoten::Feed*> feeds;

    if (!ready()) {
        qWarning("SQLite database not ready. Can not query folders from database.");
        return feeds;
    }

    Q_D(SQLiteStorage);

    QSqlQuery q(d->db);

    QString qs = QStringLiteral("SELECT fe.id, fe.folderId, fe.title, fe.url, fe.link, fe.added, fe.unreadCount, fe.ordering, fe.pinned, fe.updateErrorCount, fe.lastUpdateError, fe.faviconLink, fo.name AS folderName FROM feeds fe LEFT JOIN folders fo ON fo.id = fe.folderId");

    if (args.parentId > -1 || !args.inIds.isEmpty() || args.unreadOnly) {
        qs.append(QLatin1String(" WHERE"));
    }

    if (args.parentId > -1) {
        qs.append(QStringLiteral(" fe.folderId = %1").arg(QString::number(args.parentId)));
    }

    if (!args.inIds.isEmpty() && ((args.inIdsType == FuotenEnums::Feed) || (args.inIdsType == FuotenEnums::Folder))) {
        if (args.parentId > -1) {
            qs.append(QLatin1String(" AND"));
        }
        if (args.inIdsType == FuotenEnums::Folder) {
            qs.append(QStringLiteral(" fe.folderId IN (%1)").arg(d->intListToString(args.inIds)));
        } else {
            qs.append(QStringLiteral(" fe.id IN (%1)").arg(d->intListToString(args.inIds)));
        }
    }

    if (args.unreadOnly) {
        if ((args.parentId > -1) || (!args.inIds.isEmpty() && ((args.inIdsType == FuotenEnums::Feed) || (args.inIdsType == FuotenEnums::Folder)))) {
            qs.append(QLatin1String(" AND"));
        }
        qs.append(QLatin1String(" fe.unreadCount > 0"));
    }

    switch(args.sortingRole) {
    case FuotenEnums::Name:
        qs.append(QLatin1String(" ORDER BY fe.title"));
        break;
    case FuotenEnums::FolderName:
        qs.append(QLatin1String(" ORDER BY fo.name"));
        break;
    case FuotenEnums::ID:
        qs.append(QLatin1String(" ORDER BY fe.id"));
        break;
    case FuotenEnums::UnreadCount:
        qs.append(QLatin1String(" ORDER BY fe.unreadCount"));
        break;
    default:
        qs.append(QLatin1String(" ORDER BY fe.title"));
        break;
    }

    if (args.sortOrder == Qt::AscendingOrder) {
        qs.append(QLatin1String(" ASC"));
    } else {
        qs.append(QLatin1String(" DESC"));
    }

    if (args.limit > 0) {
        qs.append(QLatin1String(" LIMIT ")).append(QString::number(args.limit));
    }

    if (!q.exec(qs)) {
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query"), this));
        return feeds;
    }

    while (q.next()) {
        feeds.append(new Feed(
                         q.value(0).toLongLong(),
                         q.value(1).toLongLong(),
                         q.value(2).toString(),
                         QUrl(q.value(3).toString()),
                         QUrl(q.value(4).toString()),
                         QDateTime::fromTime_t(q.value(5).toUInt()),
                         q.value(6).toUInt(),
                         (Feed::FeedOrdering)q.value(7).toInt(),
                         q.value(8).toBool(),
                         q.value(9).toUInt(),
                         q.value(10).toString(),
                         QUrl(q.value(11).toString()),
                         q.value(12).toString()
                         ));
    }

    return feeds;
}



Feed *SQLiteStorage::getFeed(qint64 id)
{
    if (!ready()) {
        qWarning("SQLite database not ready. Can not query folders from database.");
        return nullptr;
    }

    Q_D(SQLiteStorage);

    QSqlQuery q(d->db);

    if (!q.prepare(QStringLiteral("SELECT fe.id, fe.folderId, fe.title, fe.url, fe.link, fe.added, fe.unreadCount, fe.ordering, fe.pinned, fe.updateErrorCount, fe.lastUpdateError, fe.faviconLink, fo.name AS folderName FROM feeds fe LEFT JOIN folders fo ON fo.id = fe.folderId WHERE fe.id = ?"))) {
        //% "Failed to prepare database query."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-prepare-query"), this));
        return nullptr;
    }

    q.addBindValue(id);

    if (!q.exec()) {
        //% "Failed to execute database query."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query"), this));
        return nullptr;
    }


    if (q.next()) {
        Feed *f = new Feed(
                        q.value(0).toLongLong(),
                        q.value(1).toLongLong(),
                        q.value(2).toString(),
                        QUrl(q.value(3).toString()),
                        QUrl(q.value(4).toString()),
                        QDateTime::fromTime_t(q.value(5).toUInt()),
                        q.value(6).toUInt(),
                        (Feed::FeedOrdering)q.value(7).toInt(),
                        q.value(8).toBool(),
                        q.value(9).toUInt(),
                        q.value(10).toString(),
                        QUrl(q.value(11).toString()),
                        q.value(12).toString()
                        );
        return f;

    } else {

        qWarning("Can not find the the feed in the local SQLite database.");
        return nullptr;

    }
}



void SQLiteStorage::feedsRequested(const QJsonDocument &json)
{
    Q_D(SQLiteStorage);

    if (!ready()) {
        //% "SQLite database not ready. Can not process requested data."
        setError(new Error(Error::StorageError, Error::Warning, qtTrId("libfuoten-err-sqlite-db-not-ready"), QString(), this));
        return;
    }

    if (json.isEmpty() || json.isNull()) {
        return;
    }

    QSqlQuery q(d->db);

    const QJsonArray feeds = json.object().value(QStringLiteral("feeds")).toArray();

#ifdef QT_DEBUG
    qDebug() << "Processing" << feeds.size() << "feeds requested from the remote server.";
#endif

    QueryArgs qa;
    const QList<Feed*> currentFeeds = getFeeds(qa);

    IdList updatedFeedIds;
    IdList newFeedIds;
    IdList deletedFeedIds;

    if (feeds.isEmpty() && currentFeeds.isEmpty()) {

#ifdef QT_DEBUG
        qDebug() << "Nothing to do. Local feeds and remote feeds are empty.";
#endif

        Q_EMIT requestedFeeds(updatedFeedIds, newFeedIds, deletedFeedIds);
        return;

    } else if (feeds.isEmpty() && !currentFeeds.isEmpty()) {

#ifdef QT_DEBUG
        qDebug() << "All feeds have been deleted on the server. Deleting local ones.";
#endif

        deletedFeedIds.reserve(currentFeeds.size());
        for (Feed *f : currentFeeds) {
            deletedFeedIds.append(f->id());
        }

        if (!q.exec(QStringLiteral("DELETE FROM feeds"))) {
            //% "Failed to execute database query."
            setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query"), this));
            return;
        }

    } else if (!feeds.isEmpty() && currentFeeds.isEmpty()) {

#ifdef QT_DEBUG
        qDebug() << "No local feeds. Adding all requested feeds as new feeds.";
#endif

        if (!d->db.transaction()) {
            //% "Failed to begin a database transaction."
            setError(new Error(q.lastError(), qtTrId("fuoten-error-transaction-begin"), this));
            return;
        }

        for (const QJsonValue &f : feeds) {
            QJsonObject o = f.toObject();
            if (!o.isEmpty()) {
                newFeedIds.append(o.value(QStringLiteral("id")).toVariant().toLongLong());

                if (!q.prepare(QStringLiteral("INSERT INTO feeds (id, folderId, title, url, link, added, ordering, pinned, updateErrorCount, lastUpdateError, faviconLink) "
                                              "VALUES (?,?,?,?,?,?,?,?,?,?,?)"
                                              ))) {
                    //% "Failed to prepare database query."
                    setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-prepare-query"), this));
                    return;
                }

                q.addBindValue(o.value(QStringLiteral("id")).toVariant().toLongLong());
                q.addBindValue(o.value(QStringLiteral("folderId")).toVariant().toLongLong());
                q.addBindValue(o.value(QStringLiteral("title")).toString());
                q.addBindValue(o.value(QStringLiteral("url")).toString());
                q.addBindValue(o.value(QStringLiteral("link")).toString());
                q.addBindValue(o.value(QStringLiteral("added")).toVariant().toUInt());
                q.addBindValue(o.value(QStringLiteral("ordering")).toInt());
                q.addBindValue(o.value(QStringLiteral("pinned")).toBool());
                q.addBindValue(o.value(QStringLiteral("updateErrorCount")).toInt());
                q.addBindValue(o.value(QStringLiteral("lastUpdateError")).toString());
                q.addBindValue(o.value(QStringLiteral("faviconLink")).toString());


                if (!q.exec()) {
                    //% "Failed to execute database query."
                    setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query"), this));
                    return;
                }
            }
        }


        if (!d->db.commit()) {
            //% "Failed to commit a database transaction."
            setError(new Error(q.lastError(), qtTrId("fuoten-error-transaction-commit"), this));
            return;
        }


    } else {


#ifdef QT_DEBUG
        qDebug() << "Checking for updated, new and deleted feeds.";
#endif

        QHash<qint64, Feed*> cfh; // current feeds hash
        IdList requestedFeedIds;

        for (Feed *f : currentFeeds) {
            cfh.insert(f->id(), f);
        }

        if (!d->db.transaction()) {
            //% "Failed to begin a database transaction."
            setError(new Error(q.lastError(), qtTrId("fuoten-error-transaction-begin"), this));
            return;
        }

        for (const QJsonValue &f : feeds) {
            QJsonObject o = f.toObject();
            if (!o.isEmpty()) {
                qint64 id = o.value(QStringLiteral("id")).toVariant().toLongLong();
                requestedFeedIds.append(id);

                if (!cfh.contains(id)) {
                    newFeedIds.append(id);

#ifdef QT_DEBUG
                    qDebug() << "Adding new feed" << o.value(QStringLiteral("title")).toString() << "with ID" << id << "to the database.";
#endif

                    if (!q.prepare(QStringLiteral("INSERT INTO feeds (id, folderId, title, url, link, added, ordering, pinned, updateErrorCount, lastUpdateError, faviconLink) "
                                                  "VALUES (?,?,?,?,?,?,?,?,?,?,?)"
                                                  ))) {
                        //% "Failed to prepare database query."
                        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-prepare-query"), this));
                        return;
                    }

                    q.addBindValue(id);
                    q.addBindValue(o.value(QStringLiteral("folderId")).toVariant().toLongLong());
                    q.addBindValue(o.value(QStringLiteral("title")).toString());
                    q.addBindValue(o.value(QStringLiteral("url")).toString());
                    q.addBindValue(o.value(QStringLiteral("link")).toString());
                    q.addBindValue(o.value(QStringLiteral("added")).toVariant().toUInt());
                    q.addBindValue(o.value(QStringLiteral("ordering")).toInt());
                    q.addBindValue(o.value(QStringLiteral("pinned")).toBool());
                    q.addBindValue(o.value(QStringLiteral("updateErrorCount")).toInt());
                    q.addBindValue(o.value(QStringLiteral("lastUpdateError")).toString());
                    q.addBindValue(o.value(QStringLiteral("faviconLink")).toString());


                    if (!q.exec()) {
                        //% "Failed to execute database query."
                        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query"), this));
                        return;
                    }



                } else {

                    const QString rTitle = o.value(QStringLiteral("title")).toString();
                    const QUrl rFaviconLink = QUrl(o.value(QStringLiteral("faviconLink")).toString());
                    const qint64 rFolderId = o.value(QStringLiteral("folderId")).toVariant().toLongLong();
                    const Feed::FeedOrdering rOrdering = (Feed::FeedOrdering)o.value(QStringLiteral("ordering")).toInt();
                    const QUrl rLink = QUrl(o.value(QStringLiteral("link")).toString());
                    const bool rPinned = o.value(QStringLiteral("pinned")).toBool();
                    const uint rUpdateErrorCount = o.value(QStringLiteral("updateErrorCount")).toInt();
                    const QString rLastUpdateError = o.value(QStringLiteral("lastUpdateError")).toString();

                    Feed *f = cfh.value(id);

                    if ((f->title() != rTitle) || (f->faviconLink() != rFaviconLink) || (f->folderId() != rFolderId) || (f->ordering() != rOrdering) || (f->link() != rLink) || (f->pinned() != rPinned) || (f->updateErrorCount() != rUpdateErrorCount) || (f->lastUpdateError() != rLastUpdateError)) {

#ifdef QT_DEBUG
                        qDebug() << "Updating feed" << f->title() << "with ID" << id << "in the database";
#endif
                        updatedFeedIds.append(id);

                        if (!q.prepare(QStringLiteral("UPDATE feeds SET folderId = ?, title = ?, link = ?, ordering = ?, pinned = ?, updateErrorCount = ?, lastUpdateError = ?, faviconLink = ? WHERE id = ?"))) {
                            //% "Failed to prepare database query."
                            setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-prepare-query"), this));
                            return;
                        }

                        q.addBindValue(rFolderId);
                        q.addBindValue(rTitle);
                        q.addBindValue(rLink.toString());
                        q.addBindValue((int)rOrdering);
                        q.addBindValue(rPinned);
                        q.addBindValue(rUpdateErrorCount);
                        q.addBindValue(rLastUpdateError);
                        q.addBindValue(rFaviconLink.toString());
                        q.addBindValue(id);

                        if (!q.exec()) {
                            //% "Failed to execute database query."
                            setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query"), this));
                            return;
                        }
                    }
                }
            }
        }



        QHash<qint64, Feed*>::const_iterator i = cfh.constBegin();
        while (i != cfh.constEnd()) {
            if (!requestedFeedIds.contains(i.key())) {
                deletedFeedIds.append(i.key());
            }
            ++i;
        }

        if (!deletedFeedIds.isEmpty()) {
#ifdef QT_DEBUG
            qDebug() << "The feeds with the following IDS have been deleted on the server:" << deletedFeedIds;
#endif

            if (!q.exec(QStringLiteral("DELETE FROM feeds WHERE id IN (%1)").arg(d->intListToString(deletedFeedIds)))) {
                //% "Failed to execute database query."
                setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query"), this));
                return;
            }
        }



        if (!d->db.commit()) {
            //% "Failed to commit a database transaction."
            setError(new Error(q.lastError(), qtTrId("fuoten-error-transaction-commit"), this));
            return;
        }

    }



    Q_EMIT requestedFeeds(updatedFeedIds, newFeedIds, deletedFeedIds);
}



void SQLiteStorage::feedCreated(const QJsonDocument &json)
{
    if (!ready()) {
        //% "SQLite database not ready. Can not process requested data."
        setError(new Error(Error::StorageError, Error::Warning, qtTrId("libfuoten-err-sqlite-db-not-ready"), QString(), this));
        return;
    }

    if (json.isEmpty() || json.isNull()) {
        qWarning("Can not add folder to SQLite database. JSON data is not valid.");
        return;
    }

    QJsonArray a = json.object().value(QStringLiteral("feeds")).toArray();

    if (a.isEmpty()) {
        qWarning("Can not add feed to SQLite database. JSON array is empty.");
        return;
    }


    QJsonObject o = a.first().toObject();

    if (o.isEmpty()) {
        qWarning("Can not add feed to SQLite databse. JSON object is empty.");
        return;
    }

    Q_D(SQLiteStorage);

    QSqlQuery q(d->db);

    if (!q.prepare(QStringLiteral("INSERT INTO feeds (id, folderId, title, url, link, added, ordering, pinned, updateErrorCount, lastUpdateError, faviconLink) "
                                  "VALUES (?,?,?,?,?,?,?,?,?,?,?)"
                                  ))) {
        //% "Failed to prepare database query."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-prepare-query"), this));
        return;
    }

    qint64 id = o.value(QStringLiteral("id")).toVariant().toLongLong();
    qint64 folderId = o.value(QStringLiteral("folderId")).toVariant().toLongLong();

    q.addBindValue(id);
    q.addBindValue(folderId);
    q.addBindValue(o.value(QStringLiteral("title")).toString());
    q.addBindValue(o.value(QStringLiteral("url")).toString());
    q.addBindValue(o.value(QStringLiteral("link")).toString());
    q.addBindValue(o.value(QStringLiteral("added")).toVariant().toUInt());
    q.addBindValue(o.value(QStringLiteral("ordering")).toInt());
    q.addBindValue(o.value(QStringLiteral("pinned")).toBool());
    q.addBindValue(o.value(QStringLiteral("updateErrorCount")).toInt());
    q.addBindValue(o.value(QStringLiteral("lastUpdateError")).toString());
    q.addBindValue(o.value(QStringLiteral("faviconLink")).toString());


    if (!q.exec()) {
        //% "Failed to execute database query."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query"), this));
        return;
    }

    Q_EMIT createdFeed(id, folderId);
}



void SQLiteStorage::feedDeleted(qint64 id)
{
    if (!ready()) {
        //% "SQLite database not ready. Can not process requested data."
        setError(new Error(Error::StorageError, Error::Warning, qtTrId("libfuoten-err-sqlite-db-not-ready"), QString(), this));
        return;
    }

    if (id <= 0) {
        //% "The feed ID is not valid."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-invalid-feed-id"), QString(), this));
        return;
    }

    Q_D(SQLiteStorage);

    QSqlQuery q(d->db);

    if (!q.prepare(QStringLiteral("DELETE FROM feeds WHERE id = ?"))) {
        //% "Failed to prepare database query."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-prepare-query"), this));
        return;
    }

    q.addBindValue(id);

    if (!q.exec()) {
        //% "Failed to execute database query."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query"), this));
        return;
    }

    Q_EMIT deletedFeed(id);
}



void SQLiteStorage::feedMoved(qint64 id, qint64 targetFolder)
{
    if (!ready()) {
        //% "SQLite database not ready. Can not process requested data."
        setError(new Error(Error::StorageError, Error::Warning, qtTrId("libfuoten-err-sqlite-db-not-ready"), QString(), this));
        return;
    }

    if (id <= 0) {
        //% "The feed ID is not valid."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-invalid-feed-id"), QString(), this));
        return;
    }

    if (targetFolder < 0) {
        //% "The folder ID is not valid."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-invalid-folder-id"), QString(), this));
        return;
    }

    Q_D(SQLiteStorage);

    QSqlQuery q(d->db);

    if (!q.prepare(QStringLiteral("UPDATE feeds SET folderId = ? WHERE id = ?"))) {
        //% "Failed to prepare database query."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-prepare-query"), this));
        return;
    }

    q.addBindValue(targetFolder);
    q.addBindValue(id);

    if (!q.exec()) {
        //% "Failed to execute database query."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query"), this));
        return;
    }

    Q_EMIT movedFeed(id, targetFolder);
}


void SQLiteStorage::feedRenamed(qint64 id, const QString &newTitle)
{
    if (!ready()) {
        //% "SQLite database not ready. Can not process requested data."
        setError(new Error(Error::StorageError, Error::Warning, qtTrId("libfuoten-err-sqlite-db-not-ready"), QString(), this));
        return;
    }

    if (newTitle.isEmpty()) {
        //% "The feed name can not be empty."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-empty-feed-name"), QString(), this));
        return;
    }

    if (id <= 0) {
        //% "The feed ID is not valid."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-invalid-feed-id"), QString(), this));
        return;
    }


    Q_D(SQLiteStorage);

    QSqlQuery q(d->db);

    if (!q.prepare(QStringLiteral("UPDATE feeds SET title = ? WHERE id = ?"))) {
        //% "Failed to prepare database query."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-prepare-query"), this));
        return;
    }

    q.addBindValue(newTitle);
    q.addBindValue(id);

    if (!q.exec()) {
        //% "Failed to execute database query."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query"), this));
        return;
    }

    Q_EMIT renamedFeed(id, newTitle);
}



void SQLiteStorage::feedMarkedRead(qint64 id, qint64 newestItem)
{
    if (!ready()) {
        //% "SQLite database not ready. Can not process requested data."
        setError(new Error(Error::StorageError, Error::Warning, qtTrId("libfuoten-err-sqlite-db-not-ready"), QString(), this));
        return;
    }

    if (id <= 0) {
        //% "The feed ID is not valid."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-invalid-feed-id"), QString(), this));
        return;
    }

    if (newestItem <= 0) {
        //% "The item ID is not valid."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-invalid-item-id"), QString(), this));
        return;
    }

    Q_D(SQLiteStorage);

    QSqlQuery q(d->db);

    if (!q.prepare(QStringLiteral("UPDATE items SET unread = 0, lastModified = ? WHERE feedId = ? AND id <= ?"))) {
        //% "Failed to prepare database query."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-prepare-query"), this));
        return;
    }

    q.addBindValue(QDateTime::currentDateTimeUtc().toTime_t());
    q.addBindValue(id);
    q.addBindValue(newestItem);

    if (!q.exec()) {
        //% "Failed to execute database query."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query"), this));
        return;
    }

    if (!q.exec(QStringLiteral("SELECT COUNT(id) FROM items WHERE unread = 1"))) {
        //% "Failed to execute database query."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query"), this));
        return;
    }

    if (q.next()) {
        setTotalUnread(q.value(0).toInt());
    }

    Q_EMIT markedReadFeed(id, newestItem);
}





Article *SQLiteStorage::getArticle(qint64 id, int bodyLimit)
{
    if (!ready()) {
        qWarning("SQLite database not ready. Can not query article from database.");
        return nullptr;
    }

    Q_D(SQLiteStorage);

    QSqlQuery q(d->db);

    if (!q.prepare(QStringLiteral("SELECT it.id, it.feedId, fe.title, it.guid, it.guidHash, it.url, it.title, it.author, it.pubDate, it.body, it.enclosureMime, it.enclosureLink, it.unread, it.starred, it.lastModified, it.fingerprint, fo.id, fo.name, it.queue FROM items it LEFT JOIN feeds fe ON fe.id = it.feedId LEFT JOIN folders fo on fo.id = fe.folderId WHERE it.id = ?"))) {
        //% "Failed to prepare database query."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-prepare-query"), this));
        return nullptr;
    }

    q.addBindValue(id);

    if (!q.exec()) {
        //% "Failed to execute database query."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query"), this));
        return nullptr;
    }

    QString body;

    if (bodyLimit == 0) {
        body = q.value(9).toString();
    } else if (bodyLimit > 0) {
        body = limitBody(q.value(9).toString(), bodyLimit);
    }


    if (q.next()) {
        Article *a = new Article(q.value(0).toLongLong(),
                                 q.value(1).toLongLong(),
                                 q.value(2).toString(),
                                 q.value(3).toString(),
                                 q.value(4).toString(),
                                 QUrl(q.value(5).toString()),
                                 q.value(6).toString(),
                                 q.value(7).toString(),
                                 QDateTime::fromTime_t(q.value(8).toUInt()),
                                 body,
                                 q.value(10).toString(),
                                 QUrl(q.value(11).toString()),
                                 q.value(12).toBool(),
                                 q.value(13).toBool(),
                                 QDateTime::fromTime_t(q.value(14).toUInt()),
                                 q.value(15).toString(),
                                 q.value(16).toLongLong(),
                                 q.value(17).toString(),
                                 FuotenEnums::QueueActions(q.value(18).toInt())
                                 );
        return a;

    } else {

        qWarning("Can not find the the article in the local SQLite database.");
        return nullptr;

    }
}



QList<Article*> SQLiteStorage::getArticles(const QueryArgs &args)
{
    QList<Article*> articles;

    if (!ready()) {
        qWarning("SQLite database not ready. Can not query articles from database.");
        return articles;
    }

    Q_D(SQLiteStorage);

    QSqlQuery q(d->db);

    QString qs = QStringLiteral("SELECT it.id, it.feedId, fe.title, it.guid, it.guidHash, it.url, it.title, it.author, it.pubDate, it.body, it.enclosureMime, it.enclosureLink, it.unread, it.starred, it.lastModified, it.fingerprint, fo.id, fo.name, it.queue FROM items it LEFT JOIN feeds fe ON fe.id = it.feedId LEFT JOIN folders fo on fo.id = fe.folderId");

    if (!args.inIds.isEmpty() || args.unreadOnly || args.starredOnly || (args.parentId > -1) || args.queuedOnly) {
        qs.append(QLatin1String(" WHERE"));
    }

    if (args.parentId > -1) {
        if (args.parentIdType == FuotenEnums::Feed) {
            qs.append(QStringLiteral(" it.feedId = %1").arg(QString::number(args.parentId)));
        } else {
            qs.append(QStringLiteral(" it.feedId IN (SELECT id FROM feeds WHERE folderId = %1)").arg(args.parentId));
        }
    }

    if (!args.inIds.isEmpty()) {
        if (args.parentId > -1) {
            qs.append(QLatin1String(" AND"));
        }
        switch(args.inIdsType) {
        case FuotenEnums::Folder:
            qs.append(QStringLiteral(" it.feedId IN (SELECT id FROM feeds WHERE folderId IN (%1))").arg(d->intListToString(args.inIds)));
            break;
        case FuotenEnums::Feed:
            qs.append(QStringLiteral(" it.feedId IN (%1)").arg(d->intListToString(args.inIds)));
            break;
        default:
            qs.append(QStringLiteral(" it.id IN (%1)").arg(d->intListToString(args.inIds)));
            break;
        }
    }

    if (args.unreadOnly) {
        if ((args.parentId > -1) || !args.inIds.isEmpty()) {
            qs.append(QLatin1String(" AND"));
        }
        qs.append(QLatin1String(" it.unread = 1"));
    }

    if (args.starredOnly) {
        if ((args.parentId > -1) || !args.inIds.isEmpty() || args.unreadOnly) {
            qs.append(QLatin1String(" AND"));
        }
        qs.append(QLatin1String(" it.starred = 1"));
    }

    if (args.queuedOnly) {
        if ((args.parentId > -1) || !args.inIds.isEmpty() || args.unreadOnly || args.starredOnly) {
            qs.append(QLatin1String(" AND"));
        }
        qs.append(QLatin1String(" it.queue > 0"));
    }

    switch(args.sortingRole) {
    case FuotenEnums::Time:
        qs.append(QLatin1String(" ORDER BY it.pubDate"));
        break;
    case FuotenEnums::ID:
        qs.append(QLatin1String(" ORDER BY it.id"));
        break;
    case FuotenEnums::Name:
        qs.append(QLatin1String(" ORDER BY it.title"));
        break;
    case FuotenEnums::FolderName:
        qs.append(QLatin1String(" ORDER BY fo.name"));
        break;
    default:
        qs.append(QLatin1String(" ORDER BY it.pubDate"));
        break;
    }

    if (args.sortOrder== Qt::AscendingOrder) {
        qs.append(QLatin1String(" ASC"));
    } else {
        qs.append(QLatin1String(" DESC"));
    }

    if (args.limit > 0) {
        qs.append(QLatin1String(" LIMIT ")).append(QString::number(args.limit));
    }

#ifdef QT_DEBUG
    qDebug() << "Start to query articles from the local SQLite database using the following query:";
    qDebug() << qs;
#endif

    if (!q.exec(qs)) {
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query"), this));
        return articles;
    }

    while (q.next()) {
        QString body;

        if (args.bodyLimit > -1) {

            body = q.value(9).toString();

            if (args.bodyLimit > 0) {

                body.replace(QRegularExpression(QStringLiteral("<[^>]*>")), QStringLiteral(" "));
                body = body.simplified();
                body = body.left(args.bodyLimit);
            }

        }

        articles.append(new Article(q.value(0).toLongLong(),
                                 q.value(1).toLongLong(),
                                 q.value(2).toString(),
                                 q.value(3).toString(),
                                 q.value(4).toString(),
                                 QUrl(q.value(5).toString()),
                                 q.value(6).toString(),
                                 q.value(7).toString(),
                                 QDateTime::fromTime_t(q.value(8).toUInt()),
                                 body,
                                 q.value(10).toString(),
                                 QUrl(q.value(11).toString()),
                                 q.value(12).toBool(),
                                 q.value(13).toBool(),
                                 QDateTime::fromTime_t(q.value(14).toUInt()),
                                 q.value(15).toString(),
                                 q.value(16).toLongLong(),
                                 q.value(17).toString(),
                                 FuotenEnums::QueueActions(q.value(18).toInt())
                                 ));
    }

    return articles;
}



GetArticlesAsyncWorker::GetArticlesAsyncWorker(const QString &dbpath, const QueryArgs &args, QObject *parent) :
    QThread(parent), m_args(args)
{
    if (!QSqlDatabase::connectionNames().contains(QStringLiteral("fuotendb"))) {
        m_db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), QStringLiteral("fuotendb"));
        m_db.setDatabaseName(dbpath);
    } else {
        m_db = QSqlDatabase::database(QStringLiteral("fuotendb"));
    }
}



void GetArticlesAsyncWorker::run()
{
    QList<Article*> articles;

    QSqlQuery q(m_db);

    if (!q.exec(QStringLiteral("PRAGMA foreign_keys = ON"))) {
        //% "Failed to execute database query."
        Q_EMIT failed(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query")));
        Q_EMIT gotArticles(articles);
        return;
    }

    QString qs = QStringLiteral("SELECT it.id, it.feedId, fe.title, it.guid, it.guidHash, it.url, it.title, it.author, it.pubDate, it.body, it.enclosureMime, it.enclosureLink, it.unread, it.starred, it.lastModified, it.fingerprint, fo.id, fo.name, it.queue FROM items it LEFT JOIN feeds fe ON fe.id = it.feedId LEFT JOIN folders fo on fo.id = fe.folderId");

    if (!m_args.inIds.isEmpty() || m_args.unreadOnly || m_args.starredOnly || (m_args.parentId > -1) || m_args.queuedOnly) {
        qs.append(QLatin1String(" WHERE"));
    }

    if (m_args.parentId > -1) {
        if (m_args.parentIdType == FuotenEnums::Feed) {
            qs.append(QStringLiteral(" it.feedId = %1").arg(QString::number(m_args.parentId)));
        } else {
            qs.append(QStringLiteral(" it.feedId IN (SELECT id FROM feeds WHERE folderId = %1)").arg(m_args.parentId));
        }
    }

    if (!m_args.inIds.isEmpty()) {
        if (m_args.parentId > -1) {
            qs.append(QLatin1String(" AND"));
        }

        QString idListString;
        if (m_args.inIds.count() == 1) {
            idListString = QString::number(m_args.inIds.first());
        } else {
            QStringList sl;
            const IdList ids = m_args.inIds;
            for (qint64 id : ids) {
                sl.append(QString::number(id));
            }
            idListString = sl.join(QChar(','));
        }


        switch(m_args.inIdsType) {
        case FuotenEnums::Folder:
            qs.append(QStringLiteral(" it.feedId IN (SELECT id FROM feeds WHERE folderId IN (%1))").arg(idListString));
            break;
        case FuotenEnums::Feed:
            qs.append(QStringLiteral(" it.feedId IN (%1)").arg(idListString));
            break;
        default:
            qs.append(QStringLiteral(" it.id IN (%1)").arg(idListString));
            break;
        }
    }

    if (m_args.unreadOnly) {
        if ((m_args.parentId > -1) || !m_args.inIds.isEmpty()) {
            qs.append(QLatin1String(" AND"));
        }
        qs.append(QLatin1String(" it.unread = 1"));
    }

    if (m_args.starredOnly) {
        if ((m_args.parentId > -1) || !m_args.inIds.isEmpty() || m_args.unreadOnly) {
            qs.append(QLatin1String(" AND"));
        }
        qs.append(QLatin1String(" it.starred = 1"));
    }

    if (m_args.queuedOnly) {
        if ((m_args.parentId > -1) || !m_args.inIds.isEmpty() || m_args.unreadOnly || m_args.starredOnly) {
            qs.append(QLatin1String(" AND"));
        }
        qs.append(QStringLiteral(" it.queue > 0"));
    }

    switch(m_args.sortingRole) {
    case FuotenEnums::Time:
        qs.append(QLatin1String(" ORDER BY it.pubDate"));
        break;
    case FuotenEnums::ID:
        qs.append(QLatin1String(" ORDER BY it.id"));
        break;
    case FuotenEnums::Name:
        qs.append(QLatin1String(" ORDER BY it.title"));
        break;
    case FuotenEnums::FolderName:
        qs.append(QLatin1String(" ORDER BY fo.name"));
        break;
    default:
        qs.append(QLatin1String(" ORDER BY it.pubDate"));
        break;
    }

    if (m_args.sortOrder== Qt::AscendingOrder) {
        qs.append(QLatin1String(" ASC"));
    } else {
        qs.append(QLatin1String(" DESC"));
    }

    if (m_args.limit > 0) {
        qs.append(QLatin1String(" LIMIT ")).append(QString::number(m_args.limit));
    }

#ifdef QT_DEBUG
    qDebug() << "Start to query articles from the local SQLite database using the following query:";
    qDebug() << qs;
#endif

    if (!q.exec(qs)) {
        Q_EMIT failed(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query")));
        Q_EMIT gotArticles(articles);
        return;
    }

    while (q.next()) {
        QString body;

        if (m_args.bodyLimit > -1) {

            body = q.value(9).toString();

            if (m_args.bodyLimit > 0) {

                body.replace(QRegularExpression(QStringLiteral("<[^>]*>")), QStringLiteral(" "));
                body = body.simplified();
                body = body.left(m_args.bodyLimit);
            }

        }

        articles.append(new Article(q.value(0).toLongLong(),
                                 q.value(1).toLongLong(),
                                 q.value(2).toString(),
                                 q.value(3).toString(),
                                 q.value(4).toString(),
                                 QUrl(q.value(5).toString()),
                                 q.value(6).toString(),
                                 q.value(7).toString(),
                                 QDateTime::fromTime_t(q.value(8).toUInt()),
                                 body,
                                 q.value(10).toString(),
                                 QUrl(q.value(11).toString()),
                                 q.value(12).toBool(),
                                 q.value(13).toBool(),
                                 QDateTime::fromTime_t(q.value(14).toUInt()),
                                 q.value(15).toString(),
                                 q.value(16).toLongLong(),
                                 q.value(17).toString(),
                                 FuotenEnums::QueueActions(q.value(18).toInt())
                                 ));
    }

    Q_EMIT gotArticles(articles);
}



void SQLiteStorage::getArticlesAsync(const QueryArgs &args)
{
    if (!ready()) {
        qWarning("SQLite database not ready. Can not query articles from database.");
        Q_EMIT gotArticlesAsync(QList<Article*>());
        return;
    }

    Q_D(SQLiteStorage);

    GetArticlesAsyncWorker *worker = new GetArticlesAsyncWorker(d->db.databaseName(), args, this);
    connect(worker, &GetArticlesAsyncWorker::gotArticles, this, &AbstractStorage::gotArticlesAsync);
    connect(worker, &GetArticlesAsyncWorker::failed, [=] (Error *e) {setError(e);});
    connect(worker, &QThread::finished, worker, &QObject::deleteLater);
    worker->start();

}



ItemsRequestedWorker::ItemsRequestedWorker(const QString &dbpath, const QJsonDocument &json, QObject *parent) :
    QThread(parent), m_json(json)
{
    if (!QSqlDatabase::connectionNames().contains(QStringLiteral("fuotendb"))) {
        m_db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), QStringLiteral("fuotendb"));
        m_db.setDatabaseName(dbpath);
    } else {
        m_db = QSqlDatabase::database(QStringLiteral("fuotendb"));
    }
}



void ItemsRequestedWorker::run()
{
    QSqlQuery q(m_db);

    if (!q.exec(QStringLiteral("PRAGMA foreign_keys = ON"))) {
        //% "Failed to execute database query."
        Q_EMIT failed(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query")));
        return;
    }


    const QJsonArray items = m_json.object().value(QStringLiteral("items")).toArray();

    IdList updatedItemIds;
    IdList newItemIds;
    IdList removedItemIds;

    if (items.isEmpty()) {
        Q_EMIT requestedItems(updatedItemIds, newItemIds, removedItemIds);
#ifdef QT_DEBUG
        qDebug() << "Nothing to do, no items.";
#endif
        return;
    }

    QHash<qint64, uint> currentItems; // contains the ids and last modified time stamps of the local items

    if (!q.exec(QStringLiteral("SELECT id, lastModified FROM items"))) {
        //% "Failed to execute database query."
        Q_EMIT failed(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query")));
        return;
    }

    while(q.next()) {
        currentItems.insert(q.value(0).toLongLong(), q.value(1).toUInt());
    }



    if (!m_db.transaction()) {
        //% "Failed to begin a database transaction."
        Q_EMIT failed(new Error(q.lastError(), qtTrId("fuoten-error-transaction-begin")));
        return;
    }

    for (const QJsonValue &i : items) {
        QJsonObject o = i.toObject();
        if (!o.isEmpty()) {
            qint64 id = o.value(QStringLiteral("id")).toVariant().toLongLong();

            if (!currentItems.isEmpty() && currentItems.contains(id)) {

                uint lastMod = o.value(QStringLiteral("lastModified")).toInt();

                if (currentItems.value(id) < lastMod) {

                    updatedItemIds.append(id);


#ifdef QT_DEBUG
                    qDebug() << "Updating the article" << o.value(QStringLiteral("title")).toString() << "with ID" << id << "in the database.";
#endif


                    if (!q.prepare(QStringLiteral("UPDATE items SET "
                                                  "title = ?, "
                                                  "url = ?, "
                                                  "author = ?, "
                                                  "pubDate = ?, "
                                                  "enclosureMime = ?, "
                                                  "enclosureLink = ?, "
                                                  "unread = ?, "
                                                  "starred = ?, "
                                                  "lastModified = ?,"
                                                  "fingerprint = ?, "
                                                  "queue = 0 "
                                                  "WHERE id = ?"
                                                  ))) {
                        //% "Failed to prepare database query."
                        Q_EMIT failed(new Error(q.lastError(), qtTrId("fuoten-error-failed-prepare-query")));
                        return;
                    }

                    q.addBindValue(o.value(QStringLiteral("title")).toString());
                    q.addBindValue(o.value(QStringLiteral("url")).toString());
                    q.addBindValue(o.value(QStringLiteral("author")).toString());
                    q.addBindValue(o.value(QStringLiteral("pubDate")).toInt());
                    q.addBindValue(o.value(QStringLiteral("enclosureMime")).toString());
                    q.addBindValue(o.value(QStringLiteral("enclosureLink")).toString());
                    q.addBindValue(o.value(QStringLiteral("unread")).toBool());
                    q.addBindValue(o.value(QStringLiteral("starred")).toBool());
                    q.addBindValue(lastMod);
                    q.addBindValue(o.value(QStringLiteral("fingerprint")).toString());
                    q.addBindValue(id);

                    if (!q.exec()) {
                        //% "Failed to execute database query."
                        Q_EMIT failed(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query")));
                        return;
                    }
                }

            } else {

                newItemIds.append(id);

#ifdef QT_DEBUG
                qDebug() << "Adding a new article" << o.value(QStringLiteral("title")).toString() << "with ID" << id << "to the database.";
#endif

                if (!q.prepare(QStringLiteral("INSERT INTO items (id, feedId, guid, guidHash, url, title, author, pubDate, body, enclosureMime, enclosureLink, unread, starred, lastModified, fingerprint) "
                                              "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"
                                              ))) {
                    //% "Failed to prepare database query."
                    Q_EMIT failed(new Error(q.lastError(), qtTrId("fuoten-error-failed-prepare-query")));
                    return;
                }

                q.addBindValue(id);
                q.addBindValue(o.value(QStringLiteral("feedId")).toVariant().toLongLong());
                q.addBindValue(o.value(QStringLiteral("guid")).toString());
                q.addBindValue(o.value(QStringLiteral("guidHash")).toString());
                q.addBindValue(o.value(QStringLiteral("url")).toString());
                q.addBindValue(o.value(QStringLiteral("title")).toString());
                q.addBindValue(o.value(QStringLiteral("author")).toString());
                q.addBindValue(o.value(QStringLiteral("pubDate")).toInt());
                q.addBindValue(o.value(QStringLiteral("body")).toString());
                q.addBindValue(o.value(QStringLiteral("enclosureMime")).toString());
                q.addBindValue(o.value(QStringLiteral("enclosureLink")).toString());
                q.addBindValue(o.value(QStringLiteral("unread")).toBool());
                q.addBindValue(o.value(QStringLiteral("starred")).toBool());
                q.addBindValue(o.value(QStringLiteral("lastModified")).toInt());
                q.addBindValue(o.value(QStringLiteral("fingerprint")).toString());

                if (!q.exec()) {
                    //% "Failed to execute database query."
                    Q_EMIT failed(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query")));
                    return;
                }
            }
        }
    }

    if (!m_db.commit()) {
        //% "Failed to commit a database transaction."
        Q_EMIT failed(new Error(q.lastError(), qtTrId("fuoten-error-transaction-commit")));
        return;
    }

    if (!q.exec(QStringLiteral("SELECT COUNT(id) FROM items WHERE unread = 1"))) {
        //% "Failed to execute database query."
        Q_EMIT failed(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query")));
        return;
    }

    if (q.next()) {
        Q_EMIT gotTotalUnread(q.value(0).toUInt());
    }

    if (!q.exec(QStringLiteral("SELECT COUNT(id) FROM items WHERE starred = 1"))) {
        //% "Failed to execute database query."
        Q_EMIT failed(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query")));
        return;
    }

    if (q.next()) {
        Q_EMIT gotStarred(q.value(0).toUInt());
    }

    Q_EMIT requestedItems(updatedItemIds, newItemIds, removedItemIds);
}




void SQLiteStorage::itemsRequested(const QJsonDocument &json)
{
    Q_D(SQLiteStorage);

    if (!ready()) {
        //% "SQLite database not ready. Can not process requested data."
        setError(new Error(Error::StorageError, Error::Warning, qtTrId("libfuoten-err-sqlite-db-not-ready"), QString(), this));
        return;
    }

    if (json.isEmpty() || json.isNull()) {
        Q_EMIT requestedItems(IdList(), IdList(), IdList());
        return;
    }

    ItemsRequestedWorker *worker = new ItemsRequestedWorker(d->db.databaseName(), json, this);
    connect(worker, &ItemsRequestedWorker::requestedItems, this, &SQLiteStorage::requestedItems);
    connect(worker, &ItemsRequestedWorker::gotStarred, this, &SQLiteStorage::setStarred);
    connect(worker, &ItemsRequestedWorker::gotTotalUnread, this, &SQLiteStorage::setTotalUnread);
    connect(worker, &ItemsRequestedWorker::failed, [=] (Error *e) {setError(e);});
    connect(worker, &QThread::finished, worker, &QObject::deleteLater);
    worker->start();
}



void SQLiteStorage::itemsMarked(const IdList &itemIds, bool unread)
{
    if (!ready()) {
        //% "SQLite database not ready. Can not process requested data."
        setError(new Error(Error::StorageError, Error::Warning, qtTrId("libfuoten-err-sqlite-db-not-ready"), QString(), this));
        return;
    }

    if (itemIds.isEmpty()) {
        qWarning("List of marked articles is empty.");
        return;
    }

    Q_D(SQLiteStorage);

    QSqlQuery q(d->db);

    if (!q.prepare(QStringLiteral("UPDATE items SET unread = ?, lastModified = ? WHERE id IN (%1)").arg(d->intListToString(itemIds)))) {
        //% "Failed to prepare database query."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-prepare-query"), this));
        return;
    }

    q.addBindValue(unread);
    q.addBindValue(QDateTime::currentDateTimeUtc().toTime_t());

    if (!q.exec()) {
        //% "Failed to execute database query."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query"), this));
        return;
    }

    if (unread) {
        setTotalUnread(totalUnread() + itemIds.count());
    } else {
        setTotalUnread(totalUnread() - itemIds.count());
    }

    Q_EMIT markedItems(itemIds, unread);
}


void SQLiteStorage::itemsStarred(const QList<QPair<qint64, QString> > &articles, bool star)
{
    if (!ready()) {
        //% "SQLite database not ready. Can not process requested data."
        setError(new Error(Error::StorageError, Error::Warning, qtTrId("libfuoten-err-sqlite-db-not-ready"), QString(), this));
        return;
    }

    if (articles.isEmpty()) {
        qWarning("No articles in the list. Can not update local storage.");
    }

    Q_D(SQLiteStorage);

    QSqlQuery q(d->db);

    if (!d->db.transaction()) {
        //% "Failed to begin a database transaction."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-transaction-begin"), this));
        return;
    }

    for (const QPair<qint64,QString> &p : articles) {

        if (!q.prepare(QStringLiteral("UPDATE items SET starred = ?, lastModified = ? WHERE feedId = ? and guidHash = ?"))) {
            //% "Failed to prepare database query."
            setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-prepare-query"), this));
            return;
        }

        q.addBindValue(star);
        q.addBindValue(QDateTime::currentDateTimeUtc().toTime_t());
        q.addBindValue(p.first);
        q.addBindValue(p.second);

        if (!q.exec()) {
            //% "Failed to execute database query."
            setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query"), this));
            return;
        }

    }

    if (!d->db.commit()) {
        //% "Failed to commit a database transaction."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-transaction-commit"), this));
        return;
    }

    if (star) {
        setStarred(starred() + articles.count());
    } else {
        setStarred(starred() - articles.count());
    }

    Q_EMIT starredItems(articles, star);
}


void SQLiteStorage::itemMarked(qint64 itemId, bool unread)
{
    if (!ready()) {
        //% "SQLite database not ready. Can not process requested data."
        setError(new Error(Error::StorageError, Error::Warning, qtTrId("libfuoten-err-sqlite-db-not-ready"), QString(), this));
        return;
    }

    Q_D(SQLiteStorage);

    QSqlQuery q(d->db);

    if (!q.prepare(QStringLiteral("UPDATE items SET unread = ?, lastModified = ? WHERE id = ?"))) {
        //% "Failed to prepare database query."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-prepare-query"), this));
        return;
    }

    q.addBindValue(unread);
    q.addBindValue(QDateTime::currentDateTimeUtc().toTime_t());
    q.addBindValue(itemId);

    if (!q.exec()) {
        //% "Failed to execute database query."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query"), this));
        return;
    }

    if (unread) {
        setTotalUnread(totalUnread()+1);
    } else {
        setTotalUnread(totalUnread()-1);
    }

    Q_EMIT markedItem(itemId, unread);
}



void SQLiteStorage::itemStarred(qint64 feedId, const QString &guidHash, bool star)
{
    if (!ready()) {
        //% "SQLite database not ready. Can not process requested data."
        setError(new Error(Error::StorageError, Error::Warning, qtTrId("libfuoten-err-sqlite-db-not-ready"), QString(), this));
        return;
    }

    Q_D(SQLiteStorage);

    QSqlQuery q(d->db);

    if (!q.prepare(QStringLiteral("UPDATE items SET starred = ?, lastModified = ? WHERE feedId = ? and guidHash = ?"))) {
        //% "Failed to prepare database query."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-prepare-query"), this));
        return;
    }

    q.addBindValue(star);
    q.addBindValue(QDateTime::currentDateTimeUtc().toTime_t());
    q.addBindValue(feedId);
    q.addBindValue(guidHash);

    if (!q.exec()) {
        //% "Failed to execute database query."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query"), this));
        return;
    }

    if (star) {
        setStarred(starred() + 1);
    } else {
        setStarred(starred() - 1);
    }

    Q_EMIT starredItem(feedId, guidHash, star);
}


void SQLiteStorage::allItemsMarkedRead(qint64 newestItemId)
{
    if (!ready()) {
        //% "SQLite database not ready. Can not process requested data."
        setError(new Error(Error::StorageError, Error::Warning, qtTrId("libfuoten-err-sqlite-db-not-ready"), QString(), this));
        return;
    }

    Q_D(SQLiteStorage);

    QSqlQuery q(d->db);

    if (!q.prepare(QStringLiteral("UPDATE items SET unread = 0, lastModified = ? WHERE id <= ?"))) {
        //% "Failed to prepare database query."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-prepare-query"), this));
        return;
    }

    q.addBindValue(newestItemId);

    if (!q.exec()) {
        //% "Failed to execute database query."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query"), this));
        return;
    }

    setTotalUnread(0);

    Q_EMIT markedAllItemsRead(newestItemId);
}



QString SQLiteStorage::getArticleBody(qint64 id)
{
    if (!ready()) {
        //% "SQLite database not ready. Can not process requested data."
        setError(new Error(Error::StorageError, Error::Warning, qtTrId("libfuoten-err-sqlite-db-not-ready"), QString(), this));
        return QString();
    }

    Q_D(SQLiteStorage);

    QSqlQuery q(d->db);

    if (!q.prepare(QStringLiteral("SELECT body FROM items WHERE id = ?"))) {
        //% "Failed to prepare database query."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-prepare-query"), this));
        return QString();
    }

    q.addBindValue(id);

    if (!q.exec()) {
        //% "Failed to execute database query."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query"), this));
        return QString();
    }

    if (q.next()) {
        return q.value(0).toString();
    } else {
        return QString();
    }
}



bool SQLiteStorage::enqueueItem(FuotenEnums::QueueAction action, Article *article)
{
    if (!ready()) {
        //% "SQLite database not ready. Can not process requested data."
        setError(new Error(Error::StorageError, Error::Warning, qtTrId("libfuoten-err-sqlite-db-not-ready"), QString(), this));
        return false;
    }

    if (!article) {
        qWarning("Invalid article object.");
        return false;
    }

    FuotenEnums::QueueActions aq = article->queue();

    QString qs(QStringLiteral("UPDATE items SET lastModified = ?, "));

    switch (action) {
    case FuotenEnums::MarkAsRead:
        if (aq.testFlag(FuotenEnums::MarkAsUnread)) {
            aq ^= FuotenEnums::MarkAsUnread;
        } else {
            aq |= FuotenEnums::MarkAsRead;
        }
        qs.append(QLatin1String("unread = 0, "));
        break;
    case FuotenEnums::MarkAsUnread:
        if (aq.testFlag(FuotenEnums::MarkAsRead)) {
            aq ^= FuotenEnums::MarkAsRead;
        } else {
            aq |= FuotenEnums::MarkAsUnread;
        }
        qs.append(QLatin1String("unread = 1, "));
        break;
    case FuotenEnums::Star:
        if (aq.testFlag(FuotenEnums::Unstar)) {
            aq ^= FuotenEnums::Unstar;
        } else {
            aq |= FuotenEnums::Star;
        }
        qs.append(QLatin1String("starred = 1, "));
        break;
    case FuotenEnums::Unstar:
        if (aq.testFlag(FuotenEnums::Star)) {
            aq ^= FuotenEnums::Star;
        } else {
            aq |= FuotenEnums::Unstar;
        }
        qs.append(QLatin1String("starred = 0, "));
        break;
    default:
        qWarning("Invalid queue action.");
        return false;
    }

    qs.append(QLatin1String("queue = ? "));

    if ((action == FuotenEnums::MarkAsUnread) || (action == FuotenEnums::MarkAsRead)) {
        qs.append(QLatin1String("WHERE id = ?"));
    } else {
        qs.append(QLatin1String("WHERE feedId = ? AND guidHash = ?"));
    }

    Q_D(SQLiteStorage);

    QSqlQuery q(d->db);

    if (!q.prepare(qs)) {
        //% "Failed to prepare database query."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-prepare-query"), this));
        return false;
    }

    q.addBindValue(QDateTime::currentDateTimeUtc().toTime_t()-10);
    q.addBindValue((int)aq);

    if ((action == FuotenEnums::MarkAsUnread) || (action == FuotenEnums::MarkAsRead)) {
        q.addBindValue(article->id());
    } else {
        q.addBindValue(article->feedId());
        q.addBindValue(article->guidHash());
    }

    if (!q.exec()) {
        //% "Failed to execute database query."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query"), this));
        return false;
    }

    article->setQueue(aq);

    switch (action) {
    case FuotenEnums::MarkAsRead:
        Q_EMIT markedItem(article->id(), false);
        setTotalUnread(totalUnread()-1);
        break;
    case FuotenEnums::MarkAsUnread:
        Q_EMIT markedItem(article->id(), true);
        setTotalUnread(totalUnread()+1);
        break;
    case FuotenEnums::Star:
        Q_EMIT starredItem(article->feedId(), article->guidHash(), true);
        setStarred(starred()+1);
        break;
    case FuotenEnums::Unstar:
        Q_EMIT starredItem(article->feedId(), article->guidHash(), false);
        setStarred(starred()-1);
        break;
    default:
        qWarning("Invalid queue action.");
        return false;
    }

    return true;
}



bool SQLiteStorage::enqueueMarkFeedRead(qint64 feedId, qint64 newestItemId)
{
    if (!ready()) {
        //% "SQLite database not ready. Can not process requested data."
        setError(new Error(Error::StorageError, Error::Warning, qtTrId("libfuoten-err-sqlite-db-not-ready"), QString(), this));
        return false;
    }

    if (feedId <= 0) {
        //% "The feed ID is not valid."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-invalid-feed-id"), QString(), this));
        return false;
    }

    if (newestItemId <= 0) {
        //% "The item ID is not valid."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-invalid-item-id"), QString(), this));
        return false;
    }

    Q_D(SQLiteStorage);

    QSqlQuery q = d->getQuery();

    if (!q.prepare(QStringLiteral("SELECT id, queue FROM items WHERE unread = 1 AND feedId = ? AND id <= ?"))) {
        //% "Failed to prepare database query."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-prepare-query"), this));
        return false;
    }

    q.addBindValue(feedId);
    q.addBindValue(newestItemId);

    if (!q.exec()) {
        //% "Failed to execute database query."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query"), this));
        return false;
    }

    QHash<qint64,FuotenEnums::QueueActions> idsAndQueue;

    while (q.next()) {
        idsAndQueue.insert(q.value(0).toLongLong(), FuotenEnums::QueueActions(q.value(1).toInt()));
    }

    if (idsAndQueue.isEmpty()) {
        qWarning("No items found.");
        return true;
    }

    QHash<qint64,FuotenEnums::QueueActions> idsAndQueueUpdated;
    QHash<qint64,FuotenEnums::QueueActions>::const_iterator i = idsAndQueue.constBegin();
    while (i != idsAndQueue.constEnd()) {
        FuotenEnums::QueueActions qa = i.value();
        if (qa.testFlag(FuotenEnums::MarkAsUnread)) {
            qa ^= FuotenEnums::MarkAsUnread;
        } else {
            qa |= FuotenEnums::MarkAsRead;
        }
        idsAndQueueUpdated.insert(i.key(), qa);
        ++i;
    }

    if (!d->db.transaction()) {
        //% "Failed to begin a database transaction."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-transaction-begin"), this));
        return false;
    }

    QHash<qint64,FuotenEnums::QueueActions>::const_iterator ii = idsAndQueueUpdated.constBegin();
    while (ii != idsAndQueueUpdated.constEnd()) {

        if (!q.prepare(QStringLiteral("UPDATE items SET unread = 0, queue = ? WHERE id = ?"))) {
            //% "Failed to prepare database query."
            setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-prepare-query"), this));
            return false;
        }

        q.addBindValue((int)ii.value());
        q.addBindValue(ii.key());

        if (!q.exec()) {
            //% "Failed to execute database query."
            setError(new Error(q.lastError(), qtTrId("fuoten-error-failed-execute-query"), this));
            return false;
        }

        ++ii;
    }


    if (!d->db.commit()) {
        //% "Failed to commit a database transaction."
        setError(new Error(q.lastError(), qtTrId("fuoten-error-transaction-commit"), this));
        return false;
    }

    setTotalUnread(totalUnread() - idsAndQueueUpdated.size());

    Q_EMIT markedReadFeedInQueue(feedId, newestItemId);

    return true;
}
