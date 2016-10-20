/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016 Buschtrommel / Matthias Fehring
 * https://www.buschmann23.de/entwicklung/bibliotheken/libfuoten/
 * https://github.com/Buschtrommel/libfuoten
 *
 * folderitem.h
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

#ifndef FOLDERITEM_H
#define FOLDERITEM_H

#include <QObject>
#include "baseitem.h"
#include "fuoten_global.h"



namespace Fuoten {

class FolderItemPrivate;

/*!
 * \brief Contains information about a single folder.
 *
 * \headerfile "" <Fuoten/folderitem.h>
 */
class FUOTENSHARED_EXPORT FolderItem : public BaseItem
{
    Q_OBJECT
    /*!
     * \brief The name of the folder.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>name() const</TD></TR><TR><TD>void</TD><TD>setName(const QString &nName)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>nameChanged(const QString &name)</TD></TR></TABLE>
     */
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
public:
    /*!
     * \brief Constructs a new empty FolderItem object.
     */
    FolderItem(QObject *parent = nullptr);

    /*!
     * \brief Constructs a new FolderItem object from a QJsonDocument.
     */
    FolderItem(const QJsonDocument &json, QObject *parent = nullptr);

    /*!
     * \brief Constructs a new FolderItem object from a QJsonObject.
     */
    FolderItem(const QJsonObject &json, QObject *parent = nullptr);

    QString name() const;

    void setName(const QString &nName);

    Q_INVOKABLE void changeName(const QString &nName);

    void loadFromJson(const QJsonDocument &json) Q_DECL_OVERRIDE;

    void loadFromJson(const QJsonObject &json) Q_DECL_OVERRIDE;


Q_SIGNALS:
    void nameChanged(const QString &name);

protected:
    FolderItem(FolderItemPrivate &dd, QObject *parent = nullptr);

private:
    Q_DISABLE_COPY(FolderItem)
    Q_DECLARE_PRIVATE(FolderItem)

};

}


#endif // FOLDERITEM_H
