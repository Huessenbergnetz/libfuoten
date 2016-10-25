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

#ifndef SQLITESTORAGE_H
#define SQLITESTORAGE_H

#include <QObject>
#include "abstractstorage.h"
#include "../fuoten.h"
#include "../fuoten_global.h"

namespace Fuoten {

class SQLiteStoragePrivate;
class Folder;

/*!
 * \brief Storage using a local SQLite database.
 *
 * To use this storage, simply set the path to the SQLite database file in the constructor and call init().
 * The path to the database file will not be created automatically. It has to be created before calling init().
 *
 * If you want to have a custom storage class, derive from AbstractStorage.
 *
 * \headerfile "" <Fuoten/Storage/SQLiteStorage>
 */
class FUOTENSHARED_EXPORT SQLiteStorage final : public AbstractStorage
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs a new SQLiteStorage object using \a dbpath.
     */
    SQLiteStorage(const QString &dbpath, QObject *parent = nullptr);

    /*!
     * \brief Initializes the SQLite database.
     *
     * This will create/check the table layout.
     */
    void init() override;

    QList<Folder*> getFolders(FuotenEnums::SortingRole sortingRole = FuotenEnums::Name, Qt::SortOrder sortOrder = Qt::AscendingOrder, const QList<qint64> &ids = QList<qint64>()) override;

public Q_SLOTS:
    void foldersRequested(const QJsonDocument &json) override;
    void folderCreated(const QJsonDocument &json) override;
    void folderRenamed(qint64 id, const QString &newName) override;
    void folderDeleted(qint64 id) override;
    void folderMarkedRead(qint64 id, qint64 newestItem);

private:
    Q_DECLARE_PRIVATE(SQLiteStorage)
    Q_DISABLE_COPY(SQLiteStorage)
};

}

#endif // SQLITESTORAGE_H
