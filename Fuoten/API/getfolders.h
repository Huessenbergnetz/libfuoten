/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENGETFOLDERS_H
#define FUOTENGETFOLDERS_H

#include <QObject>
#include "component.h"
#include "../fuoten_global.h"
#include "fuoten_export.h"

namespace Fuoten {

class GetFoldersPrivate;

/*!
 * \brief Requests the list of folders from the News App API.
 *
 * To request the folder list, the Component::configuration property has to be set to a valid AbstractConfiguration object. After setting it,
 * call execute() to perform the API request.
 *
 * If a valid AbstractStorage object is set to the Component::storage property, AbstractStorage::foldersRequested will be called in the successCallback()
 * to save the requested folders in the local storage. If the request succeeded, the Component::succeeded() signal will be emitted, containing the JSON api
 * reply.
 *
 * If something failed, Component::failed() will be emitted and the Component::error property will contain a valid pointer to an Error object.
 *
 * \par Mandatory properties
 * Component::configuration
 *
 * \par API route
 * /folders
 *
 * \par Method
 * GET
 *
 * \sa Synchronizer
 * \headerfile "" <Fuoten/API/GetFolders>
 */
class FUOTEN_EXPORT GetFolders : public Component
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs an API request object with the given \a parent to query folders from the remote server.
     */
    explicit GetFolders(QObject *parent = nullptr);

    /*!
     * \brief Destroys the %GetFolders object.
     */
    ~GetFolders() override;

    /*!
     * \brief Executes the API request.
     *
     * To perform a successful API request, Component::configuration has to be set to a valid AbstractConfiguration object.
     *
     * Execution will not run if Component::inOperation returns \c true and will itself set that property to \c true when start to perform
     * the request.
     */
    Q_INVOKABLE void execute() override;

protected:
    GetFolders(GetFoldersPrivate &dd, QObject *parent = nullptr);

    /*!
     * \brief Finishes the the operation if the request was successful.
     *
     * If Component::storage points to a valid object, it will use AbstractStorage::foldersRequested() to store, update and delete the
     * folders in the local storage according to the server reply. Afterwards it will set Component::inOperation to false and will emit
     * the Component::succeeded() signal.
     */
    void successCallback() override;

    /*!
     * \brief Checks for a \a folders array in the JSON API reply.
     *
     * Will at first perform the checks from Component::checkOutput() and will than check if the \a folders array is present.
     * Will \b not check if the array is empty.
     */
    bool checkOutput() override;

private:
    Q_DECLARE_PRIVATE(GetFolders)
    Q_DISABLE_COPY(GetFolders)
};

}

#endif // FUOTENGETFOLDERS_H
