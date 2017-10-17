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

#ifndef FUOTENABSTRACTNOTIFICATOR_H
#define FUOTENABSTRACTNOTIFICATOR_H

#include <QObject>
#include "../fuoten_global.h"

class QJsonObject;

namespace Fuoten {

class AbstractNotificatorPrivate;
class Article;

class FUOTENSHARED_EXPORT AbstractNotificator : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(AbstractNotificator)
    Q_DECLARE_PRIVATE(AbstractNotificator)
public:
    explicit AbstractNotificator(QObject *parent = nullptr);
    ~AbstractNotificator();

    QString appIcon() const;
    void setAppIcon(const QString &appIcon);

    QString appName() const;
    void setAppName(const QString &appName);

    bool isEnabled() const;
    void setEnabled(bool enabled);

    enum Type {
        NetworkError = 0,
        ParsingError,
        StorageError,
        ReplyError,
        InputError,
        AuthenticationError,
        SyncComplete,
        FoldersRequested,
        FolderCreated,
        FolderDeleted,
        FolderRenamed,
        FolderMarkedRead,
        FeedsRequested,
        FeedCreated,
        FeedDeleted,
        FeedMoved,
        FeedRenamed,
        FeedMarkedRead,
        ItemsRequested
    };

    virtual void notify(Type type, QtMsgType severity, const QVariant &data, bool force = false) = 0;

    virtual void publishArticle(const QJsonObject &article, qint64 feedId = -1, const QString &feedName = QString()) = 0;

    virtual void publishArticle(Article *article) = 0;

Q_SIGNALS:
    void appIconChanged(const QString &appIcon);
    void appNameChanged(const QString &appName);
    void enabledChanged(bool enabled);

protected:
    const QScopedPointer<AbstractNotificatorPrivate> d_ptr;
};

}

#endif // FUOTENABSTRACTNOTIFICATOR_H
