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

#ifndef FUOTENABSTRACTSTORAGE_H
#define FUOTENABSTRACTSTORAGE_H

#include <QObject>
#include "../fuoten.h"
#include "../fuoten_global.h"

namespace Fuoten {

class Folder;
class Error;
class AbstractStoragePrivate;

/*!
 * \brief Abstract class to handle the storage of requested News App data.
 *
 * Reimplement this class to store the data requested from the News App API. You can set your
 * derived class to the Component::storage property, that all classes use that derive
 * from Component. Also BaseItem hast the BaseItem::storage property for derived classes,
 * that is used to set the storage handler to API classes that perfrom API actions.
 *
 * If you derive from AbstractStorage, initialize the storage in init() and set setReady() to true
 * when your storage is ready to handle data.
 *
 * \headerfile "" <Fuoten/Storage/AbstractStorage>
 */
class FUOTENSHARED_EXPORT AbstractStorage : public QObject
{
    Q_OBJECT
    /*!
     * \brief Returns true when the storage handler has finished its initialization.
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>ready() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>readyChanged(bool ready)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool ready READ ready NOTIFY readyChanged)
    /*!
     * \brief Returns a pointer to an Error object, if any error occures, otherwise a nullptr.
     *
     * \par Access functions:
     * <TABLE><TR><TD>Error*</TD><TD>error() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>errorChanged(Error *error)</TD></TR></TABLE>
     */
    Q_PROPERTY(Fuoten::Error *error READ error NOTIFY errorChanged)
    /*!
     * \brief Total amount of unread items in the storage.
     *
     * \par Access functions:
     * <TABLE><TR><TD>quint16</TD><TD>totalUnread() const</TD></TR><TR><TD>void</TD><TD>setTotalUnread(quint16 nTotalUnread)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>totalUnreadChanged(quint16 totalUnread)</TD></TR></TABLE>
     */
    Q_PROPERTY(quint16 totalUnread READ totalUnread WRITE setTotalUnread NOTIFY totalUnreadChanged)
    /*!
     * \brief Amount of starred items.
     *
     * \par Access functions:
     * <TABLE><TR><TD>quint16</TD><TD>starred() const</TD></TR><TR><TD>void</TD><TD>setStarred(quint16 nStarred)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>starredChanged(quint16 starred)</TD></TR></TABLE>
     */
    Q_PROPERTY(quint16 starred READ starred WRITE setStarred NOTIFY starredChanged)
public:
    /*!
     * \brief Constructs a new AbstractStorage object.
     *
     * SotrageHandler is an abstract class, you can not instantiate it. You have to create a derived class.
     */
    AbstractStorage(QObject *parent = nullptr);

    /*!
     * \brief Deconstructs the AbstractStorage object.
     */
    ~AbstractStorage();

    /*!
     * \brief Returns true when the storage is ready to handle data, otherwise false.
     */
    bool ready() const;

    /*!
     * \brief Retruns a pointer to an Error object, if any error occures, otherwise returns a nullptr.
     */
    Error *error() const;

    /*!
     * \brief Initializes the storage handler.
     *
     * For example, create your database layout. When finished set setReady() to \a true.
     */
    virtual void init() = 0;

    virtual quint16 totalUnread() const;
    virtual quint16 starred() const;

    virtual void setTotalUnread(quint16 nTotalUnread);
    virtual void setStarred(quint16 nStarred);

public Q_SLOTS:
    /*!
     * \brief Receives the reply data of the GetFolders request.
     *
     * Implement this in a derived class to store folder data, for example in a local SQL database.
     * You may want to emit requestedFolders() in your implementation after you processed the data.
     *
     * \par Example JSON response data
     *
     * \code{.json}
     * {
     *     "folders": [
     *         {
     *             "id": 1,
     *             "name": "Interersting Folder"
     *         },
     *         {
     *             "id": 2,
     *             "name": "Other Folder"
     *         }, // etc
     *     ]
     * }
     * \endcode
     */
    virtual void foldersRequested(const QJsonDocument &json) = 0;

