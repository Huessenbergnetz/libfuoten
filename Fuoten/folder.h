/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
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
public:
    /*!
     * \brief Constructs a new empty %Folder object with the given \a parent.
     */
    explicit Folder(QObject *parent = nullptr);

    /*!
     * \brief Deconstructs the %Folder object.
     */
    ~Folder() override;

    /*!
     * \brief Constructs a new Folder object from the given arguments.
     */
    Folder(qint64 id, const QString &name, uint feedCount, uint unreadCount, QObject *parent = nullptr);

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
     * \brief Sets a new name for the folder on the remote server.
     *
     * Will also rename the folder in the local \a storage if a valid AbstractStorage object has been set.
     *
     * This function is invokable from QML.
     */
    Q_INVOKABLE void rename(const QString &newName, Fuoten::AbstractConfiguration *config, Fuoten::AbstractStorage *storage = nullptr);

    /*!
     * \brief Sets a new name for the folder on the remote server.
     * \overload
     *
     * This uses Component::defaultConfiguration() and Component::defaultStorage()
     */
    Q_INVOKABLE void rename(const QString &newName);

    /*!
     * \brief Removes this folder from the remote server.
     *
     * Will also remove the folder from the local \a storage if a valid AbstractStorage object has been set.
     *
     * This function is invokable from QML.
     */
    Q_INVOKABLE void remove(Fuoten::AbstractConfiguration *config, Fuoten::AbstractStorage *storage = nullptr);

    /*!
     * \brief Removes this folder from the remote server.
     * \overload
     *
     * This uses Component::defaultConfiguration() and Component::defaultStorage()
     */
    Q_INVOKABLE void remove();

    /*!
     * \brief Marks the complete folder as read on the remote server and local.
     *
     * \param config    pointer to an AbstractConfiguration subclass to get the account configuration
     * \param storage   pointer to an AbstractStorage subclass to query the newest item ID in the folder and update the local storage
     * \param enqueue   \c true to enqueue the marking local up to the next sync, valid \c storage has to be available
     */
    Q_INVOKABLE void markAsRead(Fuoten::AbstractConfiguration * config, Fuoten::AbstractStorage *storage, bool enqueue = false);

    /*!
     * \brief Marks the complete folder as read on the remote server and local.
     * \overload
     *
     * This uses Component::defaultConfiguration() and Component::defaultStorage()
     */
    Q_INVOKABLE void markAsRead(bool enqueue = false);

    /*!
     * \brief Makes a deep copy of \a other.
     *
     * \a other has to be a Folder object.
     */
    void copy(BaseItem *other) override;

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

protected:
    Folder(FolderPrivate &dd, QObject *parent = nullptr);

private:
    Q_DISABLE_COPY(Folder)
    Q_DECLARE_PRIVATE(Folder)
};

}


#endif // FUOTENFOLDER_H
