/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016-2019 Hüssenbergnetz/Matthias Fehring
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

#ifndef FUOTENSTARMULTIPLEITEMS_H
#define FUOTENSTARMULTIPLEITEMS_H

#include <QObject>
#include <QList>
#include <QPair>
#include "component.h"
#include "../fuoten_global.h"

namespace Fuoten {

class StarMultipleItemsPrivate;

/*!
 * \brief Star/unstar multiple items/articles in on request on the News App server.
 *
 * To star multiple items/articles in one request, you have to set a list of feed ID and item guid hash combinations via setItemsToStar(), or add single
 * combinations via addItem(). You need also to set a valid AbstractConfiguration derived class to Component::configurtion to provide the necessary
 * authentication credentials and server settings. After setting the mandatory propeties, call execute() to perform the API request.
 *
 * If a valid AbstractStorage object is set to the Component::storage property, AbstractStorage::itemsStarred() will be called in the successCallback() to
 * update the local storage. If the request succeeded, the StarMultipleItems::succeeded() signal will be emitted, containing the the list of articles and
 * the \a starred status.
 * If something failed, the Component::failed() signal will be emitted and Component::error will contain a valid pointer to an Error object.
 *
 * \par Mandatory properties
 * Component::configuration
 *
 * \par API route
 * /itesm/{star|unstar}/multiple
 *
 * \par Method
 * PUT
 *
 * \headerfile "" <Fuoten/API/MarkFeedRead>
 */
class FUOTENSHARED_EXPORT StarMultipleItems : public Component
{
    Q_OBJECT
    /*!
     * \brief Set to \c true if the items/articles should be marked as starred, set to \c false to mark them as unstarred.
     *
     * This property can not be changed while Component::inOperation() returns \c true.
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>starred() const</TD></TR><TR><TD>void</TD><TD>setStarred(bool nStarred)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>starredChanged(bool starred)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool starred READ starred WRITE setStarred NOTIFY starredChanged)
public:
    /*!
     * \brief Constructs a new StarMultipleItems object with default values and the given \a parent.
     */
    explicit StarMultipleItems(QObject *parent = nullptr);

    /*!
     * \brief Constructs a new StarMultipleItems object with the given arguments and \a parent.
     */
    explicit StarMultipleItems(bool starred, QObject *parent = nullptr);

    /*!
     * \brief Destroys the %StarMultipleItems object.
     */
    ~StarMultipleItems() override;

    /*!
     * \brief Getter function for the \link StarMultipleItems::starred starred \endlink property.
     * \sa StarMultipleItems::setStarred(), StarMultipleItems::starredChanged()
     */
    bool starred() const;

    /*!
     * \brief Setter function for the \link StarMultipleItems::starred starred \endlink property.
     * Emits the starredChanged() signal if \a nStarred is not equal to the stored value.
     * \sa StarMultipleItems::starred(), StarMultipleItems::starredChanged()
     */
    void setStarred(bool nStarred);

    /*!
     * \brief Returns the list of feed IDs and article GUID hashes that should be starred/unstarred.
     */
    QList<QPair<qint64, QString> > itemsToStar() const;

    /*!
     * \brief Sets the list of feed IDs and article GUID hashes that shoul be starred/unstarred.
     *
     * This can not be set while Component::inOperation() returns \c true.
     *
     * \param items  list containing a set of feed ID as \a first and article guid hash as \a second
     */
    void setItemsToStar(const QList<QPair<qint64,QString>> &items);

    /*!
     * \brief Adds a new article to the list of articles to star/unstar.
     *
     * Items can not be added while Component::inOperation() returns \c true.
     *
     * \param feedId    ID of the feed the article belongs to
     * \param guidHash  GUID hash of the article
     */
    Q_INVOKABLE void addItem(qint64 feedId, const QString &guidHash);

    /*!
     * \brief Executes the API request.
     *
     * To perform a successful API request to star/unstar a list of articles/items, you have to setItemsToStar()
     * and there has to be an AbstractConfiguration object set to Component::configuration.
     *
     * Execution will not run while Component::inOperation returns \c true and will itself set that property to \c true when the request starts.
     */
    Q_INVOKABLE void execute() override;

Q_SIGNALS:
    /*!
     * \brief This is emitted if the value of the \link StarMultipleItems::starred starred \endlink property changes.
     * \sa StarMultipleItems::starred(), StarMultipleItems::setStarred()
     */
    void starredChanged(bool starred);

    /*!
     * \brief This signal is emitted if the request to star/unstar items was successful.
     * \param items list of feed IDs and article/item GUID hashes that have been starred/unstarred
     * \param star  \c true if the articles/items have been starred, \c false if they have been unstarred
     */
    void succeeded(const QList<QPair<qint64,QString>> &items, bool star);

protected:
    StarMultipleItems(StarMultipleItemsPrivate &dd, QObject *parent = nullptr);

    /*!
     * \brief Finishes the article/items starring/unstarring if the request was successful.
     *
     * If Component::storage contains a valid pointer, the AbstractStorage::itemsStarred() function will be called to
     * update the local storage. After that the Component::inOperation property will be set to false and the succeeded() signal
     * will be emitted.
     */
    void successCallback() override;

    /*!
     * \brief Will check if the list of articles/items is not empty.
     *
     * Will at first perform the checks of Component::checkInput(). Will than simply check if list of articles is not empty.
     */
    bool checkInput() override;

private:
    Q_DISABLE_COPY(StarMultipleItems)
    Q_DISABLE_MOVE(StarMultipleItems)
    Q_DECLARE_PRIVATE(StarMultipleItems)
};

}


#endif // FUOTENSTARMULTIPLEITEMS_H