    /*!
     * \brief Receives the reply data of the CreateFolder request.
     *
     * Implement this in a derived class to store new folder data, for example in a local SQL databse.
     * You my want to emit createdFolder() in your implementation after you processed the data.
     *
     * \par Example JSON response data
     *
     * \code{.json}
     * {
     *     "folders": [
     *         {
     *             "id": 3,
     *             "name": "New Folder"
     *         }
     *     ]
     * }
     * \endcode
     */
    virtual void folderCreated(const QJsonDocument &json) = 0;

    /*!
     * \brief Receives the reply data of the RenameFolder request.
     *
     * Will rename the folder in the local storage and emits the renamedFolder() signal.
     */
    virtual void folderRenamed(quint64 id, const QString &newName) = 0;

    /*!
     * \brief Receives the reply data of the DeleteFolder request.
     *
     * Will delete the folder in the local storage and emits the deletedFolder() signal.
     */
    virtual void folderDeleted(quint64 id) = 0;

    /*!
     * \brief Returns a list of Folder objects from the local storage.
     *
     * The returned list will be sorted by \a sortingRole and \a sortOrder. If \a ids is not empty,
     * only folders with IDs from the list will be returned. The Folder objects in the returned list will
     * have their parent set to \c nullptr.
     */
    virtual QList<Folder*> getFolders(FuotenEnums::SortingRole sortingRole = FuotenEnums::Name, Qt::SortOrder sortOrder = Qt::AscendingOrder, const QList<quint64> &ids = QList<quint64>()) = 0;

protected:
    /*!
     * \brief Set this to \a true when the storage has finished it's initialization.
     */
    void setReady(bool nReady);

    /*!
     * \brief Sets the Error object.
     */
    void setError(Error *nError);

    const QScopedPointer<AbstractStoragePrivate> d_ptr;
    AbstractStorage(AbstractStoragePrivate &dd, QObject *parent = nullptr);

Q_SIGNALS:
    /*!
     * \brief Emit this after folders have been received and processed.
     *
     * Best location to emit this signal is your implementation of foldersRequested().
     *
     * \c updatedFolders should contain a list of the database IDs and names of updated folders, \c newFolders should
     * contain a list of databaes IDs and names of new folders, and \c deletedFolders should contain a list of database IDs
     * of deleted folders.
     */
    void requestedFolders(const QList<QPair<quint64, QString> > &updatedFolders, const QList<QPair<quint64, QString> > &newFolders, const QList<quint64> &deletedFolders);

    /*!
     * \brief Emit this after a new folder has been created.
     *
     * Best loaction to emit this signal is your implementation of folderCreated(). The signal
     * has to contain the \a id and the \a name of the new folder.
     */
    void createdFolder(quint64 id, const QString &name);

    /*!
     * \brief Emit this after a folder has been renamed.
     *
     * Best location to emit this signal is your implementation of folderRenamed(). The signal
     * has to contain the \a id and the \a newName of the folder.
     */
    void renamedFolder(quint64 id, const QString &newName);

    /*!
     * \brief Emit this after a folder has been deleted.
     *
     * Best location to emit this signal is your implementation of folderDeleted(). The signal
     * has to contain the \a id fo the deleted folder.
     */
    void deletedFolder(quint64 id);

    /*!
     * \brief Emitted whenever the ready property changes.
     */
    void readyChanged(bool ready);

    /*!
     * \brief Emitted whenever the error property changes.
     */
    void errorChanged(Error *error);

    void totalUnreadChanged(quint16 totalUnread);
    void starredChanged(quint16 starred);


private:
    Q_DISABLE_COPY(AbstractStorage)
    Q_DECLARE_PRIVATE(AbstractStorage)
};

}

#endif // FUOTENABSTRACTSTORAGE_H
