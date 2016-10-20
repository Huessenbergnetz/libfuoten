/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016 Buschtrommel / Matthias Fehring
 * https://www.buschmann23.de/entwicklung/bibliotheken/libfuoten/
 * https://github.com/Buschtrommel/libfuoten
 *
 * Folders/getfolders.h
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

#ifndef GETFOLDERS_H
#define GETFOLDERS_H

#include <QObject>
#include "../component.h"
#include "../fuoten_global.h"

namespace Fuoten {

class GetFoldersPrivate;

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
    Q_INVOKABLE void execute() Q_DECL_OVERRIDE;

protected:
    GetFolders(GetFoldersPrivate &dd, QObject *parent = nullptr);

    void successCallback() Q_DECL_OVERRIDE;

    void extractError(QNetworkReply *reply) Q_DECL_OVERRIDE;

    bool checkOutput() Q_DECL_OVERRIDE;

private:
    Q_DECLARE_PRIVATE(GetFolders)
    Q_DISABLE_COPY(GetFolders)
};

}

#endif // GETFOLDERS_H
