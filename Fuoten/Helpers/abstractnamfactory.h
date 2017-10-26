/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016-2017 Matthias Fehring
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

#ifndef FUOTENABSTRACTNAMFACTORY_H
#define FUOTENABSTRACTNAMFACTORY_H

#include <QObject>
class QNetworkAccessManager;

namespace Fuoten {

/*!
 * \brief The AbstractNamFactory class creates QNetworkAccessManager instances for Fuoten API classes.
 *
 * Fuoten API classes use QNetworkAccessManager for all network access. By implementing a factory, it
 * is possible to provide the API classes with custom QNetworkAccessManager instances with specialized
 * caching, proxy and cookies support.
 *
 * To implement a factory, subclass AbstractNamFactory and implement the virtual create() method, then
 * assign it to the Fuoten API classes engine using Component::setNetworkAccessManagerFactory().
 *
 * \headerfile "" <Fuoten/Helpers/AbstractNamFactory>
 */
class AbstractNamFactory
{
    Q_DISABLE_COPY(AbstractNamFactory)
public:
    /*!
     * \brief Destroys the factory. The default implementation does nothing.
     */
    virtual ~AbstractNamFactory();

    /*!
     * \brief Creates and returns a network access manager with the specified \a parent.
     * This method must return a new QNetworkAccessManager instance each time it is called.
     */
    virtual QNetworkAccessManager *create(QObject *parent) = 0;
};

}

#endif // FUOTENABSTRACTNAMFACTORY_H
