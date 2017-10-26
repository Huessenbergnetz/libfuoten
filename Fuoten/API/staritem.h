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

#ifndef FUOTENSTARITEM_H
#define FUOTENSTARITEM_H

#include <QObject>
#include "component.h"
#include "../fuoten_global.h"

namespace Fuoten {

class StarItemPrivate;

/*!
 * \brief Stars or unstars an item/article on the remote News App.
 *
 * To star or unstar an item/article, you have to set StarItem::feedId to a valid feed ID, StarItem::guidHash to a valid item/article guid hash and Component::configuration
 * to a valid AbstractConfiguration derived object that provides the authentication credentials and server information. After setting the mandatory properties,
 * call execute() to perform the API request.
 *
 * If a valid AbstractStorage object is set to the Component::storage property, AbstractStorage::itemStarred() will be called in the successCallback() to
 * update the local storage. If the request succeeded, the StarItem::succeeded() signal will be emitted, containing the \a feedId, the \a guidHash and the \a starred status.
 * If something failed, the Component::failed() signal will be emitted and Component::error will contain a valid pointer to an Error object.
 *
 * \par Mandatory properties
 * StarItem::feedId, StarItem::guidHash, Component::configuration
 *
 * \par API route
 * /folders/{feedId}/{guidHash}/{star|unstar}
 *
 * \par Method
 * PUT
 *
 * \headerfile "" <Fuoten/API/StarItem>
 */
class FUOTENSHARED_EXPORT StarItem : public Component
{
    Q_OBJECT
    /*!
     * \brief ID of the feed the item/article to star/unstar belongs to.
     *
     * This property can not be changed while Component::inOperation() returns \c true.
     *
     * \par Access functions:
     * <TABLE><TR><TD>qint64</TD><TD>feedId() const</TD></TR><TR><TD>void</TD><TD>setFeedId(qint64 nFeedId)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>feedIdChanged(qint64 feedId)</TD></TR></TABLE>
     */
    Q_PROPERTY(qint64 feedId READ feedId WRITE setFeedId NOTIFY feedIdChanged)
    /*!
     * \brief GUID hash of the item/article to star/unstar.
     *
     * This property can not be changed while Component::inOperation() returns \c true.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>guidHash() const</TD></TR><TR><TD>void</TD><TD>setGuidHash(const QString &nGuidHash)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>guidHashChanged(const QString &guidHash)</TD></TR></TABLE>
     */
    Q_PROPERTY(QString guidHash READ guidHash WRITE setGuidHash NOTIFY guidHashChanged)
    /*!
     * \brief True if the item/article should be starred, otherwise false.
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
     * \brief Constructs a new StarItem object with default values and the given \a parent.
     */
    explicit StarItem(QObject *parent = nullptr);

    /*!
     * \brief Constructs a new StarItem object with the given arguments and \a parent.
     */
    StarItem(qint64 feedId, const QString &guidHash, bool starred, QObject *parent = nullptr);

    /*!
     * \brief Getter function for the \link StarItem::feedId feedId \endlink property.
     * \sa StarItem::setFeedId(), StarItem::feedIdChanged()
     * \return the ID of the feed the item/article that should be starred/unstarred belongs to
     */
    qint64 feedId() const;
    /*!
     * \brief Getter function for the \link StarItem::guidHash guidHash \endlink property.
     * \sa StarItem::setGuidHash(), StarItem::guidHashChanged()
     * \return the GUID hash of the item/article that should be starred/ustarred
     */
    QString guidHash() const;
    /*!
     * \brief Getter function for the \link StarItem::starred starred \endlink property.
     * \sa StarItem::setStarred(), StarItem::starredChanged()
     * \return \c true if the item/article should be starred, \c false if it should be unstarred
     */
    bool starred() const;

