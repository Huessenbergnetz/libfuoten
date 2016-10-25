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

#ifndef FUOTENBASEITEM_H
#define FUOTENBASEITEM_H

#include <QObject>
#include "fuoten_global.h"

class QJsonDocument;
class QJsonObject;

namespace Fuoten {

class Error;
class Configuration;
class BaseItemPrivate;
class AbstractStorage;
class Component;

/*!
 * \brief Abstract base class for Folder, Feed and Article.
 *
 * Provides some basic properties used by other items. You have to derive this class and
 * implement the pure virtual funtions.
 *
 * \par Usage in models
 *
 * If you are using a class that derives from BaseItem as the content object in a model, do
 * not set the model as parent of the item if you intent to use item internal operations like
 * remove, rename, etc. Setting the model as parent will delete the item when the model will
 * be destructed.
 *
 * Delete the model items manually in the model's destructor and check for items that are still
 * inOperation(), use QObject::deleteLater() to delete these items so that they can finish the
 * ongoing operation.
 *
 * \code{.cpp}
 * while (!folders.isEmpty()) {
 *     Folder *f = folders.takeFirst();
 *     if (f->inOperation()) {
 *         f->deleteLater();
 *     } else {
 *         delete f;
 *     }
 * }
 * \endcode
 *
 * \headerfile "" <Fuoten/BaseItem>
 */
class FUOTENSHARED_EXPORT BaseItem : public QObject
{
    Q_OBJECT
    /*!
     * \brief Returns true while there is an operation running on the item.
     *
     * This is used for operations that are directly invoked by methods from the item,
     * not from outside classes.
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>inOperation() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>inOperationChanged(bool inOperation)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool inOperation READ inOperation NOTIFY inOperationChanged)
    /*!
     * \brief Database ID of the item/feed/folder.
     *
     * \par Access functions:
     * <TABLE><TR><TD>quint64</TD><TD>id() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>idChanged(quint64 id)</TD></TR></TABLE>
     *
     * \sa setId()
     */
    Q_PROPERTY(quint64 id READ id NOTIFY idChanged)
    /*!
     * \brief Pointer to an Error object, if an error occured, otherwise returns a nullptr.
     *
     * \par Access functions:
     * <TABLE><TR><TD>Error*</TD><TD>error() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>errorChanged(Error *error)</TD></TR></TABLE>
     *
     * \sa setError()
     */
    Q_PROPERTY(Fuoten::Error *error READ error NOTIFY errorChanged)
public:
    /*!
     * \brief Constructs a new BaseItem object.
     */
    BaseItem(QObject *parent = nullptr);

    /*!
     * \brief Deconstructs the BaseItem object.
     */
    ~BaseItem();

    /*!
     * \brief Returns true while there is some operation ongoing directly on the item.
     *
     * \sa BaseItem::inOperation
     */
    bool inOperation() const;

    /*!
     * \brief Returns the ID of the item.
     *
     * \sa BaseItem::id
     */
    quint64 id() const;

    /*!
     * \brief Returns a pointer to an Error object, if any error occured, otherwise a \c nullptr.
     *
     * \sa BaseItem::error
     */
    Error *error() const;

    /*!
     * \brief Sets the database ID of the item/feed/folder.
     *
     * \sa id
     */
    void setId(quint64 nId);

    Q_INVOKABLE void clearError();

Q_SIGNALS:
    /*!
     * \brief This signal will be emitted whenever the inOperation property changes.
     * \sa BaseItem::inOperation
     */
    void inOperationChanged(bool inOperation);

    /*!
     * \brief This signal will be emitted whenever the id property changes.
     * \sa BaseItem::id
     */
    void idChanged(quint64 id);

    /*!
     * \brief This signal will be emitted whenever the error property changes. May return a nullptr.
     * \sa BaseItem::error
     */
    void errorChanged(Error *error);

protected:
    const QScopedPointer<BaseItemPrivate> d_ptr;
    BaseItem(BaseItemPrivate &dd, QObject *parent = nullptr);

    /*!
     * \brief Sets the Error object of the error property.
     *
     * \sa error
     */
    void setError(Error *nError);

    /*!
     * \brief Sets the API component to perform an internal API request.
     *
     * This has to be a subclass of Component. Failed requests are handled automatically by connecting
     * to the Component::failed() signal. It will set the BaseItem::error property and will delete the component
     * as well setting the internal pointer to a \c nullptr.
     *
     * Handling of succeeded API requests should be handled in the implementation.
     *
     * \par Example
     *
     * \code{.cpp}
     * void Folder::remove(Configuration *config, AbstractStorage *storage)
     * {
     *     if (!config) {
     *         qWarning("Can not delete the folder. No Configuration available.");
     *         return;
     *     }
     *
     *
     *     DeleteFolder *df = new DeleteFolder(this);
     *     df->setConfiguration(config);
     *     df->setStorage(storage);
     *     df->setFolderId(id());
     *     connect(df, &DeleteFolder::succeeded, this, &QObject::deleteLater);
     *     setComponent(df);
     *     component()->execute();
     *     Q_EMIT inOperationChanged(inOperation());
     * }
     * \endcode
     *
     * \sa component()
     */
    void setComponent(Component *nComp);

    /*!
     * \brief Returns the pointer to the currently active Component.
     *
     * If no component is active, a \c nullptr will be returned.
     *
     * \sa setComponent()
     */
    Component *component() const;

private:
    Q_DISABLE_COPY(BaseItem)
    Q_DECLARE_PRIVATE(BaseItem)

};

}


#endif // FUOTENBASEITEM_H
