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
class Configuration;
class AbstractStorage;

/*!
 * \brief Combines updating of folders, feeds and items.
 *
 * \headerfile "" <Fuoten/Helpers/Synchronizer>
 */
class FUOTENSHARED_EXPORT Synchronizer : public QObject
{
    Q_OBJECT
    /*!
     * \brief Pointer to an Error object, if an error occured. Otherwise a nullptr.
     *
     * \par Access functions:
     * <TABLE><TR><TD>Error*</TD><TD>error() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>errorChanged(Error *error)</TD></TR></TABLE>
     */
    Q_PROPERTY(Fuoten::Error *error READ error NOTIFY errorChanged)
    /*!
     * \brief Pointer to a Configuration object.
     *
     * You have to set a valid configuration object in order to start an update.
     *
     * \par Access functions:
     * <TABLE><TR><TD>Configuration*</TD><TD>configuration() const</TD></TR><TR><TD>void</TD><TD>setConfiguration(Configuration *nConfiguration)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>configurationChanged(Configuration *configuration)</TD></TR></TABLE>
     */
    Q_PROPERTY(Fuoten::Configuration *configuration READ configuration WRITE setConfiguration NOTIFY configurationChanged)
    /*!
     * \brief Pointer to a AbstractStorage derived object.
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
     * \brief Constructs a new Synchronizer object.
     */
    explicit Synchronizer(QObject *parent = nullptr);

    /*!
     * \brief Deconstructs the Synchronizer object.
     */
    ~Synchronizer();

    Error *error() const;
    Configuration *configuration() const;
    AbstractStorage *storage() const;
    bool inOperation() const;

    void setConfiguration(Configuration *nConfiguration);
    void setStorage(AbstractStorage *nStorageHandler);

    /*!
     * \brief Invokes the synchronizing process.
     *
     * The base implementation simply calls start().
     */
    Q_INVOKABLE virtual void sync();


Q_SIGNALS:
    void errorChanged(Error *error);
    void configurationChanged(Configuration *configuration);
    void storageChanged(AbstractStorage *storage);
    void inOperationChanged(bool inOperation);

    void succeeded();
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

private:
    Q_DISABLE_COPY(Synchronizer)
    Q_DECLARE_PRIVATE(Synchronizer)

};

}


#endif // FUOTENSYNCHRONIZER_H
