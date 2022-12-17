/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENBASEITEM_H
#define FUOTENBASEITEM_H

#include <QObject>
#include "fuoten_global.h"
#include "fuoten_export.h"

class QJsonDocument;
class QJsonObject;

namespace Fuoten {

class Error;
class AbstractConfiguration;
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
class FUOTEN_EXPORT BaseItem : public QObject
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
     * \brief Database ID of the Article, Feed or Folder.
     *
     * \par Access functions:
     * <TABLE><TR><TD>qint64</TD><TD>id() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>idChanged(qint64 id)</TD></TR></TABLE>
     *
     * \sa setId()
     */
    Q_PROPERTY(qint64 id READ id NOTIFY idChanged)
    /*!
     * \brief Pointer to an Error object, if an error occurred, otherwise returns a \c nullptr.
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
     * \brief Constructs an empty base item with the given \a parent.
     */
    explicit BaseItem(QObject *parent = nullptr);

    /*!
     * \brief Deconstructs the base item.
     */
    ~BaseItem() override;

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
    qint64 id() const;

    /*!
     * \brief Returns a pointer to an Error object, if any error occurred, otherwise a \c nullptr.
     *
     * \sa BaseItem::error
     */
    Error *error() const;

    /*!
     * \brief Sets the database ID of the item/feed/folder.
     *
     * \sa id
     */
    void setId(qint64 nId);

    /*!
     * \brief Removes the current Error object and sets a \c nullptr.
     */
    Q_INVOKABLE void clearError();

    /*!
     * \brief Makes a deep copy of \a other into this object.
     *
     * The default implementation does nothing. When reimplementing, do a qobject_cast to the derived type.
     */
    virtual void copy(BaseItem *other);

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
    void idChanged(qint64 id);

    /*!
     * \brief This signal will be emitted whenever the error property changes. May return a nullptr.
     * \sa BaseItem::error
     */
    void errorChanged(Fuoten::Error *error);

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
     * void Folder::remove(AbstractConfiguration *config, AbstractStorage *storage)
     * {
     *     if (!config) {
     *         qWarning("Can not delete the folder. No AbstractConfiguration available.");
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
