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

#ifndef FUOTENSYNCHRONIZER_H
#define FUOTENSYNCHRONIZER_H

#include <QObject>
#include "../fuoten_global.h"

namespace Fuoten {

class SynchronizerPrivate;
class Error;
class AbstractConfiguration;
class AbstractStorage;

/*!
 * \brief Combines updating of folders, feeds and articles.
 *
 * \par Mandatory properties
 * Synchronizer::configuration
 *
 * \headerfile "" <Fuoten/Helpers/Synchronizer>
 */
class FUOTENSHARED_EXPORT Synchronizer : public QObject
{
    Q_OBJECT
    /*!
     * \brief Pointer to an Error object, if an error occurred. Otherwise a \c nullptr.
     *
     * \par Access functions:
     * <TABLE><TR><TD>Error*</TD><TD>error() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>errorChanged(Error *error)</TD></TR></TABLE>
     */
    Q_PROPERTY(Fuoten::Error *error READ error NOTIFY errorChanged)
    /*!
     * \brief Pointer to a AbstractConfiguration object.
     *
     * You have to set a valid AbstractConfiguration object in order to start a synchronization.
     *
     * \par Access functions:
     * <TABLE><TR><TD>AbstractConfiguration*</TD><TD>configuration() const</TD></TR><TR><TD>void</TD><TD>setConfiguration(AbstractConfiguration *nAbstractConfiguration)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>configurationChanged(AbstractConfiguration *configuration)</TD></TR></TABLE>
     */
    Q_PROPERTY(Fuoten::AbstractConfiguration *configuration READ configuration WRITE setConfiguration NOTIFY configurationChanged)
    /*!
     * \brief Pointer to aa AbstractStorage derived object.
     *
     * \par Access functions:
     * <TABLE><TR><TD>AbstractStorage*</TD><TD>storage() const</TD></TR><TR><TD>void</TD><TD>setStorage(AbstractStorage *nStorageHandler)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>storageChanged(AbstractStorage *storage)</TD></TR></TABLE>
     */
    Q_PROPERTY(Fuoten::AbstractStorage *storage READ storage WRITE setStorage NOTIFY storageChanged)
    /*!
     * \brief Returns true while the Synchronizer is in operational mode.
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>inOperation() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>inOperationChanged(bool inOperation)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool inOperation READ inOperation NOTIFY inOperationChanged)
public:
    /*!
     * \brief Constructs a new Synchronizer object with the given \a parent.
     */
    explicit Synchronizer(QObject *parent = nullptr);

    /*!
     * \brief Deconstructs the Synchronizer object.
     */
    ~Synchronizer();

    /*!
     * \brief Returns a pointer to an Error object if any error occurred, otherwise a \c nullptr.
     * \sa error
     */
    Error *error() const;

    /*!
     * \brief Returns the pointer to the currently set AbstractConfiguration object.
     * \sa configuration
     */
    AbstractConfiguration *configuration() const;

    /*!
     * \brief Returns the pointer to the currently set AbstractStorage object, if any, otherwise a \c nullptr.
     */
    AbstractStorage *storage() const;

    /*!
     * \brief Returns true while the synchronization is active.
     * \sa inOperation
     */
    bool inOperation() const;

    /*!
     * \brief Sets the pointer to a AbstractConfiguration object.
     * \sa configuration
     */
    void setConfiguration(AbstractConfiguration *nAbstractConfiguration);

    /*!
     * \brief Sets the pointer to an AbstractStorage object.
     * \sa storage
     */
    void setStorage(AbstractStorage *nStorageHandler);

    /*!
     * \brief Invokes the synchronizing process.
     *
     * The base implementation simply calls start().
     */
    Q_INVOKABLE virtual void sync();


Q_SIGNALS:
    /*!
     * \brief This signal is emitted if the pointer to an Error object changes.
     *
     * Maybe a \c nullptr if the Error object was reset.
     *
     * \sa error
     */
    void errorChanged(Error *error);

    /*!
     * \brief This signal is emitted if the pointer to the AbstractConfiguration object changes.
     * \sa configuration
     */
    void configurationChanged(AbstractConfiguration *configuration);

    /*!
     * \brief This signall is emitted if the pointer to the AbstractStorage object changes.
     * \sa storage
     */
    void storageChanged(AbstractStorage *storage);

    /*!
     * \brief This signal is emitted if the oprational state changes.
     * \sa inOperation
     */
    void inOperationChanged(bool inOperation);

    /*!
     * \brief This signal is emitted if the synchronization process was successful.
     * \sa failed()
     */
    void succeeded();

    /*!
     * \brief This signal is emitted if the synchonization process failed for some reason.
     * \sa succeeded(), error
     */
    void failed(Error *error);

protected:
    const QScopedPointer<SynchronizerPrivate> d_ptr;
    /*!
     * \brief Starts the synchronizing process.
     *
     * Prior to starting the synchronizing process, it checks inOperation(). If that returns \c true,
     * nothing will happen. If inOperation() returns \c false, it will set it to \c true and will
     * start the update process.
     *
     * When you create a derived class, call start() in your implementation of sync().
     */
    void start();

protected Q_SLOTS:
    /*!
     * \brief Sets a new Error object to the error property.
     * \sa error
     */
    void setError(Error *nError);

    /*!
     * \brief Notifies the News App about unread articles from the local queue.
     */
    void notifyAboutUnread();

    /*!
     * \brief Notifies the News App about read articles from the local queue.
     */
    void notifyAboutRead();

    /*!
     * \brief Notifies the News App about starred articles from the local queue.
     */
    void notifyAboutStarred();

    /*!
     * \brief Notifies the News app about unstarred articles from the local queue.
     */
    void notifyAboutUnstarred();

    /*!
     * \brief Requests folder information from the News App.
     */
    void requestFolders();

    /*!
     * \brief Requests feed information from the News App.
     */
    void requestFeeds();

    /*!
     * \brief Requests all unread articles from the News App.
     */
    void requestUnread();

    /*!
     * \brief Requests all starred articles from the News App.
     */
    void requestStarred();

    /*!
     * \brief Requests articles from the News App that are new or have benn updated since last sync.
     */
    void requestUpdated();

    /*!
     * \brief Finishes the synchrinization and cleans up.
     */
    void finished();

private:
    Q_DISABLE_COPY(Synchronizer)
    Q_DECLARE_PRIVATE(Synchronizer)

};

}


#endif // FUOTENSYNCHRONIZER_H
