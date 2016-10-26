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
     * <TABLE><TR><TD>void</TD><TD>itemCountChanged(uint itemCount)</TD></TR></TABLE>
     *
     * \sa setItemCount()
     */
    Q_PROPERTY(uint itemCount READ itemCount NOTIFY itemCountChanged)
public:
    /*!
     * \brief Constructs a new empty Folder object.
     */
    explicit Folder(QObject *parent = nullptr);


    /*!
     * \brief Constructs a new Folder object from the given arguments.
     */
    Folder(qint64 id, const QString &name, uint feedCount, uint unreadCount, uint itemCount, QObject *parent = nullptr);

    /*!
     * \brief Returns the name of the folder.
     *
     * \sa Folder::name
     */
    QString name() const;

    /*!
     * \brief Returns the number of feeds contained in this folder.
     *
     * \sa Folder::feedCount
     */
    uint feedCount() const;

    /*!
     * \brief Returns the number of unread items in this folder.
     *
     * \sa Folder::unreadCount
     */
    uint unreadCount() const;

    /*!
     * \brief Returns the total number of items in this folder.
     *
     * \sa Folder::itemCount
     */
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
    void setItemCount(uint nItemCount);

    /*!
     * \brief Sets a new name for the folder on the remote server.
     *
     * Will also rename the folder in the local \a storage if a valid AbstractStorage object has been set.
     *
     * This function is invokable from QML.
     */
    Q_INVOKABLE void rename(const QString &newName, Fuoten::Configuration *config, Fuoten::AbstractStorage *storage = nullptr);

    /*!
     * \brief Removes this folder from the remote server.
     *
     * Will also remove the folder from the local \a storage if a valid AbstractStorage object has been set.
     *
     * This function is invokable from QML.
     */
    Q_INVOKABLE void remove(Fuoten::Configuration *config, Fuoten::AbstractStorage *storage = nullptr);

Q_SIGNALS:
    /*!
     * \brief This signal will be emitted if the folder name changes.
     * \sa Folder::name
     */
    void nameChanged(const QString &name);

    /*!
     * \brief This signal will be emitted if the number of feeds in the folder changes.
     * \sa Folder::feedCount
     */
    void feedCountChanged(uint feedCount);

    /*!
     * \brief This signal will be emitted if the number of unread items in the folder changes.
     * \sa Folder:unreadCount
     */
    void unreadCountChanged(uint unreadCount);

    /*!
     * \brief This signal will be emitted if then total number of items in the folder changes.
     * \sa Folder::itemCount
     */
    void itemCountChanged(uint itemCount);

protected:
    Folder(FolderPrivate &dd, QObject *parent = nullptr);

private:
    Q_DISABLE_COPY(Folder)
    Q_DECLARE_PRIVATE(Folder)

};

}


#endif // FUOTENFOLDER_H