    /*!
     * \brief Setter function for the \link StarItem::feedId feedId \endlink property.
     * Emits the feedIdChanged() signal if \a nFeedId is not equal to the stored value.
     * Can not be changed while Component::inOperation returns true.
     *
     * \sa StarItem::feedId(), StarItem::feedIdChanged()
     * \param nFeedId ID of the feed the item that should be starred/unstarred belongs to
     */
    void setFeedId(qint64 nFeedId);
    /*!
     * \brief Setter function for the \link StarItem::guidHash guidHash \endlink property.
     * Emits the guidHashChanged() signal if \a nGuidHash is not equal to the stored value.
     * Can not be changed while Component::inOperation returns true.
     *
     * \sa StarItem::guidHash(), StarItem::guidHashChanged()
     * \param nGuidHash the GUID hash of the article that should be starred/unstarred
     */
    void setGuidHash(const QString &nGuidHash);
    /*!
     * \brief Setter function for the \link StarItem::starred starred \endlink property.
     * Emits the starredChanged() signal if \a nStarred is not equal to the stored value.
     * Can not be changed while Component::inOperation returns true.
     *
     * \sa StarItem::starred(), StarItem::starredChanged()
     * \param nStarred \c true if the article should be starred, \c false if it should be unstarred
     */
    void setStarred(bool nStarred);


    /*!
     * \brief Executes the API request.
     *
     * To perform a successful API request to star or unstar an article/item, StarItem::feedId has to be a valid feed ID, StarItem::guidHash has to be
     * an valid item/article guid hash and there has to be an AbstractConfiguration object set to Component::configuration.
     *
     * Execution will not run while Component::inOperation returns \c true and will itself set that property to \c true when the request starts.
     */
    Q_INVOKABLE void execute() override;


Q_SIGNALS:
    /*!
     * \brief This is emitted if the value of the \link StarItem::feedId feedId \endlink property changes.
     * \sa StarItem::feedId(), StarItem::setFeedId()
     * \param feedId ID of the feed the item that should be starred/unstarred belongs to
     */
    void feedIdChanged(qint64 feedId);
    /*!
     * \brief This is emitted if the value of the \link StarItem::guidHash guidHash \endlink property changes.
     * \sa StarItem::guidHash(), StarItem::setGuidHash()
     * \param guidHash the GUID hash of the article that should be starred/unstarred
     */
    void guidHashChanged(const QString &guidHash);
    /*!
     * \brief This is emitted if the value of the \link StarItem::starred starred \endlink property changes.
     * \sa StarItem::starred(), StarItem::setStarred()
     * \param starred \c true if the article should be starred, \c false if it should be unstarred
     */
    void starredChanged(bool starred);

    /*!
     * \brief This signal is emitted if the request to star/unstar an item/article was successful.
     * \param feedId    ID of the feed the starred/unstarred item/article belongs to
     * \param guidHash  GUID hash of the item/article that has been starred/unstarred
     * \param starred   \c true if the article has been starred, \c false if it has been unstarred
     */
    void succeeded(qint64 feedId, const QString &guidHash, bool starred);

protected:
    StarItem(StarItemPrivate &dd, QObject *parent = nullptr);

    /*!
     * \brief Finishes the article starring/unstarring if the request was successful.
     *
     * If Component::storage contains a valid pointer, the AbstractStorage::itemStarred() function will be called to
     * update the local storage. After that the Component::inOperation property will be set to false and the succeeded() signal
     * will be emitted.
     */
    void successCallback() override;

    /*!
     * \brief Will check for valid \a feedId and non-empty \a guidHash.
     *
     * Will at first perform the checks of Component::checkInput(). Will than simply check if the \a feedId is not lower or equal to \c 0
     * and if the \a guidHash is not empty.
     */
    bool checkInput() override;

    /*!
     * \brief Extracts possible errors replied by the News App API.
     */
    void extractError(QNetworkReply *reply) override;

private:
    Q_DISABLE_COPY(StarItem)
    Q_DECLARE_PRIVATE(StarItem)

};

}


#endif // FUOTENSTARITEM_H
