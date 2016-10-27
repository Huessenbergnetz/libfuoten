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

#ifndef SQLITESTORAGE_P
#define SQLITESTORAGE_P

#include "sqlitestorage.h"
#include "abstractstorage_p.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QStringList>
#include <QThread>

#include <QtDebug>

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
    void failed(Error *error);
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

    QStringList intListToStringList(const QList<qint64> &ints) const
    {
        QStringList sl;

        if (!ints.isEmpty()) {

            for (qint64 i : ints) {
                sl.append(QString::number(i));
            }

        }

        return sl;
    }

    QString intListToString(const QList<qint64> &ints) const
    {
        return intListToStringList(ints).join(QChar(','));
    }

    QSqlDatabase db;
};

}

#endif // SQLITESTORAGE_P

