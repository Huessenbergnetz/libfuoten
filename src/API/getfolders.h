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

#ifndef FUOTENGETFOLDERS_H
#define FUOTENGETFOLDERS_H

#include <QObject>
#include "component.h"
#include "../fuoten_global.h"

namespace Fuoten {

class GetFoldersPrivate;

/*!
 * \brief Requests the lists of folders from the API.
 *
 * \headerfile "" <Fuoten/API/GetFolders>
 */
class FUOTENSHARED_EXPORT GetFolders : public Component
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs a new GetFolders object.
     */
    GetFolders(QObject *parent = nullptr);

    /*!
     * \brief Executes the API request.
     */
    Q_INVOKABLE void execute() override;

protected:
    GetFolders(GetFoldersPrivate &dd, QObject *parent = nullptr);

    void successCallback() override;

    void extractError(QNetworkReply *reply) override;

    bool checkOutput() override;

private:
    Q_DECLARE_PRIVATE(GetFolders)
    Q_DISABLE_COPY(GetFolders)
};

}

#endif // FUOTENGETFOLDERS_H
