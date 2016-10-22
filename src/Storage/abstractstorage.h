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
 * from Component. Also BaseItem hast the BaseItem::storage property for derived class,
 * that is used to set the storage handler to API classes that perfrom API actions.
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

    bool ready() const;
    Error *error() const;

    /*!
     * \brief Initializes the storage handler.
     *
     * For example, create your database layout. When finished set setReady() to \a true.
     */
    virtual void init() = 0;

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
     * \brief Returns a list of Folder objects from the local storage.
     */
    virtual QList<Folder*> getFolders(const QList<quint64> &ids = QList<quint64>(), FuotenEnums::SortingRole sortingRole = FuotenEnums::Name, Qt::SortOrder sortOrder = Qt::AscendingOrder) const = 0;

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
    void requestedFolders(const QList<QPair<quint64, QString> > &updatedFolders, const QList<QPair<quint64, QString> > &newFolders, const QList<quint64> deletedFolders);

    /*!
     * \brief Emit this after a new folder has been created.
     *
     * Best loaction to emit this signal is your implementation of folderCreated(). The signal
     * should contain the \c id and the \c name of the new folder.
     */
    void createdFolder(quint64 id, const QString &name);

    void readyChanged(bool ready);
    void errorChanged(Error *error);


private:
    Q_DISABLE_COPY(AbstractStorage)
    Q_DECLARE_PRIVATE(AbstractStorage)
};

}

#endif // FUOTENABSTRACTSTORAGE_H
