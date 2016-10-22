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

#ifndef FUOTENFOLDER_H
#define FUOTENFOLDER_H

#include <QObject>
#include "baseitem.h"
#include "fuoten_global.h"



namespace Fuoten {

class FolderPrivate;

/*!
 * \brief Contains information about a single folder.
 *
 * \headerfile "" <Fuoten/Folder>
 */
class FUOTENSHARED_EXPORT Folder : public BaseItem
{
    Q_OBJECT
    /*!
     * \brief The name of the folder.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>name() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>nameChanged(const QString &name)</TD></TR></TABLE>
     *
     * \sa setName()
     */
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    /*!
     * \brief Returns the number of feeds conained in this folder.
     *
     * \par Access functions:
     * <TABLE><TR><TD>uint</TD><TD>feedCount() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>feedCountChanged(uint feedCount)</TD></TR></TABLE>
     *
     * \sa setFeedCount()
     */
    Q_PROPERTY(uint feedCount READ feedCount NOTIFY feedCountChanged)
    /*!
     * \brief Returns the number of unread items in this folder.
     *
     * \par Access functions:
     * <TABLE><TR><TD>uint</TD><TD>unreadCount() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>unreadCountChanged(uint unreadCount)</TD></TR></TABLE>
     *
     * \sa setUnreadCount()
     */
    Q_PROPERTY(uint unreadCount READ unreadCount NOTIFY unreadCountChanged)
    /*!
     * \brief Returns the number of items in this folder.
     *
     * \par Access functions:
     * <TABLE><TR><TD>uint</TD><TD>itemCount() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>itemCountChanged(const uint &itemCount)</TD></TR></TABLE>
     *
     * \sa setItemCount()
     */
    Q_PROPERTY(uint itemCount READ itemCount NOTIFY itemCountChanged)
public:
    /*!
     * \brief Constructs a new empty Folder object.
     */
    Folder(QObject *parent = nullptr);

    /*!
     * \brief Constructs a new Folder object from a QJsonDocument.
     */
    Folder(const QJsonDocument &json, QObject *parent = nullptr);

    /*!
     * \brief Constructs a new Folder object from a QJsonObject.
     */
    Folder(const QJsonObject &json, QObject *parent = nullptr);

    /*!
     * \brief Constructs a new Folder object from the given arguments.
     */
    Folder(quint64 id, const QString &name, uint feedCount, uint unreadCount, uint itemCount, QObject *parent = nullptr);

    QString name() const;
    uint feedCount() const;
    uint unreadCount() const;
    uint itemCount() const;

    /*!
     * \brief Sets the name of the folder.
     *
     * \sa name
     */
    void setName(const QString &nName);

    /*!
     * \brief Sets the number of feeds the folder contains.
     *
     * \sa feedCount
     */
    void setFeedCount(uint nFeedCount);

    /*!
     * \brief Sets the number of unread items in this folder.
     *
     * \sa unreadCount
     */
    void setUnreadCount(uint nUnreadCount);

    /*!
     * \brief Sets the total number of items in this folder.
     *
     * \sa itemCount
     */
    void setItemCount(const uint &nItemCount);

    Q_INVOKABLE void rename(const QString &nName);

    void loadFromJson(const QJsonDocument &json) Q_DECL_OVERRIDE;

    void loadFromJson(const QJsonObject &json) Q_DECL_OVERRIDE;


Q_SIGNALS:
    void nameChanged(const QString &name);
    void feedCountChanged(uint feedCount);
    void unreadCountChanged(uint unreadCount);
    void itemCountChanged(const uint &itemCount);

protected:
    Folder(FolderPrivate &dd, QObject *parent = nullptr);

private:
    Q_DISABLE_COPY(Folder)
    Q_DECLARE_PRIVATE(Folder)

};

}


#endif // FUOTENFOLDER_H
