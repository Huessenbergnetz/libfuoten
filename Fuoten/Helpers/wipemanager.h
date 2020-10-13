/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016-2020 Hüssenbergnetz/Matthias Fehring
 * https://github.com/Huessenbergnetz/libfuoten
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

#ifndef FUOTENWIPEMANAGER_H
#define FUOTENWIPEMANAGER_H

#include <QObject>
#include "../fuoten_global.h"

namespace Fuoten {

class WipeManagerPrivate;
class AbstractConfiguration;
class AbstractStorage;
class AbstractNotificator;

class FUOTENSHARED_EXPORT WipeManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool inOperation READ inOperation NOTIFY inOperationChanged)
public:
    /*!
     * \brief Constructs a new %WipeManager object with the given \a parent.
     */
    explicit WipeManager(QObject *parent = nullptr);

    /*!
     * \brief Destroys the %WipeManager object.
     */
    ~WipeManager() override;

    bool inOperation() const;

    AbstractConfiguration* configuration() const;
    void setConfiguration(AbstractConfiguration *configuration);

    AbstractStorage* storage() const;
    void setStorage(AbstractStorage *storage);

    AbstractNotificator* notificator() const;
    void setNotificator(AbstractNotificator *notificator);

    void checkForWipe();

Q_SIGNALS:
    void inOperationChanged(bool inOperation);
    void wipeStarted();
    void wipeFailed();
    void wipeDone();

private:
    const QScopedPointer<WipeManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(WipeManager)
    Q_DISABLE_COPY(WipeManager)
    Q_DISABLE_MOVE(WipeManager)
};

}

#endif // FUOTENWIPEMANAGER_H
