/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016-2020 Hüssenbergnetz/Matthias Fehring
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

#include "sqlitestorage_p.h"
#include <cmath>
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

using namespace Fuoten;

#define SEL_TOTAL_UNREAD "SELECT * FROM total_unread"
#define SEL_TOTAL_STARRED "SELECT * FROM total_starred"


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


SQLiteStorageManager::~SQLiteStorageManager()
{

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
    bool result = m_db.open();
    Q_ASSERT_X(result, "init database", "failed to open database");

    qDebug("%s", "Start checking database scheme.");

    QSqlQuery q(m_db);
    result = q.exec(QStringLiteral("PRAGMA foreign_keys = ON"));
    Q_ASSERT_X(result, "init database", "failed to activate foreign keys");

    result = q.exec(QStringLiteral("CREATE TABLE IF NOT EXISTS system "
                                   "(id INTEGER PRIMARY KEY NOT NULL, "
                                   "key TEXT NOT NULL, "
                                   "value TEXT NO NULL)"
                                   ));
    Q_ASSERT_X(result, "init database", "failed to create system table");

    result = q.exec(QStringLiteral("CREATE TABLE IF NOT EXISTS folders "
                                   "(id INTEGER PRIMARY KEY NOT NULL, "
                                   "name TEXT NOT NULL, "
                                   "unreadCount INTEGER DEFAULT 0, "
                                   "feedCount INTEGER DEFAULT 0)"
                                   ));
    Q_ASSERT_X(result, "init database", "failed to create folders table");

    result = q.exec(QStringLiteral("INSERT OR IGNORE INTO folders (id, name) VALUES (0, '')"));
    Q_ASSERT_X(result, "init database", "failed to inser base folder into database");


    result = q.exec(QStringLiteral("CREATE TABLE IF NOT EXISTS feeds "
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
                                   ));
    Q_ASSERT_X(result, "init database", "failed to create feeds table");

    result = q.exec(QStringLiteral("CREATE TABLE IF NOT EXISTS items "
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
                                   ));
    Q_ASSERT_X(result, "init database", "failed to create items table");

    result = q.exec(QStringLiteral("CREATE INDEX IF NOT EXISTS feeds_folder_id_index ON feeds (folderId)"));
    Q_ASSERT_X(result, "init database", "failed to create feeds_folder_id_index");

    result = q.exec(QStringLiteral("CREATE INDEX IF NOT EXISTS items_item_guid ON items (guidHash, feedId)"));
    Q_ASSERT_X(result, "init database", "failed to create items_item_guid index");

    result = q.exec(QStringLiteral("CREATE INDEX IF NOT EXISTS items_feed_id_index ON items (feedId)"));
    Q_ASSERT_X(result, "init database", "failed to create items_feed_id_index");

    for (const QString &trigger : {QStringLiteral("feeds_unreadCount_update_item"), QStringLiteral("feeds_unreadCount_delete_item"), QStringLiteral("feeds_unreadCount_insert_item"), QStringLiteral("folders_unreadCount_update_feed"), QStringLiteral("folders_counts_delete_feed"), QStringLiteral("folders_counts_insert_feed"), QStringLiteral("folders_counts_move_feed")}) {
        result = q.exec(QStringLiteral("DROP TRIGGER IF EXISTS %1").arg(trigger));
        Q_ASSERT_X(result, "init database", "failed to drop obsolete trigger");
    }

    result = q.exec(QStringLiteral("CREATE VIEW IF NOT EXISTS total_unread AS SELECT COUNT(id) FROM items WHERE unread = 1"));
    Q_ASSERT_X(result, "init database", "failed to create total_unread view");

    result = q.exec(QStringLiteral("CREATE VIEW IF NOT EXISTS total_starred AS SELECT COUNT(id) FROM items WHERE starred = 1"));
    Q_ASSERT_X(result, "init database", "failed to create total_starred view");

    result = q.exec(QStringLiteral("SELECT value FROM system WHERE key = 'schema_version'"));
    Q_ASSERT_X(result, "init database", "failed to query installed schema version");

    if (Q_LIKELY(q.next())) {
        m_currentDbVersion = static_cast<quint16>(q.value(0).toUInt());
    }

    if (Q_UNLIKELY(m_currentDbVersion < 1)) {
        result = q.exec(QStringLiteral("INSERT INTO system (key, value) VALUES ('schema_version', '1')"));
        Q_ASSERT_X(result, "init database", "failed to insert schema version into database");
        m_currentDbVersion = 1;
    }

    if (m_currentDbVersion < 2) {
        qDebug("%s", "Performing database schema upgrade to version 2.");

        result = q.exec(QStringLiteral("ALTER TABLE items ADD COLUMN rtl INTEGER NOT NULL DEFAULT 0"));
        Q_ASSERT_X(result, "init databse", "failed to add column rtl to table items");

        result = q.exec(QStringLiteral("ALTER TABLE items ADD COLUMN mediaThumbnail TEXT"));
        Q_ASSERT_X(result, "init databse", "failed to add column mediaThumbnail to table items");

        result = q.exec(QStringLiteral("ALTER TABLE items ADD COLUMN mediaDescription TEXT"));
        Q_ASSERT_X(result, "init databse", "failed to add column mediaDescription to table items");

        result = q.exec(QStringLiteral("UPDATE system SET value = '2' WHERE key = 'schema_version'"));
        Q_ASSERT_X(result, "init database", "failed to update schema version in database");

        m_currentDbVersion = 2;
    }

    Q_EMIT succeeded();

    qDebug("%s", "Finished checking database scheme.");
}


SQLiteStoragePrivate::SQLiteStoragePrivate(const QString &_dbpath) : AbstractStoragePrivate()
{
    if (!QSqlDatabase::connectionNames().contains(QStringLiteral("fuotendb"))) {
        db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), QStringLiteral("fuotendb"));
        db.setDatabaseName(_dbpath);
    } else {
        db = QSqlDatabase::database(QStringLiteral("fuotendb"));
    }
}


SQLiteStoragePrivate::~SQLiteStoragePrivate()
{

}


QStringList SQLiteStoragePrivate::intListToStringList(const IdList &ints) const
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


QString SQLiteStoragePrivate::intListToString(const IdList &ints) const
{
    if (ints.isEmpty()) {
        return QString();
    } else if (ints.count() == 1) {
        return QString::number(ints.first());
    } else {
        return intListToStringList(ints).join(QChar(','));
    }
}


QSqlQuery SQLiteStoragePrivate::getQuery() const
{
    return QSqlQuery(db);
}


SQLiteStorage::SQLiteStorage(const QString &dbpath, QObject *parent) :
    AbstractStorage(* new SQLiteStoragePrivate(dbpath), parent)
{
}


SQLiteStorage::~SQLiteStorage()
{

}


