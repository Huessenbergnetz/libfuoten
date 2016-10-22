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

#ifndef FUOTENBASEITEM_H
#define FUOTENBASEITEM_H

#include <QObject>
#include "fuoten_global.h"

class QJsonDocument;
class QJsonObject;

namespace Fuoten {

class Error;
class Configuration;
class BaseItemPrivate;
class StorageHandler;

/*!
 * \brief Abstract base class for FolderItem, FeedItem and NewsItem.
 *
 * Provides some basic properties used by other items. You have to derive this class and
 * implement the pure virtual funtions.
 *
 * \headerfile "" <Fuoten/BaseItem>
 */
class FUOTENSHARED_EXPORT BaseItem : public QObject
{
    Q_OBJECT
    /*!
     * \brief Returns true while there is an operation running on the item.
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>inOperation() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>inOperationChanged(bool inOperation)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool inOperation READ inOperation NOTIFY inOperationChanged)
    /*!
     * \brief Database ID of the item/feed/folder.
     *
     * \par Access functions:
     * <TABLE><TR><TD>quint64</TD><TD>id() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>idChanged(quint64 id)</TD></TR></TABLE>
     *
     * \sa setId()
     */
    Q_PROPERTY(quint64 id READ id NOTIFY idChanged)
    /*!
     * \brief Pointer to an Error object, if an error occured, otherwise returns a nullptr.
     *
     * \par Access functions:
     * <TABLE><TR><TD>Error*</TD><TD>error() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>errorChanged(Error *error)</TD></TR></TABLE>
     *
     * \sa setError()
     */
    Q_PROPERTY(Fuoten::Error *error READ error NOTIFY errorChanged)
    /*!
     * \brief Pointer to a Configuration object. Mandatory for API calls.
     *
     * \par Access functions:
     * <TABLE><TR><TD>Configuration*</TD><TD>configuration() const</TD></TR><TR><TD>void</TD><TD>setConfiguration(Configuration *nConfiguration)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>configurationChanged(Configuration *configuration)</TD></TR></TABLE>
     */
    Q_PROPERTY(Fuoten::Configuration *configuration READ configuration WRITE setConfiguration NOTIFY configurationChanged)
    /*!
     * \brief Pointer to a StorageHandler object.
     *
     * This will be used to set the Component::storageHandler property if you perform an API action directly from the item class.
     *
     * \par Access functions:
     * <TABLE><TR><TD>StorageHandler*</TD><TD>storageHandler() const</TD></TR><TR><TD>void</TD><TD>setStorageHandler(StorageHandler *nStorageHandler)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>storageHandlerChanged(StorageHandler *storageHandler)</TD></TR></TABLE>
     */
    Q_PROPERTY(Fuoten::StorageHandler *storageHandler READ storageHandler WRITE setStorageHandler NOTIFY storageHandlerChanged)
public:
    /*!
     * \brief Constructs a new BaseItem object.
     */
    BaseItem(QObject *parent = nullptr);

    /*!
     * \brief Deconstructs the BaseItem object.
     */
    ~BaseItem();

    bool inOperation() const;
    quint64 id() const;
    Error *error() const;
    Configuration *configuration() const;
    StorageHandler *storageHandler() const;

    /*!
     * \brief Sets the database ID of the item/feed/folder.
     *
     * \sa id
     */
    void setId(quint64 nId);

    void setConfiguration(Configuration *nConfiguration);
    void setStorageHandler(StorageHandler *nStorageHandler);

    /*!
     * \brief Loads the data from a JSON document into the item/feed/folder.
     */
    virtual void loadFromJson(const QJsonDocument &json) = 0;

    /*!
     * \brief Loads the data from a JSON object into the item/feed/folder.
     */
    virtual void loadFromJson(const QJsonObject &json) = 0;

protected Q_SLOTS:
    void setError(Error *nError);

Q_SIGNALS:
    void inOperationChanged(bool inOperation);
    void idChanged(quint64 id);
    void errorChanged(Error *error);
    void configurationChanged(Configuration *configuration);
    void storageHandlerChanged(StorageHandler *storageHandler);

protected:
    const QScopedPointer<BaseItemPrivate> d_ptr;
    BaseItem(BaseItemPrivate &dd, QObject *parent = nullptr);

private:
    Q_DISABLE_COPY(BaseItem)
    Q_DECLARE_PRIVATE(BaseItem)

};

}


#endif // FUOTENBASEITEM_H
