/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENABSTRACTNAMFACTORY_H
#define FUOTENABSTRACTNAMFACTORY_H

#include <QObject>
#include "fuoten_export.h"

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
class FUOTEN_EXPORT AbstractNamFactory
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