void SQLiteStorage::init()
{
    Q_D(SQLiteStorage);

    SQLiteStorageManager *sm = new SQLiteStorageManager(d->db.databaseName(), this);
    connect(sm, &SQLiteStorageManager::succeeded, this, [=] () {
        bool result = d->db.open();
        Q_ASSERT_X(result, "init database", "failed to open database");

        QSqlQuery q(d->db);

        result = q.exec(QStringLiteral("PRAGMA foreign_keys = ON"));
        Q_ASSERT_X(result, "init databse", "failed to enable foreign keys support");

        result = (q.exec(QStringLiteral(SEL_TOTAL_UNREAD)) && q.next());
        Q_ASSERT_X(result, "init database", "failed to query unread items from database");

        setTotalUnread(q.value(0).toInt());

        result = (q.exec(QStringLiteral(SEL_TOTAL_STARRED)) && q.next());
        Q_ASSERT_X(result, "init database", "failed to query starred items from database");

        setStarred(q.value(0).toInt());

        setReady(true);
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
        notify(error());
        return -1;
    }

    if ((type == FuotenEnums::Folder) && (id <= 0)) {
        //% "The folder ID is not valid."
        setError(new Error(Error::ApplicationError, Error::Critical, qtTrId("libfuoten-err-invalid-folder-id"), QString(), this));
        notify(error());
        return -1;
    }

    if ((type == FuotenEnums::Feed) && (id <= 0)) {
        //% "The feed ID is not valid."
        setError(new Error(Error::ApplicationError, Error::Critical, qtTrId("libfuoten-err-invalid-feed-id"), QString(), this));
        notify(error());
        return -1;
    }

    QString qs;

    switch (type) {
    case FuotenEnums::Feed:
        qs = QStringLiteral("SELECT id FROM items WHERE feedId = ? ORDER BY id DESC LIMIT 1");
        break;
    case FuotenEnums::All:
        qs = QStringLiteral("SELECT id FROM items ORDER BY id DESC LIMIT 1");
        break;
    case FuotenEnums::Folder:
        qs = QStringLiteral("SELECT it.id FROM items it INNER JOIN feeds fe ON it.feedId = fe.id WHERE fe.folderId = ? ORDER BY it.id DESC LIMIT 1");
        break;
    default:
        return -1;
    }

    Q_D(SQLiteStorage);

    QSqlQuery q(d->db);
    bool qresult = true;

    if (type == FuotenEnums::All) {

        qresult = q.exec(qs);
        Q_ASSERT_X(qresult, "get newest item ID", "failed to execute database query");

    } else {

        qresult = q.prepare(qs);
        Q_ASSERT_X(qresult, "get newest item ID", "failed to prepare database query");

        q.addBindValue(id);

        qresult = q.exec();
        Q_ASSERT_X(qresult, "get newest item ID", "failed to execute database query");
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
        notify(error());
        return;
    }

    if (json.isEmpty() || json.isNull()) {
        return;
    }

    const QJsonArray folders = json.object().value(QStringLiteral("folders")).toArray();

    qDebug("Processing %i folders requested from the remote server.", folders.size());

    QHash<qint64, QString> reqFolders({{0, QString("")}});

    for (const QJsonValue &f : folders) {
        const QJsonObject o = f.toObject();
        if (Q_LIKELY(!o.isEmpty())) {
            reqFolders.insert(std::llround(o.value(QStringLiteral("id")).toDouble()), o.value(QStringLiteral("name")).toString());
        }
    }

    QSqlQuery q(d->db);
    q.setForwardOnly(true);

    // query the currently local available folders in the database
    QHash<qint64, QString> currentFolders;

    bool qresult = q.exec(QStringLiteral("SELECT id, name FROM folders"));
    Q_ASSERT_X(qresult, "folders requested", "failed query folders from database");

    while (q.next()) {
        currentFolders.insert(q.value(0).toLongLong(), q.value(1).toString());
    }

    if (reqFolders.isEmpty() && currentFolders.isEmpty()) {
        qDebug("%s", "Nothing to do. Returning.");
        return;
    }

    IdList deletedIds;
    QStringList deletedFolderNames;
    QList<QPair<qint64, QString>> newFolders;
    QStringList newFolderNames;
    QList<QPair<qint64, QString>> updatedFolders;
    QStringList updatedFolderNames;

    if (currentFolders.isEmpty()) {

        qDebug("%s", "No local folders. Adding all requested folders as new.");

        auto i = reqFolders.constBegin();
        while (i != reqFolders.constEnd()) {
            newFolders.push_back(QPair<qint64, QString>(i.key(), i.value()));
            newFolderNames.push_back(i.value());
            ++i;
        }

    } else if (reqFolders.isEmpty()) {

        qDebug("%s", "Requested folders list is empty. Adding all local folders to deleted.");

        for (auto i = currentFolders.constBegin(); i != currentFolders.constEnd(); ++i) {
            deletedIds.push_back(i.key());
            deletedFolderNames.push_back(i.value());
        }

    } else {

        qDebug("%s", "Checking for updated and deleted folders.");

        for (auto i = currentFolders.constBegin(); i != currentFolders.constEnd(); ++i) {
            if (reqFolders.contains(i.key())) {
                if (reqFolders.value(i.key()) != i.value()) {
                    const QString newFolderName = reqFolders.value(i.key());
                    updatedFolders.push_back(qMakePair(i.key(), newFolderName));
                    updatedFolderNames.push_back(newFolderName);
                }
            } else {
                deletedIds << i.key();
                deletedFolderNames << i.value();
            }
        }

        qDebug("%s", "Checking for new folders.");
        for (auto i = reqFolders.constBegin(); i != reqFolders.constEnd(); ++i) {
            if (!currentFolders.contains(i.key())) {
                newFolders.push_back(qMakePair(i.key(), i.value()));
                newFolderNames.push_back(i.value());
            }
        }
    }


    if (!deletedIds.empty() || !newFolders.empty() || !updatedFolders.empty()) {

        qDebug("%s", "Start updating the folders table.");

        qresult = d->db.transaction();
        Q_ASSERT_X(qresult, "folders requested", "failed to start database transaction");

        if (!deletedIds.empty()) {

#ifndef QT_NO_DEBUG_OUTPUT
            QString printIdList;
            for (const qint64 id : deletedIds) { // clazy:exclude=range-loop
                printIdList.append(QString::number(id)).append(QLatin1Char(','));
            }
            printIdList.chop(1);
            qDebug("Deleting folders with IDs %s from local database.", qUtf8Printable(printIdList));
#endif

            qresult = q.exec(QStringLiteral("DELETE FROM folders WHERE id IN (%1)").arg(d->intListToString(deletedIds)));
            Q_ASSERT_X(qresult, "folders requested", "failed to delete folders from database");
        }

        if (!updatedFolders.empty()) {

            qresult = q.prepare(QStringLiteral("UPDATE folders SET name = :name WHERE id = :id"));
            Q_ASSERT_X(qresult, "folders requested", "failed to prepare updating folders in database");

            for (int i = 0; i < updatedFolders.size(); ++i) {

                qDebug("Updating name of folder with ID %lli in local database to %s.", updatedFolders.at(i).first, qUtf8Printable(updatedFolders.at(i).second));

                q.bindValue(QStringLiteral(":name"), updatedFolders.at(i).second);
                q.bindValue(QStringLiteral(":id"), updatedFolders.at(i).first);

                qresult = q.exec();
                Q_ASSERT_X(qresult, "folders requested", "failed to update folders in datbase");
            }
        }


        if (!newFolders.empty()) {

            qresult = q.prepare(QStringLiteral("INSERT INTO folders (id, name) VALUES (:id, :name)"));
            Q_ASSERT_X(qresult, "folders requested", "failed to prepare insertion of new folders in database");

            for (int i = 0; i < newFolders.size(); ++i) {

                qDebug("Adding folder \"%s\" with ID %lli to the local database.", qUtf8Printable(newFolders.at(i).second), newFolders.at(i).first);

                q.bindValue(QStringLiteral(":id"), newFolders.at(i).first);
                q.bindValue(QStringLiteral(":name"), newFolders.at(i).second);

                qresult = q.exec();
                Q_ASSERT_X(qresult, "folders requested", "failed to insert new folders into database");
            }
        }


        qresult = d->db.commit();
        Q_ASSERT_X(qresult, "folders requested", "failed to perform database commit");

        qresult = (q.exec(QStringLiteral(SEL_TOTAL_UNREAD)) && q.next());
        Q_ASSERT(qresult);
        setTotalUnread(q.value(0).value<quint16>());

        qresult = (q.exec(QStringLiteral(SEL_TOTAL_STARRED)) && q.next());
        Q_ASSERT(qresult);
        setStarred(q.value(0).value<quint16>());

        if (notificator()) {
            QVariantList notifyData;
            notifyData.push_back(newFolderNames);
            notifyData.push_back(updatedFolderNames);
            notifyData.push_back(deletedFolderNames);
            notificator()->notify(AbstractNotificator::FoldersRequested, QtInfoMsg, notifyData);
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

    const QJsonArray a = json.object().value(QStringLiteral("folders")).toArray();

    if (a.isEmpty()) {
        qWarning("Can not add folder to SQLite database. JSON array is empty.");
        return;
    }


    const QJsonObject o = a.first().toObject();

    if (o.isEmpty()) {
        qWarning("Can not add folder to SQLite databse. JSON object is empty.");
        return;
    }

    const qint64 id = std::llround(o.value(QStringLiteral("id")).toDouble());
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
    bool qresult = true;

    qresult = q.prepare(QStringLiteral("INSERT INTO folders (id, name) VALUES (?, ?)"));
    Q_ASSERT_X(qresult, "folder created", "failed to prepare insertion of new folder into database");

    q.addBindValue(id);
    q.addBindValue(name);

    qresult = q.exec();
    Q_ASSERT_X(qresult, "folder created", "failed to insert new folder into database");

    if (notificator()) {
        notificator()->notify(AbstractNotificator::FolderCreated, QtInfoMsg, name);
    }

    Q_EMIT createdFolder(id, name);

}



void SQLiteStorage::folderRenamed(qint64 id, const QString &newName)
{
    if (!ready()) {
        //% "SQLite database not ready. Can not process requested data."
        setError(new Error(Error::StorageError, Error::Warning, qtTrId("libfuoten-err-sqlite-db-not-ready"), QString(), this));
        notify(error());
        return;
    }

    if (newName.isEmpty()) {
        //% "The folder name can not be empty."
        setError(new Error(Error::ApplicationError, Error::Critical, qtTrId("libfuoten-err-empty-folder-name"), QString(), this));
        notify(error());
        return;
    }

    if (id <= 0) {
        //% "The folder ID is not valid."
        setError(new Error(Error::ApplicationError, Error::Critical, qtTrId("libfuoten-err-invalid-folder-id"), QString(), this));
        notify(error());
        return;
    }

    Q_D(SQLiteStorage);

    QSqlQuery q(d->db);
    bool qresult = q.prepare(QStringLiteral("SELECT name FROM folders WHERE id = ?"));
    Q_ASSERT(qresult);
    q.addBindValue(id);
    qresult = (q.exec() && q.next());
    Q_ASSERT(qresult);

    const QString oldName = q.value(0).toString();

    qresult = q.prepare(QStringLiteral("UPDATE folders SET name = ? WHERE id = ?"));
    Q_ASSERT_X(qresult, "folder renamed", "failed to prepare updating folder in database");

    q.addBindValue(newName);
    q.addBindValue(id);

    qresult = q.exec();
    Q_ASSERT_X(qresult, "folder renamed", "failed to update folder in database");

    if (notificator()) {
        notificator()->notify(AbstractNotificator::FolderRenamed, QtInfoMsg, QStringList({oldName, newName}));
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

    QSqlQuery q(d->db);
    q.setForwardOnly(true);

    bool qresult = q.exec(qs);
    Q_ASSERT_X(qresult, "get folders", "failed to execute datbase query");

    while (q.next()) {
        const qint64 id = q.value(0).toLongLong();
        if (Q_LIKELY(id > 0)) {
            folders.append(new Folder(
                               id,
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
        notify(error());
        return;
    }

    if (id <= 0) {
        //% "The folder ID is not valid."
        setError(new Error(Error::ApplicationError, Error::Critical, qtTrId("libfuoten-err-invalid-folder-id"), QString(), this));
        notify(error());
        return;
    }

    Q_D(SQLiteStorage);

    QSqlQuery q(d->db);

    bool qresult = q.prepare(QStringLiteral("SELECT name FROM folders WHERE id = ?"));
    Q_ASSERT_X(qresult, "folder deleted", "failed to prepare query to get name of deleted folder");

    q.addBindValue(id);

    qresult = (q.exec() && q.next());
    Q_ASSERT_X(qresult, "folder deleted", "failed to query name of deleted folder");

    const QString name = q.value(0).toString();

    qresult = q.prepare(QStringLiteral("DELETE FROM folders WHERE id = ?"));
    Q_ASSERT_X(qresult, "folder deleted", "failed to prepare qurey to delete folder from database");

    q.addBindValue(id);

    qresult = q.exec();
    Q_ASSERT_X(qresult, "folder deleted", "failed to delete folder from database");

    qresult = (q.exec(QStringLiteral(SEL_TOTAL_UNREAD)) && q.next());
    Q_ASSERT(qresult);

    setTotalUnread(q.value(0).value<quint16>());

    qresult = (q.exec(QStringLiteral(SEL_TOTAL_STARRED)) && q.next());
    Q_ASSERT(qresult);

    setStarred(q.value(0).value<quint16>());

    if (notificator()) {
        notificator()->notify(AbstractNotificator::FolderDeleted, QtInfoMsg, name);
    }

    Q_EMIT deletedFolder(id);
}



void SQLiteStorage::folderMarkedRead(qint64 id, qint64 newestItem)
{
    Q_D(SQLiteStorage);

    QSqlQuery q(d->db);
    q.setForwardOnly(true);

    bool qresult = q.prepare(QStringLiteral("UPDATE items SET unread = 0, lastModified = ? WHERE feedId IN (SELECT id FROM feeds WHERE folderId = ?)"));
    Q_ASSERT_X(qresult, "folder marked read", "failed to prepare database query");

    q.addBindValue(QDateTime::currentDateTimeUtc().toTime_t());
    q.addBindValue(id);

    qresult = q.exec();
    Q_ASSERT_X(qresult, "folder marked read", "failed to execute database query");

    qresult = q.prepare(QStringLiteral("SELECT id FROM feeds WHERE folderId = ?"));
    Q_ASSERT_X(qresult, "folder marked read", "failed to prepare database query");

    q.addBindValue(id);

    qresult = q.exec();
    Q_ASSERT(qresult);

    IdList feedIds;
    while (q.next()) {
        feedIds.push_back(q.value(0).value<qint64>());
    }

    if (!feedIds.empty()) {
        qresult = q.prepare(QStringLiteral("UPDATE feeds SET unreadCount = (SELECT COUNT(id) FROM items WHERE unread = 1 AND feedId = :feedId) WHERE id = :feedId"));
        Q_ASSERT(qresult);
        for (int i = 0; i < feedIds.size(); ++i) {
            q.bindValue(QStringLiteral(":feedId"), feedIds.at(i));
            qresult = q.exec();
            Q_ASSERT(qresult);
        }
    }

    qresult = q.prepare(QStringLiteral("UPDATE folders SET unreadCount = (SELECT SUM(unreadCount) FROM feeds WHERE folderId = :folderId) WHERE id = :folderId"));
    Q_ASSERT(qresult);
    q.bindValue(QStringLiteral(":folderId"), id);
    qresult = q.exec();
    Q_ASSERT(qresult);

    qresult = (q.exec(QStringLiteral(SEL_TOTAL_UNREAD)) && q.next());
    Q_ASSERT_X(qresult, "folder marked read", "failed to query total unread items count");

    setTotalUnread(q.value(0).toInt());

    if (notificator()) {
        qresult = q.prepare(QStringLiteral("SELECT name FROM folders WHERE id = ?"));
        Q_ASSERT(qresult);
        q.addBindValue(id);

        qresult = (q.exec() && q.next());
        Q_ASSERT(qresult);

        const QString name = q.value(0).toString();

        notificator()->notify(AbstractNotificator::FolderMarkedRead, QtInfoMsg, name);
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

    QSqlQuery q(d->db);
    q.setForwardOnly(true);
    bool qresult = q.exec(qs);
    Q_ASSERT_X(qresult, "get feeds", "failed to query feeds from database");

    while (q.next()) {
        feeds.append(new Feed(
                         q.value(0).toLongLong(),
                         q.value(1).toLongLong(),
                         q.value(2).toString(),
                         QUrl(q.value(3).toString()),
                         QUrl(q.value(4).toString()),
                         QDateTime::fromTime_t(q.value(5).toUInt()),
                         q.value(6).toUInt(),
                         static_cast<Feed::FeedOrdering>(q.value(7).toInt()),
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

    bool qresult = q.prepare(QStringLiteral("SELECT fe.id, fe.folderId, fe.title, fe.url, fe.link, fe.added, fe.unreadCount, fe.ordering, fe.pinned, fe.updateErrorCount, fe.lastUpdateError, fe.faviconLink, fo.name AS folderName FROM feeds fe LEFT JOIN folders fo ON fo.id = fe.folderId WHERE fe.id = ?"));
    Q_ASSERT_X(qresult, "get feed", "failed to prepare database query");

    q.addBindValue(id);

    qresult = q.exec();
    Q_ASSERT_X(qresult, "get feed", "failed to execute database query");


    if (Q_LIKELY(q.next())) {
        Feed *f = new Feed(
                        q.value(0).toLongLong(),
                        q.value(1).toLongLong(),
                        q.value(2).toString(),
                        QUrl(q.value(3).toString()),
                        QUrl(q.value(4).toString()),
                        QDateTime::fromTime_t(q.value(5).toUInt()),
                        q.value(6).toUInt(),
                        static_cast<Feed::FeedOrdering>(q.value(7).toInt()),
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
        notify(error());
        return;
    }

    if (json.isEmpty() || json.isNull()) {
        return;
    }

    QSqlQuery q(d->db);
    bool qresult = true;

    const QJsonArray feeds = json.object().value(QStringLiteral("feeds")).toArray();

    qDebug("Processing %i feeds requested from the remote server.", feeds.size());

    QueryArgs qa;
    const QList<Feed*> currentFeeds = getFeeds(qa);

    IdList updatedFeedIds;
    QStringList updatedFeedNames;
    IdList newFeedIds;
    QStringList newFeedNames;
    IdList deletedFeedIds;
    QStringList deletedFeedNames;

    if (feeds.isEmpty() && currentFeeds.isEmpty()) {

        qDebug("%s", "Nothing to do. Local feeds and remote feeds are empty.");

        Q_EMIT requestedFeeds(updatedFeedIds, newFeedIds, deletedFeedIds);
        return;

    } else if (feeds.isEmpty() && !currentFeeds.isEmpty()) {

        qDebug("%s", "All feeds have been deleted on the server. Deleting local ones.");

        deletedFeedIds.reserve(currentFeeds.size());
        deletedFeedNames.reserve(currentFeeds.size());
        for (Feed *f : currentFeeds) {
            deletedFeedIds.push_back(f->id());
            deletedFeedNames.push_back(f->title());
        }

        qresult = q.exec(QStringLiteral("DELETE FROM feeds"));
        Q_ASSERT_X(qresult, "feeds requested", "failed to delete all feeds from database");

    } else if (!feeds.isEmpty() && currentFeeds.isEmpty()) {

        qDebug("%s", "No local feeds. Adding all requested feeds as new feeds.");

        newFeedIds.reserve(feeds.size());
        newFeedNames.reserve(feeds.size());

        qresult = d->db.transaction();
        Q_ASSERT_X(qresult, "feeds requested", "failed to start database transaction");


        qresult = q.prepare(QStringLiteral("INSERT INTO feeds (id, folderId, title, url, link, added, ordering, pinned, updateErrorCount, lastUpdateError, faviconLink) "
                                           "VALUES (:id, :folderId, :title, :url, :link, :added, :ordering, :pinned, :updateErrorCount, :lastUpdateError, :faviconLink)"
                                           ));
        Q_ASSERT_X(qresult, "feeds requested", "failed to prepare to insert new feed into database");

        for (const QJsonValue &f : feeds) {
            const QJsonObject o = f.toObject();
            if (Q_LIKELY(!o.isEmpty())) {
                const qlonglong feedId = std::llround(o.value(QStringLiteral("id")).toDouble());
                const QString feedTitle = o.value(QStringLiteral("title")).toString();
                newFeedIds.push_back(feedId);
                newFeedNames.push_back(feedTitle);

                q.bindValue(QStringLiteral(":id"), feedId);
                q.bindValue(QStringLiteral(":folderId"), std::llround(o.value(QStringLiteral("folderId")).toDouble()));
                q.bindValue(QStringLiteral(":title"), feedTitle);
                q.bindValue(QStringLiteral(":url"), o.value(QStringLiteral("url")).toString());
                q.bindValue(QStringLiteral(":link"), o.value(QStringLiteral("link")).toString());
                q.bindValue(QStringLiteral(":added"), o.value(QStringLiteral("added")).toVariant().toUInt());
                q.bindValue(QStringLiteral(":ordering"), o.value(QStringLiteral("ordering")).toInt());
                q.bindValue(QStringLiteral(":pinned"), o.value(QStringLiteral("pinned")).toBool());
                q.bindValue(QStringLiteral(":updateErrorCount"), o.value(QStringLiteral("updateErrorCount")).toInt());
                q.bindValue(QStringLiteral(":lastUpdateError"), o.value(QStringLiteral("lastUpdateError")).toString());
                q.bindValue(QStringLiteral(":faviconLink"), o.value(QStringLiteral("faviconLink")).toString());

                qresult = q.exec();
                Q_ASSERT_X(qresult, "feeds requested", "failed to insert new feed into database");
            }
        }

        qresult = d->db.commit();
        Q_ASSERT_X(qresult, "feeds requested", "failed to commit database transaction");


    } else {


        qDebug("%s", "Checking for updated, new and deleted feeds.");

        QHash<qint64, Feed*> cfh; // current feeds hash
        IdList requestedFeedIds;

        for (Feed *f : currentFeeds) {
            cfh.insert(f->id(), f);
        }

        qresult = d->db.transaction();
        Q_ASSERT_X(qresult, "feeds requested", "failed to start database transaction");

        for (const QJsonValue &f : feeds) {
            const QJsonObject o = f.toObject();
            if (Q_UNLIKELY(!o.isEmpty())) {
                const qint64 id = std::llround(o.value(QStringLiteral("id")).toDouble());
                const QString title = o.value(QStringLiteral("title")).toString();
                requestedFeedIds.push_back(id);

                if (!cfh.contains(id)) {
                    newFeedIds.push_back(id);
                    newFeedNames.push_back(title);

                    qDebug("Adding new feed \"%s\" with ID %lli to the database.", qUtf8Printable(o.value(QStringLiteral("title")).toString()), id);

                    qresult = q.prepare(QStringLiteral("INSERT INTO feeds (id, folderId, title, url, link, added, ordering, pinned, updateErrorCount, lastUpdateError, faviconLink) "
                                                       "VALUES (?,?,?,?,?,?,?,?,?,?,?)"
                                                       ));
                    Q_ASSERT_X(qresult, "feeds requested", "failed to prepare inserting new feed into database");

                    q.addBindValue(id);
                    q.addBindValue(std::llround(o.value(QStringLiteral("folderId")).toDouble()));
                    q.addBindValue(title);
                    q.addBindValue(o.value(QStringLiteral("url")).toString());
                    q.addBindValue(o.value(QStringLiteral("link")).toString());
                    q.addBindValue(o.value(QStringLiteral("added")).toVariant().toUInt());
                    q.addBindValue(o.value(QStringLiteral("ordering")).toInt());
                    q.addBindValue(o.value(QStringLiteral("pinned")).toBool());
                    q.addBindValue(o.value(QStringLiteral("updateErrorCount")).toInt());
                    q.addBindValue(o.value(QStringLiteral("lastUpdateError")).toString());
                    q.addBindValue(o.value(QStringLiteral("faviconLink")).toString());

                    qresult = q.exec();
                    Q_ASSERT_X(qresult, "feeds requested", "failed to insert new feed into database");

                } else {

                    const QUrl rFaviconLink = QUrl(o.value(QStringLiteral("faviconLink")).toString());
                    const qint64 rFolderId = std::llround(o.value(QStringLiteral("folderId")).toDouble());
                    const Feed::FeedOrdering rOrdering = static_cast<Feed::FeedOrdering>(o.value(QStringLiteral("ordering")).toInt());
                    const QUrl rLink = QUrl(o.value(QStringLiteral("link")).toString());
                    const bool rPinned = o.value(QStringLiteral("pinned")).toBool();
                    const uint rUpdateErrorCount = o.value(QStringLiteral("updateErrorCount")).toInt();
                    const QString rLastUpdateError = o.value(QStringLiteral("lastUpdateError")).toString();

                    Feed *f = cfh.value(id);

                    if ((f->title() != title) || (f->faviconLink() != rFaviconLink) || (f->folderId() != rFolderId) || (f->ordering() != rOrdering) || (f->link() != rLink) || (f->pinned() != rPinned) || (f->updateErrorCount() != rUpdateErrorCount) || (f->lastUpdateError() != rLastUpdateError)) {

                        qDebug("Updating feed \"%s\" with ID %lli in the database.", qUtf8Printable(f->title()), id);

                        updatedFeedIds.push_back(id);

                        // only notify about feeds with relevant changes
                        if (f->title() != title || f->folderId() != rFolderId || f->ordering() != rOrdering || f->pinned() != rPinned) {
                            updatedFeedNames.push_back(title);
                        }

                        qresult = q.prepare(QStringLiteral("UPDATE feeds SET folderId = ?, title = ?, link = ?, ordering = ?, pinned = ?, updateErrorCount = ?, lastUpdateError = ?, faviconLink = ? WHERE id = ?"));
                        Q_ASSERT_X(qresult, "feeds requested", "failed to prepare updating feed in database");

                        q.addBindValue(rFolderId);
                        q.addBindValue(title);
                        q.addBindValue(rLink.toString());
                        q.addBindValue(static_cast<int>(rOrdering));
                        q.addBindValue(rPinned);
                        q.addBindValue(rUpdateErrorCount);
                        q.addBindValue(rLastUpdateError);
                        q.addBindValue(rFaviconLink.toString());
                        q.addBindValue(id);

                        qresult = q.exec();
                        Q_ASSERT_X(qresult, "feeds requested", "failed to update feed in database");
                    }
                }
            }
        }



        QHash<qint64, Feed*>::const_iterator i = cfh.constBegin();
        while (i != cfh.constEnd()) {
            if (!requestedFeedIds.contains(i.key())) {
                deletedFeedIds.push_back(i.key());
                deletedFeedNames.push_back(i.value()->title());
            }
            ++i;
        }

        if (!deletedFeedIds.isEmpty()) {
#ifndef QT_NO_DEBUG_OUTPUT
            QString printIdsString;
            for (const qint64 id : deletedFeedIds) { // clazy:exclude=range-loop
                printIdsString.append(QString::number(id)).append(QLatin1Char(','));
            }
            printIdsString.chop(1);
            qDebug("The feeds with the following IDs have been deleted on the server: %s", qUtf8Printable(printIdsString));
#endif

            qresult = q.exec(QStringLiteral("DELETE FROM feeds WHERE id IN (%1)").arg(d->intListToString(deletedFeedIds)));
            Q_ASSERT_X(qresult, "feeds requested", "failed to delete feeds from database");
        }

        qresult = d->db.commit();
        Q_ASSERT_X(qresult, "feeds requested", "failed to commit changes to database");

        cfh.clear();
    }

    qDeleteAll(currentFeeds);

    q.setForwardOnly(true);
    qresult = q.exec(QStringLiteral("SELECT id FROM folders"));
    Q_ASSERT(qresult);

    IdList folderIds;
    while (q.next()) {
        folderIds.push_back(q.value(0).value<qint64>());
    }

    if (!folderIds.empty()) {
        for (int i = 0; i < folderIds.size(); ++i) {
            const qint64 folderId = folderIds.at(i);
            qresult = q.prepare(QStringLiteral("UPDATE folders SET unreadCount = (SELECT SUM(unreadCount) FROM feeds WHERE folderId = :folderId), feedCount = (SELECT COUNT(id) FROM feeds WHERE folderId = :folderId) WHERE id = :folderId"));
            Q_ASSERT(qresult);
            q.bindValue(QStringLiteral(":folderId"), folderId);
            qresult = q.exec();
            Q_ASSERT(qresult);
        }
    }

    qresult = (q.exec(QStringLiteral(SEL_TOTAL_UNREAD)) && q.next());
    Q_ASSERT(qresult);
    setTotalUnread(q.value(0).value<quint16>());

    qresult = (q.exec(QStringLiteral(SEL_TOTAL_STARRED)) && q.next());
    Q_ASSERT(qresult);
    setStarred(q.value(0).value<quint16>());

    if (!newFeedNames.empty() || !updatedFeedNames.empty() || !deletedFeedNames.empty()) {
        if (notificator()) {
            QVariantList data;
            data.push_back(newFeedNames);
            data.push_back(updatedFeedNames);
            data.push_back(deletedFeedNames);
            notificator()->notify(AbstractNotificator::FeedsRequested, QtInfoMsg, data);
        }
    }

    Q_EMIT requestedFeeds(updatedFeedIds, newFeedIds, deletedFeedIds);
}



void SQLiteStorage::feedCreated(const QJsonDocument &json)
{
    if (!ready()) {
        //% "SQLite database not ready. Can not process requested data."
        setError(new Error(Error::StorageError, Error::Warning, qtTrId("libfuoten-err-sqlite-db-not-ready"), QString(), this));
        notify(error());
        return;
    }

    if (json.isEmpty() || json.isNull()) {
        qWarning("Can not add folder to SQLite database. JSON data is not valid.");
        return;
    }

    const QJsonArray a = json.object().value(QStringLiteral("feeds")).toArray();

    if (a.isEmpty()) {
        qWarning("Can not add feed to SQLite database. JSON array is empty.");
        return;
    }


    const QJsonObject o = a.first().toObject();

    if (o.isEmpty()) {
        qWarning("Can not add feed to SQLite databse. JSON object is empty.");
        return;
    }

    Q_D(SQLiteStorage);

    QSqlQuery q(d->db);

    bool qresult = q.prepare(QStringLiteral("INSERT INTO feeds (id, folderId, title, url, link, added, ordering, pinned, updateErrorCount, lastUpdateError, faviconLink) "
                                            "VALUES (?,?,?,?,?,?,?,?,?,?,?)"
                                            ));
    Q_ASSERT_X(qresult, "feed created", "failed to prepare database query");

    const qint64 id = std::llround(o.value(QStringLiteral("id")).toDouble());
    const qint64 folderId = std::llround(o.value(QStringLiteral("folderId")).toDouble());
    const QString title = o.value(QStringLiteral("title")).toString();

    q.addBindValue(id);
    q.addBindValue(folderId);
    q.addBindValue(title);
    q.addBindValue(o.value(QStringLiteral("url")).toString());
    q.addBindValue(o.value(QStringLiteral("link")).toString());
    q.addBindValue(o.value(QStringLiteral("added")).toVariant().toUInt());
    q.addBindValue(o.value(QStringLiteral("ordering")).toInt());
    q.addBindValue(o.value(QStringLiteral("pinned")).toBool());
    q.addBindValue(o.value(QStringLiteral("updateErrorCount")).toInt());
    q.addBindValue(o.value(QStringLiteral("lastUpdateError")).toString());
    q.addBindValue(o.value(QStringLiteral("faviconLink")).toString());

    qresult = q.exec();
    Q_ASSERT_X(qresult, "feed created", "failed to execute database query");

    qresult = q.prepare(QStringLiteral("UPDATE folders SET feedCount = feedCount + 1, unreadCount = (SELECT SUM(unreadCount) FROM feeds WHERE folderId = :folderId) WHERE id = :folderId"));
    Q_ASSERT(qresult);
    q.bindValue(QStringLiteral(":folderId"), folderId);
    qresult = q.exec();
    Q_ASSERT(qresult);

    if (notificator()) {
        notificator()->notify(AbstractNotificator::FeedCreated, QtInfoMsg, title);
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
        setError(new Error(Error::ApplicationError, Error::Critical, qtTrId("libfuoten-err-invalid-feed-id"), QString(), this));
        return;
    }

    Q_D(SQLiteStorage);

    QSqlQuery q(d->db);

    bool qresult = q.prepare(QStringLiteral("SELECT folderId, title FROM feeds WHERE id = ?"));
    Q_ASSERT(qresult);
    q.addBindValue(id);
    qresult = (q.exec() && q.next());
    Q_ASSERT(qresult);
    const qint64 folderId = q.value(0).value<qint64>();
    const QString title = q.value(1).toString();

    qresult = q.prepare(QStringLiteral("DELETE FROM feeds WHERE id = ?"));
    Q_ASSERT_X(qresult, "feed deleted", "failed to prepare database query");

    q.addBindValue(id);

    qresult = q.exec();
    Q_ASSERT_X(qresult, "feed deleted", "failed to execute database query");

    qresult = q.prepare(QStringLiteral("UPDATE folders SET feedCount = feedCount - 1, unreadCount = (SELECT SUM(unreadCount) FROM feeds WHERE folderId = :folderId) WHERE id = :folderId"));
    Q_ASSERT(qresult);
    q.bindValue(QStringLiteral(":folderId"), folderId);
    qresult = q.exec();
    Q_ASSERT(qresult);

    qresult = (q.exec(QStringLiteral(SEL_TOTAL_UNREAD)) && q.next());
    Q_ASSERT(qresult);
    setTotalUnread(q.value(0).value<quint16>());

    qresult = (q.exec(QStringLiteral(SEL_TOTAL_STARRED)) && q.next());
    Q_ASSERT(qresult);
    setStarred(q.value(0).value<quint16>());

    if (notificator()) {
        notificator()->notify(AbstractNotificator::FeedDeleted, QtInfoMsg, title);
    }

    Q_EMIT deletedFeed(id);
}



void SQLiteStorage::feedMoved(qint64 id, qint64 targetFolder)
{
    if (!ready()) {
        //% "SQLite database not ready. Can not process requested data."
        setError(new Error(Error::StorageError, Error::Warning, qtTrId("libfuoten-err-sqlite-db-not-ready"), QString(), this));
        notify(error());
        return;
    }

    if (id <= 0) {
        //% "The feed ID is not valid."
        setError(new Error(Error::ApplicationError, Error::Critical, qtTrId("libfuoten-err-invalid-feed-id"), QString(), this));
        notify(error());
        return;
    }

    if (targetFolder < 0) {
        //% "The folder ID is not valid."
        setError(new Error(Error::ApplicationError, Error::Critical, qtTrId("libfuoten-err-invalid-folder-id"), QString(), this));
        notify(error());
        return;
    }

    Q_D(SQLiteStorage);

    QSqlQuery q(d->db);

    bool qresult = q.prepare(QStringLiteral("SELECT folderId FROM feeds WHERE id = ?"));
    Q_ASSERT(qresult);
    q.addBindValue(id);
    qresult = (q.exec() && q.next());
    Q_ASSERT(qresult);
    const qint64 oldFolderId = q.value(0).value<qint64>();

    qresult = q.prepare(QStringLiteral("UPDATE feeds SET folderId = ? WHERE id = ?"));
    Q_ASSERT_X(qresult, "feed moved", "failed to prepare database query");

    q.addBindValue(targetFolder);
    q.addBindValue(id);

    qresult = q.exec();
    Q_ASSERT_X(qresult, "feed moved", "failed to execute database query");

    for (const qint64 fid : {targetFolder, oldFolderId}) {
        qresult = q.prepare(QStringLiteral("UPDATE folders SET unreadCount = (SELECT SUM(unreadCount) FROM feeds WHERE folderId = :folderId), feedCount = (SELECT COUNT(id) FROM feeds WHERE folderId = :folderId) WHERE id = :folderId"));
        Q_ASSERT(qresult);
        q.bindValue(QStringLiteral(":folderId"), fid);
        qresult = q.exec();
        Q_ASSERT(qresult);
    }

    if (notificator() && notificator()->isEnabled()) {
        qresult = q.prepare(QStringLiteral("SELECT name FROM folders WHERE id = ?"));
        Q_ASSERT(qresult);
        q.addBindValue(oldFolderId);
        qresult = (q.exec() && q.next());
        Q_ASSERT(qresult);
        const QString oldFolderName = q.value(0).toString();

        qresult = q.prepare(QStringLiteral("SELECT name FROM folders WHERE id = ?"));
        Q_ASSERT(qresult);
        q.addBindValue(targetFolder);
        qresult = (q.exec() && q.next());
        Q_ASSERT(qresult);
        const QString targetFolderName = q.value(0).toString();

        qresult = q.prepare(QStringLiteral("SELECT title FROM feeds WHERE id = ?"));
        Q_ASSERT(qresult);
        q.addBindValue(id);
        qresult = (q.exec() && q.next());
        Q_ASSERT(qresult);
        const QString feedTitle = q.value(0).toString();

        QVariantList data;
        data.push_back(feedTitle);
        data.push_back(oldFolderName);
        data.push_back(targetFolderName);

        notificator()->notify(AbstractNotificator::FeedMoved, QtInfoMsg, data);
    }

    Q_EMIT movedFeed(id, targetFolder);
}


void SQLiteStorage::feedRenamed(qint64 id, const QString &newTitle)
{
    if (!ready()) {
        //% "SQLite database not ready. Can not process requested data."
        setError(new Error(Error::StorageError, Error::Warning, qtTrId("libfuoten-err-sqlite-db-not-ready"), QString(), this));
        notify(error());
        return;
    }

    if (newTitle.isEmpty()) {
        //% "The feed name can not be empty."
        setError(new Error(Error::ApplicationError, Error::Critical, qtTrId("libfuoten-err-empty-feed-name"), QString(), this));
        notify(error());
        return;
    }

    if (id <= 0) {
        //% "The feed ID is not valid."
        setError(new Error(Error::ApplicationError, Error::Critical, qtTrId("libfuoten-err-invalid-feed-id"), QString(), this));
        notify(error());
        return;
    }


    Q_D(SQLiteStorage);

    QSqlQuery q(d->db);

    bool qresult = q.prepare(QStringLiteral("SELECT title FROM feeds WHERE id = ?"));
    Q_ASSERT_X(qresult, "feed renamed", "failed to prepare query for old feed title");
    q.addBindValue(id);
    qresult = (q.exec() && q.next());
    Q_ASSERT_X(qresult, "feed renamed", "failed to query old feed title");
    const QString oldTitle = q.value(0).toString();

    qresult = q.prepare(QStringLiteral("UPDATE feeds SET title = ? WHERE id = ?"));
    Q_ASSERT_X(qresult, "feed renamed", "failed to prepare database query");

    q.addBindValue(newTitle);
    q.addBindValue(id);

    qresult = q.exec();
    Q_ASSERT_X(qresult, "feed renamed", "failed to execute database query");

    if (notificator()) {
        QVariantList data;
        data.push_back(oldTitle);
        data.push_back(newTitle);
        notificator()->notify(AbstractNotificator::FeedRenamed, QtInfoMsg, data);
    }

    Q_EMIT renamedFeed(id, newTitle);
}



void SQLiteStorage::feedMarkedRead(qint64 id, qint64 newestItem)
{
    if (!ready()) {
        //% "SQLite database not ready. Can not process requested data."
        setError(new Error(Error::StorageError, Error::Warning, qtTrId("libfuoten-err-sqlite-db-not-ready"), QString(), this));
        notify(error());
        return;
    }

    if (id <= 0) {
        //% "The feed ID is not valid."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-invalid-feed-id"), QString(), this));
        notify(error());
        return;
    }

    if (newestItem <= 0) {
        //% "The item ID is not valid."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-invalid-item-id"), QString(), this));
        notify(error());
        return;
    }

    Q_D(SQLiteStorage);

    QSqlQuery q(d->db);

    bool qresult = q.prepare(QStringLiteral("UPDATE items SET unread = 0, lastModified = ? WHERE feedId = ? AND id <= ?"));
    Q_ASSERT_X(qresult, "feed marked read", "failed to prepare database query");

    q.addBindValue(QDateTime::currentDateTimeUtc().toTime_t());
    q.addBindValue(id);
    q.addBindValue(newestItem);

    qresult = q.exec();
    Q_ASSERT_X(qresult, "feed marked read", "failed to execute database query");

    qresult = q.prepare(QStringLiteral("UPDATE feeds SET unreadCount = (SELECT COUNT(id) FROM items WHERE unread = 1 AND feedId = :feedId) WHERE id = :feedId"));
    Q_ASSERT(qresult);
    q.bindValue(QStringLiteral(":feedId"), id);
    qresult = q.exec();
    Q_ASSERT(qresult);

    qresult = q.prepare(QStringLiteral("SELECT folderId FROM feeds WHERE id = ?"));
    Q_ASSERT(qresult);
    q.addBindValue(id);
    qresult = (q.exec() && q.next());

    const qint64 folderId = q.value(0).value<qint64>();

    qresult = q.prepare(QStringLiteral("UPDATE folders SET unreadCount = (SELECT SUM(unreadCount) FROM feeds WHERE folderId = :folderId) WHERE id = :folderId"));
    Q_ASSERT(qresult);
    q.bindValue(QStringLiteral(":folderId"), folderId);
    qresult = q.exec();
    Q_ASSERT(qresult);

    qresult = (q.exec(QStringLiteral(SEL_TOTAL_UNREAD)) && q.next());
    Q_ASSERT_X(qresult, "feed marked read", "failed to query all unread items from database");

    setTotalUnread(q.value(0).toInt());

    if (notificator()) {
        qresult = q.prepare(QStringLiteral("SELECT title FROM feeds WHERE id = ?"));
        Q_ASSERT_X(qresult, "feed marked read", "failed to prepare query for feed title");
        q.addBindValue(id);
        qresult = (q.exec() && q.next());
        Q_ASSERT_X(qresult, "feed marked read", "failed to query title of the feed");
        const QString title = q.value(0).toString();

        notificator()->notify(AbstractNotificator::FeedMarkedRead, QtInfoMsg, title);
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

    bool qresult = q.prepare(QStringLiteral("SELECT it.id, it.feedId, fe.title, it.guid, it.guidHash, it.url, it.title, it.author, it.pubDate, it.body, it.enclosureMime, it.enclosureLink, it.unread, it.starred, it.lastModified, it.fingerprint, fo.id, fo.name, it.queue FROM items it LEFT JOIN feeds fe ON fe.id = it.feedId LEFT JOIN folders fo on fo.id = fe.folderId WHERE it.id = ?"));
    Q_ASSERT_X(qresult, "get article", "failed to prepare database query");

    q.addBindValue(id);

    qresult = q.exec();
    Q_ASSERT_X(qresult, "get article", "failed to execute database query");

    QString body;

    if (bodyLimit == 0) {
        body = q.value(9).toString();
    } else if (bodyLimit > 0) {
        body = limitBody(q.value(9).toString(), bodyLimit);
    }


    if (Q_LIKELY(q.next())) {
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

    QString qs = QStringLiteral("SELECT it.id, it.feedId, fe.title, it.guid, it.guidHash, it.url, it.title, it.author, it.pubDate, it.body, it.enclosureMime, it.enclosureLink, it.unread, it.starred, it.lastModified, it.fingerprint, fo.id, fo.name, it.queue FROM items it LEFT JOIN feeds fe ON fe.id = it.feedId LEFT JOIN folders fo on fo.id = fe.folderId");

    qs.append(QStringLiteral(" WHERE it.pubDate < %1").arg(QString::number(QDateTime::currentDateTimeUtc().toTime_t())));

    if (args.parentId > -1) {
        if (args.parentIdType == FuotenEnums::Feed) {
            qs.append(QStringLiteral(" AND it.feedId = %1").arg(QString::number(args.parentId)));
        } else {
            qs.append(QStringLiteral(" AND it.feedId IN (SELECT id FROM feeds WHERE folderId = %1)").arg(args.parentId));
        }
    }

    if (!args.inIds.isEmpty()) {
        switch(args.inIdsType) {
        case FuotenEnums::Folder:
            qs.append(QStringLiteral(" AND it.feedId IN (SELECT id FROM feeds WHERE folderId IN (%1))").arg(d->intListToString(args.inIds)));
            break;
        case FuotenEnums::Feed:
            qs.append(QStringLiteral(" AND it.feedId IN (%1)").arg(d->intListToString(args.inIds)));
            break;
        default:
            qs.append(QStringLiteral(" AND it.id IN (%1)").arg(d->intListToString(args.inIds)));
            break;
        }
    }

    if (args.unreadOnly) {
        qs.append(QLatin1String(" AND it.unread = 1"));
    }

    if (args.starredOnly) {
        qs.append(QLatin1String(" AND it.starred = 1"));
    }

    if (args.queuedOnly) {
        qs.append(QLatin1String(" AND it.queue > 0"));
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

    qDebug("Start to query articles from the local SQLite database using the following query: %s", qUtf8Printable(qs));

    QSqlQuery q(d->db);
    q.setForwardOnly(true);

    bool qresult = q.exec(qs);
    Q_ASSERT_X(qresult, "get article", "failed to execute database query");

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


GetArticlesAsyncWorker::~GetArticlesAsyncWorker()
{

}


void GetArticlesAsyncWorker::run()
{
    QList<Article*> articles;

    QSqlQuery q(m_db);

    bool qresult = q.exec(QStringLiteral("PRAGMA foreign_keys = ON"));
    Q_ASSERT_X(qresult, "get articles async", "failed to enable foreign keys support");

    QString qs = QStringLiteral("SELECT it.id, it.feedId, fe.title, it.guid, it.guidHash, it.url, it.title, it.author, it.pubDate, it.body, it.enclosureMime, it.enclosureLink, it.unread, it.starred, it.lastModified, it.fingerprint, fo.id, fo.name, it.queue FROM items it LEFT JOIN feeds fe ON fe.id = it.feedId LEFT JOIN folders fo on fo.id = fe.folderId");

    qs.append(QStringLiteral(" WHERE it.pubDate < %1").arg(QString::number(QDateTime::currentDateTimeUtc().toTime_t())));

    if (m_args.parentId > -1) {
        if (m_args.parentIdType == FuotenEnums::Feed) {
            qs.append(QStringLiteral(" AND it.feedId = %1").arg(QString::number(m_args.parentId)));
        } else {
            qs.append(QStringLiteral(" AND it.feedId IN (SELECT id FROM feeds WHERE folderId = %1)").arg(m_args.parentId));
        }
    }

    if (!m_args.inIds.isEmpty()) {
        QString idListString;
        if (m_args.inIds.count() == 1) {
            idListString = QString::number(m_args.inIds.first());
        } else {
            const IdList ids = m_args.inIds;
            QStringList sl;
            sl.reserve(ids.size());
            for (qint64 id : ids) {
                sl.append(QString::number(id));
            }
            idListString = sl.join(QChar(','));
        }


        switch(m_args.inIdsType) {
        case FuotenEnums::Folder:
            qs.append(QStringLiteral(" AND it.feedId IN (SELECT id FROM feeds WHERE folderId IN (%1))").arg(idListString));
            break;
        case FuotenEnums::Feed:
            qs.append(QStringLiteral(" AND it.feedId IN (%1)").arg(idListString));
            break;
        default:
            qs.append(QStringLiteral(" AND it.id IN (%1)").arg(idListString));
            break;
        }
    }

    if (m_args.unreadOnly) {
        qs.append(QLatin1String(" AND it.unread = 1"));
    }

    if (m_args.starredOnly) {
        qs.append(QLatin1String(" AND it.starred = 1"));
    }

    if (m_args.queuedOnly) {
        qs.append(QStringLiteral(" AND it.queue > 0"));
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

    qDebug("Start to query articles fromt the local SQLite database using the following query: %s", qUtf8Printable(qs));

    q.setForwardOnly(true);
    qresult = q.exec(qs);
    Q_ASSERT_X(qresult, "get articles async", "failed to execute database query");

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
    connect(worker, &GetArticlesAsyncWorker::failed, this, [=] (Error *e) {setError(e);});
    connect(worker, &QThread::finished, worker, &QObject::deleteLater);
    worker->start();

}



ItemsRequestedWorker::ItemsRequestedWorker(const QString &dbpath, const QJsonDocument &json, AbstractConfiguration *config, AbstractNotificator *notificator, QObject *parent) :
    QThread(parent), m_json(json), m_config(config), m_notificator(notificator)
{
    if (!QSqlDatabase::connectionNames().contains(QStringLiteral("fuotendb"))) {
        m_db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), QStringLiteral("fuotendb"));
        m_db.setDatabaseName(dbpath);
    } else {
        m_db = QSqlDatabase::database(QStringLiteral("fuotendb"));
    }
}


ItemsRequestedWorker::~ItemsRequestedWorker()
{

}


void ItemsRequestedWorker::run()
{
    QSqlQuery q(m_db);

    bool qresult = q.exec(QStringLiteral("PRAGMA foreign_keys = ON"));
    Q_ASSERT_X(qresult, "items requested worker", "failed to enable foreign keys support");
    q.setForwardOnly(true);

    const QJsonArray items = m_json.object().value(QStringLiteral("items")).toArray();

    IdList updatedItemIds;
    IdList newItemIds;
    IdList removedItemIds;

    if (items.isEmpty()) {
        Q_EMIT requestedItems(updatedItemIds, newItemIds, removedItemIds);
        qDebug("%s", "Nothing to do. No Items.");
        return;
    }

    QHash<qint64, uint> currentItems; // contains the ids and last modified time stamps of the local items

    qresult = q.exec(QStringLiteral("SELECT id, lastModified FROM items"));
    Q_ASSERT_X(qresult, "items requested worker", "failed to query current items from database");

    while(q.next()) {
        currentItems.insert(q.value(0).toLongLong(), q.value(1).toUInt());
    }

    QHash<qint64,QString> feedsIdTitleMap;
    qresult = q.exec(QStringLiteral("SELECT id, title FROM feeds"));
    Q_ASSERT(qresult);

    while(q.next()) {
        feedsIdTitleMap.insert(q.value(0).value<qint64>(), q.value(1).toString());
    }


    qresult = m_db.transaction();
    Q_ASSERT_X(qresult, "items requested worker", "failed to start database transaction");

    quint32 newUnreadItems = 0;

    QVector<QJsonObject> articlesToPublish;
    const bool publishArticles = (m_notificator && m_notificator->isArticlePublishingEnabled());

    for (const QJsonValue &i : items) {
        const QJsonObject o = i.toObject();
        if (Q_LIKELY(!o.isEmpty())) {
            const qint64 id = std::llround(o.value(QStringLiteral("id")).toDouble());

            if (!currentItems.isEmpty() && currentItems.contains(id)) {

                uint lastMod = o.value(QStringLiteral("lastModified")).toInt();

                if (currentItems.value(id) < lastMod) {

                    updatedItemIds.append(id);


                    qDebug("Updating the article \"%s\" with ID %lli in the database.", qUtf8Printable(o.value(QStringLiteral("title")).toString()), id);

                    qresult = q.prepare(QStringLiteral("UPDATE items SET "
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
                                                       ));
                    Q_ASSERT_X(qresult, "items requested worker", "failed to prepare update of item into database");

                    q.addBindValue(o.value(QStringLiteral("title")).toString(QStringLiteral("")));
                    q.addBindValue(o.value(QStringLiteral("url")).toString(QStringLiteral("")));
                    q.addBindValue(o.value(QStringLiteral("author")).toString(QStringLiteral("")));
                    q.addBindValue(o.value(QStringLiteral("pubDate")).toInt());
                    q.addBindValue(o.value(QStringLiteral("enclosureMime")).toString());
                    q.addBindValue(o.value(QStringLiteral("enclosureLink")).toString());
                    q.addBindValue(o.value(QStringLiteral("unread")).toBool());
                    q.addBindValue(o.value(QStringLiteral("starred")).toBool());
                    q.addBindValue(lastMod);
                    q.addBindValue(o.value(QStringLiteral("fingerprint")).toString());
                    q.addBindValue(id);

                    qresult = q.exec();
                    Q_ASSERT_X(qresult, "items requested worker", "failed to update item in databae");
                }

            } else {

                newItemIds.append(id);
                const bool unread = o.value(QStringLiteral("unread")).toBool();
                if (unread) {
                    newUnreadItems++;
                }

                qDebug("Adding new article \"%s\" with ID %lli to the database.", qUtf8Printable(o.value(QStringLiteral("title")).toString()), id);

                qresult = q.prepare(QStringLiteral("INSERT INTO items (id, feedId, guid, guidHash, url, title, author, pubDate, body, enclosureMime, enclosureLink, unread, starred, lastModified, fingerprint, rtl, mediaThumbnail, mediaDescription) "
                                                   "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"
                                                   ));
                Q_ASSERT_X(qresult, "items requested worker", "failed to prepare insertion of new item into database");

                q.addBindValue(id);
                q.addBindValue(std::llround(o.value(QStringLiteral("feedId")).toDouble()));
                q.addBindValue(o.value(QStringLiteral("guid")).toString());
                q.addBindValue(o.value(QStringLiteral("guidHash")).toString());
                q.addBindValue(o.value(QStringLiteral("url")).toString(QStringLiteral("")));
                q.addBindValue(o.value(QStringLiteral("title")).toString(QStringLiteral("")));
                q.addBindValue(o.value(QStringLiteral("author")).toString(QStringLiteral("")));
                q.addBindValue(o.value(QStringLiteral("pubDate")).toInt());
                q.addBindValue(o.value(QStringLiteral("body")).toString(QStringLiteral("")));
                q.addBindValue(o.value(QStringLiteral("enclosureMime")).toString());
                q.addBindValue(o.value(QStringLiteral("enclosureLink")).toString());
                q.addBindValue(unread);
                q.addBindValue(o.value(QStringLiteral("starred")).toBool());
                q.addBindValue(o.value(QStringLiteral("lastModified")).toInt());
                q.addBindValue(o.value(QStringLiteral("fingerprint")).toString());
                q.addBindValue(o.value(QStringLiteral("rtl")).toBool());
                q.addBindValue(o.value(QStringLiteral("mediaThumbnail")).toString());
                q.addBindValue(o.value(QStringLiteral("mediaDescription")).toString());

                qresult = q.exec();
                Q_ASSERT_X(qresult, "items requested worker", "failed to execute insertion of new item into database");

                if (publishArticles && unread) {
                    if (m_notificator->checkForPublishing(o)) {
                        articlesToPublish.push_back(o);
                    }
                }
            }
        }
    }

    qresult = m_db.commit();
    Q_ASSERT_X(qresult, "items requested worker", "failed to commit database transaction");

    const IdList feedIds = feedsIdTitleMap.keys();

    // cleaning feeds by deleting items over threshold
    // but check for valid configuration object first

    if (Q_LIKELY(m_config)) {

        if (Q_LIKELY(!feedIds.isEmpty())) {

            IdList iIds; // item IDs
            for (qint64 fId : feedIds) {

                const FuotenEnums::ItemDeletionStrategy delStrat = m_config->getPerFeedDeletionStrategy(fId);
                const quint16 delVal = m_config->getPerFeedDeletionValue(fId);

                if ((delStrat != FuotenEnums::NoItemDeletion) && (delVal > 0)) {

                    if (delStrat == FuotenEnums::DeleteItemsByCount) {

                        qresult = q.prepare(QStringLiteral("SELECT id FROM items WHERE feedId = ? AND starred = 0 ORDER BY id DESC"));
                        Q_ASSERT_X(qresult, "items requested worker", "failed to preparey querying item IDs from database");

                        q.addBindValue(fId);

                        qresult = q.exec();
                        Q_ASSERT_X(qresult, "items requested worker", "failed to execute querying item IDs from database");

                        iIds.clear();
                        while (q.next()) {
                            iIds.append(q.value(0).toLongLong());
                        }

                        if (iIds.count() > delVal) {

                            qDebug("Removing all items from feed with ID %lli, keeping only %i most recent items.", fId, delVal);

                            QStringList iIdsToDelete;
                            for (int i = 0; i < iIds.count(); ++i) {
                                if (i >= delVal) {
                                    iIdsToDelete.append(QString::number(iIds.at(i)));
                                    removedItemIds.append(iIds.at(i));
                                }
                            }

                            if (!iIdsToDelete.isEmpty()) {
                                qresult = q.exec(QStringLiteral("DELETE FROM items WHERE id IN (%1)").arg(iIdsToDelete.join(QChar(','))));
                                Q_ASSERT_X(qresult, "items requested worker", "failed to delete items from database");
                            }
                        }

                    } else {

                        const QDateTime tt = QDateTime::currentDateTimeUtc().addDays(delVal * -1);

                        qDebug("Removing all items older thant %s from the feed with ID %lli.", qUtf8Printable(tt.toString(Qt::ISODate)), fId);

                        qresult = q.prepare(QStringLiteral("SELECT id FROM items WHERE feedId = ? AND starred = 0 AND pubDate < ?"));
                        Q_ASSERT_X(qresult, "items requested worker", "failed to prepare selecting item IDs from database");

                        q.addBindValue(fId);
                        q.addBindValue(tt.toTime_t());

                        qresult = q.exec();
                        Q_ASSERT_X(qresult, "items requested worker", "failed to selecting item IDs from database");

                        while (q.next()) {
                            removedItemIds.append(q.value(0).toLongLong());
                        }

                        qresult = q.prepare(QStringLiteral("DELETE FROM items WHERE feedId = ? AND starred = 0 AND pubDate < ?"));
                        Q_ASSERT_X(qresult, "items requested worker", "failed to prepare item deletion from database");

                        q.addBindValue(fId);
                        q.addBindValue(tt.toTime_t());

                        qresult = q.exec();
                        Q_ASSERT_X(qresult, "items requested worker", "failed to execute item deletion from database");
                    }
                }
            }
        }
    }

    if (!feedIds.empty()) {
        qresult = m_db.transaction();
        Q_ASSERT(qresult);
        qresult = q.prepare(QStringLiteral("UPDATE feeds SET unreadCount = (SELECT COUNT(id) FROM items WHERE unread = 1 AND feedId = :feedId) WHERE id = :feedId"));
        Q_ASSERT(qresult);
        for (const qint64 id : feedIds) {
            q.bindValue(QStringLiteral(":feedId"), id);
            qresult = q.exec();
            Q_ASSERT(qresult);
        }
        qresult = m_db.commit();
        Q_ASSERT(qresult);
    }

    IdList folderIds;
    qresult = q.exec(QStringLiteral("SELECT id FROM folders"));
    Q_ASSERT(qresult);
    while (q.next()) {
        folderIds.push_back(q.value(0).value<qint64>());
    }

    if (!folderIds.empty()) {
        qresult = m_db.transaction();
        Q_ASSERT(qresult);
        qresult = q.prepare(QStringLiteral("UPDATE folders SET unreadCount = (SELECT SUM(unreadCount) FROM feeds WHERE folderId = :folderId) WHERE id = :folderId"));
        Q_ASSERT(qresult);
        for (const qint64 id : folderIds) {
            q.bindValue(QStringLiteral(":folderId"), id);
            qresult = q.exec();
            Q_ASSERT(qresult);
        }
        qresult = m_db.commit();
        Q_ASSERT(qresult);
    }

    qresult = (q.exec(QStringLiteral(SEL_TOTAL_UNREAD)) && q.next());
    Q_ASSERT_X(qresult, "items requested worker", "failed to select total unread item count from database");
    Q_EMIT gotTotalUnread(q.value(0).toInt());

    qresult = (q.exec(QStringLiteral(SEL_TOTAL_STARRED)) && q.next());
    Q_ASSERT_X(qresult, "items requested worker", "failed to select total starred item count from database");
    Q_EMIT gotStarred(q.value(0).toInt());

    Q_EMIT requestedItems(updatedItemIds, newItemIds, removedItemIds);

    if (publishArticles && !articlesToPublish.empty()) {
        for (auto i = articlesToPublish.constBegin(); i != articlesToPublish.constEnd(); ++i) {
            const QJsonObject o = *i;
            if (!removedItemIds.contains(o.value(QStringLiteral("id")).toVariant().value<qint64>())) {
                m_notificator->publishArticle(o, feedsIdTitleMap.value(o.value(QStringLiteral("feedId")).toVariant().value<qint64>()));
            }
        }
    }

    if (m_notificator && (newUnreadItems > 0)) {
        m_notificator->notify(AbstractNotificator::ItemsRequested, QtInfoMsg, newUnreadItems);
    }
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

    ItemsRequestedWorker *worker = new ItemsRequestedWorker(d->db.databaseName(), json, configuration(), notificator(), this);
    connect(worker, &ItemsRequestedWorker::requestedItems, this, &SQLiteStorage::requestedItems);
    connect(worker, &ItemsRequestedWorker::gotStarred, this, &SQLiteStorage::setStarred);
    connect(worker, &ItemsRequestedWorker::gotTotalUnread, this, &SQLiteStorage::setTotalUnread);
    connect(worker, &ItemsRequestedWorker::failed, this, [=] (Error *e) {setError(e);});
    connect(worker, &QThread::finished, worker, &QObject::deleteLater);
    worker->start();
}



void SQLiteStorage::itemsMarked(const IdList &itemIds, bool unread)
{
    qDebug("%s", "Start to mark items as read in the local storage.");

    if (!ready()) {
        //% "SQLite database not ready. Can not process requested data."
        setError(new Error(Error::StorageError, Error::Warning, qtTrId("libfuoten-err-sqlite-db-not-ready"), QString(), this));
        notify(error());
        return;
    }

    if (itemIds.isEmpty()) {
        qWarning("List of marked articles is empty.");
        return;
    }

    Q_D(SQLiteStorage);

    const QString idListString = d->intListToString(itemIds);

    QSqlQuery q(d->db);
    q.setForwardOnly(true);
    bool qresult = q.prepare(QStringLiteral("UPDATE items SET unread = ?, lastModified = ? WHERE id IN (%1)").arg(idListString));
    Q_ASSERT_X(qresult, "items marked", "failed to prepare database query");

    q.addBindValue(unread);
    q.addBindValue(QDateTime::currentDateTimeUtc().toTime_t());

    qresult = q.exec();
    Q_ASSERT_X(qresult, "items marked", "failed to execute databae query");

    qDebug("Updated items in the database that have been marked as %s", unread ? "unread" : "read");

    qresult = q.exec(QStringLiteral("SELECT DISTINCT feedId FROM items WHERE id IN (%1)").arg(idListString));
    Q_ASSERT(qresult);
    IdList feedIds;
    while (q.next()) {
        feedIds.push_back(q.value(0).value<qint64>());
    }
    qDebug("Selected affected feeds after items in the database have been marked as %s.", unread ? "unread" : "read");
    if (!feedIds.empty()) {
        qresult = d->db.transaction();
        Q_ASSERT(qresult);
        for (const qint64 id : feedIds) {
            qresult = q.prepare(QStringLiteral("UPDATE feeds SET unreadCount = (SELECT COUNT(id) FROM items WHERE unread = 1 AND feedId = :feedId) WHERE id = :feedId"));
            Q_ASSERT(qresult);
            q.bindValue(QStringLiteral(":feedId"), id);
            qresult = q.exec();
            Q_ASSERT(qresult);
        }
        qresult = d->db.commit();
        Q_ASSERT(qresult);
    }
    qDebug("Updated affected feeds after items in the database have been marked as %s.", unread ? "unread" : "read");

    qresult = q.exec(QStringLiteral("SELECT DISTINCT folderId FROM feeds WHERE id IN (%1)").arg(d->intListToString(feedIds)));
    Q_ASSERT(qresult);
    IdList folderIds;
    while (q.next()) {
        folderIds.push_back(q.value(0).value<qint64>());
    }
    qDebug("Selected affected folders after items in the database have been marked as %s.", unread ? "unread" : "read");
    if (!folderIds.empty()) {
        qresult = d->db.transaction();
        Q_ASSERT(qresult);
        for (const qint64 id : folderIds) {
            qresult = q.prepare(QStringLiteral("UPDATE folders SET unreadCount = (SELECT SUM(unreadCount) FROM feeds WHERE folderId = :folderId) WHERE id = :folderId"));
            Q_ASSERT(qresult);
            q.bindValue(QStringLiteral(":folderId"), id);
            qresult = q.exec();
            Q_ASSERT(qresult);
        }
        qresult = d->db.commit();
        Q_ASSERT(qresult);
    }
    qDebug("Updated affected folders after items in the database have been marked as %s.", unread ? "unread" : "read");

    qresult = (q.exec(QStringLiteral(SEL_TOTAL_UNREAD)) && q.next());
    Q_ASSERT(qresult);
    setTotalUnread(q.value(0).value<quint16>());
    qDebug("Updated total count of unread items.");

    Q_EMIT markedItems(itemIds, unread);
}


void SQLiteStorage::itemsStarred(const QList<QPair<qint64, QString> > &articles, bool star)
{
    if (!ready()) {
        //% "SQLite database not ready. Can not process requested data."
        setError(new Error(Error::StorageError, Error::Warning, qtTrId("libfuoten-err-sqlite-db-not-ready"), QString(), this));
        notify(error());
        return;
    }

    if (articles.isEmpty()) {
        qWarning("No articles in the list. Can not update local storage.");
    }

    Q_D(SQLiteStorage);

    QSqlQuery q(d->db);
    q.setForwardOnly(true);

    bool qresult = d->db.transaction();
    Q_ASSERT_X(qresult, "items starred", "failed to start database transaction");

    for (const QPair<qint64,QString> &p : articles) {

        qresult = q.prepare(QStringLiteral("UPDATE items SET starred = ?, lastModified = ? WHERE feedId = ? and guidHash = ?"));
        Q_ASSERT_X(qresult, "items starred", "failed to prepare updating item in database");

        q.addBindValue(star);
        q.addBindValue(QDateTime::currentDateTimeUtc().toTime_t());
        q.addBindValue(p.first);
        q.addBindValue(p.second);

        qresult = q.exec();
        Q_ASSERT_X(qresult, "items starred", "failed to execute updating item in database");

    }

    qresult = d->db.commit();
    Q_ASSERT_X(qresult, "items starred", "failed to commit database transaction");

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
        notify(error());
        return;
    }

    Q_D(SQLiteStorage);

    QSqlQuery q(d->db);

    bool qresult = q.prepare(QStringLiteral("UPDATE items SET unread = ?, lastModified = ? WHERE id = ?"));
    Q_ASSERT_X(qresult, "item marked", "failed to prepare database transaction");

    q.addBindValue(unread);
    q.addBindValue(QDateTime::currentDateTimeUtc().toTime_t());
    q.addBindValue(itemId);

    qresult = q.exec();
    Q_ASSERT_X(qresult, "item marked", "failed to execute database transaction");

    qresult = q.prepare(QStringLiteral("SELECT it.feedId, fe.folderId FROM items it JOIN feeds fe ON fe.id = it.feedId WHERE it.id = ?"));
    Q_ASSERT(qresult);
    q.addBindValue(itemId);
    qresult = (q.exec() && q.next());
    Q_ASSERT(qresult);

    const qint64 feedId = q.value(0).value<qint64>();
    const qint64 folderId = q.value(1).value<qint64>();

    qresult = q.prepare(QStringLiteral("UPDATE feeds SET unreadCount = unreadCount + ? WHERE id = ?"));
    Q_ASSERT(qresult);
    q.addBindValue(unread ? 1 : -1);
    q.addBindValue(feedId);
    qresult = q.exec();
    Q_ASSERT(qresult);

    qresult = q.prepare(QStringLiteral("UPDATE folders SET unreadCount = unreadCount + ? WHERE id = ?"));
    Q_ASSERT(qresult);
    q.addBindValue(unread ? 1 : -1);
    q.addBindValue(folderId);
    qresult = q.exec();
    Q_ASSERT(qresult);

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
        notify(error());
        return;
    }

    Q_D(SQLiteStorage);

    QSqlQuery q(d->db);

    bool qresult = q.prepare(QStringLiteral("UPDATE items SET starred = ?, lastModified = ? WHERE feedId = ? and guidHash = ?"));
    Q_ASSERT_X(qresult, "item starred", "failed to prepare database transaction");

    q.addBindValue(star);
    q.addBindValue(QDateTime::currentDateTimeUtc().toTime_t());
    q.addBindValue(feedId);
    q.addBindValue(guidHash);

    qresult = q.exec();
    Q_ASSERT_X(qresult, "item starred", "failed to execute database transaction");

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
        notify(error());
        return;
    }

    Q_D(SQLiteStorage);

    QSqlQuery q(d->db);

    bool qresult = q.prepare(QStringLiteral("UPDATE items SET unread = 0, lastModified = ? WHERE id <= ?"));
    Q_ASSERT_X(qresult, "all items marked read", "failed to prepare database transaciton");

    q.addBindValue(newestItemId);

    qresult = q.exec();
    Q_ASSERT_X(qresult, "all items marked read", "failed to execute database transaction");

    qresult = q.exec(QStringLiteral("UPDATE feeds SET unreadCount = 0"));
    Q_ASSERT(qresult);

    qresult = q.exec(QStringLiteral("UPDATE folders SET unreadCount = 0"));
    Q_ASSERT(qresult);

    setTotalUnread(0);

    Q_EMIT markedAllItemsRead(newestItemId);
}



QString SQLiteStorage::getArticleBody(qint64 id)
{
    QString body;

    if (!ready()) {
        //% "SQLite database not ready. Can not process requested data."
        setError(new Error(Error::StorageError, Error::Warning, qtTrId("libfuoten-err-sqlite-db-not-ready"), QString(), this));
        notify(error());
        return body;
    }

    Q_D(SQLiteStorage);

    QSqlQuery q(d->db);

    bool qresult = q.prepare(QStringLiteral("SELECT body FROM items WHERE id = ?"));
    Q_ASSERT_X(qresult, "get article body", "failed to prepare database transaction");

    q.addBindValue(id);

    qresult = q.exec();
    Q_ASSERT_X(qresult, "get article body", "failed to execute database query");

    if (Q_LIKELY(q.next())) {
        body = q.value(0).toString();
    }

    return body;
}



bool SQLiteStorage::enqueueItem(FuotenEnums::QueueAction action, Article *article)
{
    if (!ready()) {
        //% "SQLite database not ready. Can not process requested data."
        setError(new Error(Error::StorageError, Error::Warning, qtTrId("libfuoten-err-sqlite-db-not-ready"), QString(), this));
        notify(error());
        return false;
    }

    if (!article) {
        //% "Invalid article object."
        setError(new Error(Error::ApplicationError, Error::Critical, qtTrId("libfuoten-err-invalid-article-object"), QString(), this));
        notify(error());
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

    bool qresult = q.prepare(qs);
    Q_ASSERT_X(qresult, "enqueue item", "failed to prepare datbase query");

    q.addBindValue(QDateTime::currentDateTimeUtc().toTime_t()-10);
    q.addBindValue(static_cast<int>(aq));

    if ((action == FuotenEnums::MarkAsUnread) || (action == FuotenEnums::MarkAsRead)) {
        q.addBindValue(article->id());
    } else {
        q.addBindValue(article->feedId());
        q.addBindValue(article->guidHash());
    }

    qresult = q.exec();
    Q_ASSERT_X(qresult, "enqueue item", "failed to execute database query");

    article->setQueue(aq);

    if ((action == FuotenEnums::MarkAsUnread) || (action == FuotenEnums::MarkAsRead)) {
        qresult = q.prepare(QStringLiteral("UPDATE feeds SET unreadCount = unreadCount + ? WHERE id = ?"));
        Q_ASSERT(qresult);
        q.addBindValue((action == FuotenEnums::MarkAsUnread) ? 1 : -1);
        q.addBindValue(article->feedId());
        qresult = q.exec();
        Q_ASSERT(qresult);

        qresult = q.prepare(QStringLiteral("UPDATE folders SET unreadCount = unreadCount + ? WHERE id = ?"));
        Q_ASSERT(qresult);
        q.addBindValue((action == FuotenEnums::MarkAsUnread) ? 1 : -1);
        q.addBindValue(article->folderId());
        qresult = q.exec();
        Q_ASSERT(qresult);
    }

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



EnqueueMarkReadWorker::EnqueueMarkReadWorker(const QString &dbpath, qint64 id, FuotenEnums::Type idType, qint64 newestItemId, QObject *parent) :
    QThread(parent), m_id(id), m_newestItemId(newestItemId), m_idType(idType)
{
    if (!QSqlDatabase::connectionNames().contains(QStringLiteral("fuotendb"))) {
        m_db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), QStringLiteral("fuotendb"));
        m_db.setDatabaseName(dbpath);
    } else {
        m_db = QSqlDatabase::database(QStringLiteral("fuotendb"));
    }
}


EnqueueMarkReadWorker::~EnqueueMarkReadWorker()
{

}


void EnqueueMarkReadWorker::run()
{
    QSqlQuery q(m_db);
    q.setForwardOnly(true);

    bool qresult = q.exec(QStringLiteral("PRAGMA foreign_keys = ON"));
    Q_ASSERT_X(qresult, "enqueue mark read worker", "failed to enable foreign keys support");

    QString qs; // query string

    switch(m_idType) {
    case FuotenEnums::Feed:
        qs = QStringLiteral("SELECT id, queue FROM items WHERE unread = 1 AND id <= ? AND feedId = ?");
        break;
    case FuotenEnums::Folder:
        qs = QStringLiteral("SELECT id, queue FROM items WHERE unread = 1 AND id <= ? AND feedId IN (SELECT id FROM feeds WHERE folderId = ?)");
        break;
    case FuotenEnums::All:
        qs = QStringLiteral("SELECT id, queue FROM items WHERE unread = 1");
        break;
    default:
        //% "Invalid ID type."
        Q_EMIT failed(new Error(Error::ApplicationError, Error::Critical, qtTrId("libfuoten-err-invalid-id-type"), QString(), this));
        return;
    }

    qresult = q.prepare(qs);
    Q_ASSERT_X(qresult, "enqueue mark read worker", "failed to prepare database query");

    if (m_idType != FuotenEnums::All) {
        q.addBindValue(m_newestItemId);
        q.addBindValue(m_id);
    }

    qresult = q.exec();
    Q_ASSERT_X(qresult, "enqueue mark read worker", "failed to execute database query");

    QHash<qint64,FuotenEnums::QueueActions> idsAndQueue;

    while (q.next()) {
        idsAndQueue.insert(q.value(0).toLongLong(), FuotenEnums::QueueActions(q.value(1).toInt()));
    }

    if (idsAndQueue.isEmpty()) {
        qWarning("No items found.");
        return;
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

    qresult = m_db.transaction();
    Q_ASSERT_X(qresult, "equeue mark read worker", "failed to start database transaction");

    QHash<qint64,FuotenEnums::QueueActions>::const_iterator ii = idsAndQueueUpdated.constBegin();
    while (ii != idsAndQueueUpdated.constEnd()) {

        qresult = q.prepare(QStringLiteral("UPDATE items SET unread = 0, queue = ? WHERE id = ?"));
        Q_ASSERT_X(qresult, "enqueue mark read worker", "failed to prepary database query");

        q.addBindValue(static_cast<int>(ii.value()));
        q.addBindValue(ii.key());

        qresult = q.exec();
        Q_ASSERT_X(qresult, "enqueue mark read worker", "failed to execute database query");

        ++ii;
    }

    qresult = m_db.commit();
    Q_ASSERT_X(qresult, "enqueue mark read worker", "failed to commit database transaction");

    qresult = q.exec(QStringLiteral("SELECT id FROM feeds"));
    Q_ASSERT(qresult);
    IdList feedIds;
    while (q.next()) {
        feedIds.push_back(q.value(0).value<qint64>());
    }
    if (!feedIds.empty()) {
        qresult = m_db.transaction();
        Q_ASSERT(qresult);
        for (const qint64 id : feedIds) {
            qresult = q.prepare(QStringLiteral("UPDATE feeds SET unreadCount = (SELECT COUNT(id) FROM items WHERE unread = 1 AND feedId = :feedId) WHERE id = :feedId"));
            Q_ASSERT(qresult);
            q.bindValue(QStringLiteral(":feedId"), id);
            qresult = q.exec();
            Q_ASSERT(qresult);
        }
        qresult = m_db.commit();
        Q_ASSERT(qresult);
    }

    qresult = q.exec(QStringLiteral("SELECT id FROM folders"));
    Q_ASSERT(qresult);
    IdList folderIds;
    while (q.next()) {
        folderIds.push_back(q.value(0).value<qint64>());
    }
    if (!folderIds.empty()) {
        qresult = m_db.transaction();
        Q_ASSERT(qresult);
        for (const qint64 id : folderIds) {
            qresult = q.prepare(QStringLiteral("UPDATE folders SET unreadCount = (SELECT SUM(unreadCount) FROM feeds WHERE folderId = :folderId) WHERE id = :folderId"));
            Q_ASSERT(qresult);
            q.bindValue(QStringLiteral(":folderId"), id);
            qresult = q.exec();
            Q_ASSERT(qresult);
        }
        qresult = m_db.commit();
        Q_ASSERT(qresult);
    }

    qresult = (q.exec(QStringLiteral(SEL_TOTAL_UNREAD)) && q.next());
    Q_ASSERT_X(qresult, "enqueue mark read worker", "failed to query totol unread item count from database");
    Q_EMIT gotTotalUnread(q.value(0).toInt());

    switch (m_idType) {
    case FuotenEnums::Feed:
        Q_EMIT markedReadFeedInQueue(m_id, m_newestItemId);
        break;
    case FuotenEnums::Folder:
        Q_EMIT markedReadFolderInQueue(m_id, m_newestItemId);
        break;
    default:
        Q_EMIT markedAllItemsReadInQueue();
        break;
    }
}



bool SQLiteStorage::enqueueMarkFeedRead(qint64 feedId, qint64 newestItemId)
{
    if (inOperation()) {
        qWarning("Still in operation. Returning.");
        return false;
    }

    if (!ready()) {
        //% "SQLite database not ready. Can not process requested data."
        setError(new Error(Error::StorageError, Error::Warning, qtTrId("libfuoten-err-sqlite-db-not-ready"), QString(), this));
        notify(error());
        return false;
    }

    if (feedId <= 0) {
        //% "The feed ID is not valid."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-invalid-feed-id"), QString(), this));
        notify(error());
        return false;
    }

    if (newestItemId <= 0) {
        //% "The item ID is not valid."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-invalid-item-id"), QString(), this));
        notify(error());
        return false;
    }

    setInOperation(true);

    Q_D(SQLiteStorage);

    EnqueueMarkReadWorker *worker = new EnqueueMarkReadWorker(d->db.databaseName(), feedId, FuotenEnums::Feed, newestItemId, this);
    connect(worker, &EnqueueMarkReadWorker::markedReadFeedInQueue, this, &SQLiteStorage::markedReadFeedInQueue);
    connect(worker, &EnqueueMarkReadWorker::gotTotalUnread, this, &SQLiteStorage::setTotalUnread);
    connect(worker, &EnqueueMarkReadWorker::failed, this, [=] (Error *e) {setError(e);});
    connect(worker, &QThread::finished, this, [=] () {setInOperation(false);});
    connect(worker, &QThread::finished, worker, &QObject::deleteLater);
    worker->start();

    return true;
}



bool SQLiteStorage::enqueueMarkFolderRead(qint64 folderId, qint64 newestItemId)
{
    if (inOperation()) {
        qWarning("Still in operation. Returning.");
        return false;
    }

    if (!ready()) {
        //% "SQLite database not ready. Can not process requested data."
        setError(new Error(Error::StorageError, Error::Warning, qtTrId("libfuoten-err-sqlite-db-not-ready"), QString(), this));
        notify(error());
        return false;
    }

    if (folderId < 0) {
        //% "The folder ID is not valid."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-invalid-folder-id"), QString(), this));
        notify(error());
        return false;
    }

    if (newestItemId <= 0) {
        //% "The item ID is not valid."
        setError(new Error(Error::InputError, Error::Critical, qtTrId("libfuoten-err-invalid-item-id"), QString(), this));
        notify(error());
        return false;
    }

    setInOperation(true);

    Q_D(SQLiteStorage);

    EnqueueMarkReadWorker *worker = new EnqueueMarkReadWorker(d->db.databaseName(), folderId, FuotenEnums::Folder, newestItemId, this);
    connect(worker, &EnqueueMarkReadWorker::markedReadFolderInQueue, this, &SQLiteStorage::markedReadFolderInQueue);
    connect(worker, &EnqueueMarkReadWorker::gotTotalUnread, this, &SQLiteStorage::setTotalUnread);
    connect(worker, &EnqueueMarkReadWorker::failed, this, [=] (Error *e) {setError(e);});
    connect(worker, &QThread::finished, this, [=] () {setInOperation(false);});
    connect(worker, &QThread::finished, worker, &QObject::deleteLater);
    worker->start();

    return true;
}



bool SQLiteStorage::enqueueMarkAllItemsRead()
{
    if (inOperation()) {
        qWarning("Still in operation. Returning.");
        return false;
    }

    if (!ready()) {
        //% "SQLite database not ready. Can not process requested data."
        setError(new Error(Error::StorageError, Error::Warning, qtTrId("libfuoten-err-sqlite-db-not-ready"), QString(), this));
        notify(error());
        return false;
    }

    setInOperation(true);

    Q_D(SQLiteStorage);

    EnqueueMarkReadWorker *worker = new EnqueueMarkReadWorker(d->db.databaseName(), 0, FuotenEnums::All, -1, this);
    connect(worker, &EnqueueMarkReadWorker::markedAllItemsReadInQueue, this, &SQLiteStorage::markedAllItemsReadInQueue);
    connect(worker, &EnqueueMarkReadWorker::gotTotalUnread, this, &SQLiteStorage::setTotalUnread);
    connect(worker, &EnqueueMarkReadWorker::failed, this, [=] (Error *e) {setError(e);});
    connect(worker, &QThread::finished, this, [=] () {setInOperation(false);});
    connect(worker, &QThread::finished, worker, &QObject::deleteLater);
    worker->start();

    return true;
}



ClearQueueWorker::ClearQueueWorker(const QString &dbpath, QObject *parent) :
    QThread(parent)
{
    if (!QSqlDatabase::connectionNames().contains(QStringLiteral("fuotendb"))) {
        m_db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), QStringLiteral("fuotendb"));
        m_db.setDatabaseName(dbpath);
    } else {
        m_db = QSqlDatabase::database(QStringLiteral("fuotendb"));
    }
}


ClearQueueWorker::~ClearQueueWorker()
{

}


void ClearQueueWorker::run()
{
    QSqlQuery q(m_db);

    bool qresult = q.exec(QStringLiteral("PRAGMA foreign_keys = ON"));
    Q_ASSERT_X(qresult, "clear queue worker", "failed to enable foreign keys support");

    qresult = q.exec(QStringLiteral("UPDATE items SET queue = 0"));
    Q_ASSERT_X(qresult, "clear queue worker", "failed to execute databae query");

    Q_EMIT queueCleared();
}



void SQLiteStorage::clearQueue()
{
    if (inOperation()) {
        qWarning("Still in operation. Returning.");
        return;
    }

    if (!ready()) {
        //% "SQLite database not ready. Can not process requested data."
        setError(new Error(Error::StorageError, Error::Warning, qtTrId("libfuoten-err-sqlite-db-not-ready"), QString(), this));
        notify(error());
        return;
    }

    setInOperation(true);

    Q_D(SQLiteStorage);

    ClearQueueWorker *worker = new ClearQueueWorker(d->db.databaseName(), this);
    connect(worker, &ClearQueueWorker::queueCleared, this, &AbstractStorage::queueCleared);
    connect(worker, &ClearQueueWorker::failed, this, [=] (Error *e) {setError(e);});
    connect(worker, &QThread::finished, this, [=] () {setInOperation(false);});
    connect(worker, &QThread::finished, worker, &QObject::deleteLater);
    worker->start();
}

void SQLiteStorage::clearStorage()
{
    Q_ASSERT_X(configuration(), "clear storage", "no configuration available");

    if (inOperation()) {
        qWarning("%s", "Still in operation. Returning.");
        return;
    }

    if (!ready()) {
        setError(new Error(Error::StorageError, Error::Warning, qtTrId("libfuoten-err-sqlite-db-not-ready"), QString(), this));
        return;
    }

    qDebug("%s", "Start clearing local storage.");

    setInOperation(true);

    Q_D(SQLiteStorage);

    QSqlQuery q(d->db);

    if (Q_UNLIKELY(!q.exec(QStringLiteral("DROP VIEW IF EXISTS total_starred")))) {
        setError(new Error(q.lastError(), QString(), this));
        setInOperation(false);
        return;
    }

    if (Q_UNLIKELY(!q.exec(QStringLiteral("DROP VIEW IF EXISTS total_unread")))) {
        setError(new Error(q.lastError(), QString(), this));
        setInOperation(false);
        return;
    }

    if (Q_UNLIKELY(!q.exec(QStringLiteral("DROP INDEX IF EXISTS items_feed_id_index")))) {
        setError(new Error(q.lastError(), QString(), this));
        setInOperation(false);
        return;
    }

    if (Q_UNLIKELY(!q.exec(QStringLiteral("DROP INDEX IF EXISTS items_item_guid")))) {
        setError(new Error(q.lastError(), QString(), this));
        setInOperation(false);
        return;
    }

    if (Q_UNLIKELY(!q.exec(QStringLiteral("DROP INDEX IF EXISTS feeds_folder_id_index")))) {
        setError(new Error(q.lastError(), QString(), this));
        setInOperation(false);
        return;
    }

    if (Q_UNLIKELY(!q.exec(QStringLiteral("DROP TABLE IF EXISTS items")))) {
        setError(new Error(q.lastError(), QString(), this));
        setInOperation(false);
        return;
    }

    if (Q_UNLIKELY(!q.exec(QStringLiteral("DROP TABLE IF EXISTS feeds")))) {
        setError(new Error(q.lastError(), QString(), this));
        setInOperation(false);
        return;
    }

    if (Q_UNLIKELY(!q.exec(QStringLiteral("DROP TABLE IF EXISTS folders")))) {
        setError(new Error(q.lastError(), QString(), this));
        setInOperation(false);
        return;
    }

    if (Q_UNLIKELY(!q.exec(QStringLiteral("DROP TABLE IF EXISTS system")))) {
        setError(new Error(q.lastError(), QString(), this));
        setInOperation(false);
        return;
    }


    configuration()->setLastSync(QDateTime::fromMSecsSinceEpoch(0));

    qDebug("%s", "Finished clearing local storage.");

    setInOperation(false);

    Q_EMIT storageCleared();
}

#include "moc_sqlitestorage.cpp"
