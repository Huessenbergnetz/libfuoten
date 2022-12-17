/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENFEEDLISTFILTERMODEL_H
#define FUOTENFEEDLISTFILTERMODEL_H

#include <QObject>
#include "basefiltermodel.h"
#include "../fuoten_global.h"

namespace Fuoten {

class FeedListFilterModelPrivate;

/*!
 * \brief Proxy filter model for FeedListModel.
 *
 * This proxy model filters and sorts the data of a FeedListModel that is internally created.
 *
 * \headerfile "" <Fuoten/Models/FeedListFilterModel>
 */
class FUOTENSHARED_EXPORT FeedListFilterModel : public BaseFilterModel
{
    Q_OBJECT
    /*!
     * \brief Set this to true, to respect the pinned status in sorting.
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>respectPinned() const</TD></TR><TR><TD>void</TD><TD>setRespectPinned(bool nRespectPinned)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>respectPinnedChanged(bool respectPinned)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool respectPinned READ respectPinned WRITE setRespectPinned NOTIFY respectPinnedChanged)
    /*!
     * \brief Set to true to sort the feeds by the name of their folder.
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>sortByFolder() const</TD></TR><TR><TD>void</TD><TD>setSortByFolder(bool nSortByFolder)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>sortByFolderChanged(bool sortByFolder)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool sortByFolder READ sortByFolder WRITE setSortByFolder NOTIFY sortByFolderChanged)
public:
    /*!
     * \brief Constructs a new feed list filter model with the given \a parent.
     */
    explicit FeedListFilterModel(QObject *parent = nullptr);

    /*!
     * \brief Deconstructs the %FeedListFilterModel object.
     */
    ~FeedListFilterModel() override;

    /*!
     * \brief Returns true while the underlying FeedListModel is loading.
     */
    bool inOperation() const override;

    /*!
     * \brief Returns the pointer to the local storage handler set in the underlying FeedListModel.
     */
    AbstractStorage *storage() const override;

    /*!
     * \brief Returns the parent ID that is currently set in the underlying FeedListModel.
     */
    qint64 parentId() const override;

    /*!
     * \brief Sets the pointer to a local storage object in the underlying FeedListModel.
     * \param nStorage reimplemented local storage
     */
    void setStorage(AbstractStorage *nStorage) override;

    /*!
     * \brief Sets the parent ID in the underlying FeedListModel.
     * \param nParentId ID of the parent folder, \c -1 (default) to show all feeds
     */
    void setParentId(qint64 nParentId) override;

    /*!
     * \brief Loads the data in the underlying FeedListModel.
     */
    Q_INVOKABLE void load(const QString &locale = QString()) override;

    /*!
     * \brief Reloads the complete underlying FeedListModel.
     */
    Q_INVOKABLE void reload(const QString &locale = QString()) override;

    /*!
     * \brief Returns true if the pinned status should be respected on sorting.
     * \sa respectPinned
     */
    bool respectPinned() const;

    /*!
     * \brief Set this to true if the pinned status should be respected on sorting.
     * \sa respectPinned
     */
    void setRespectPinned(bool nRespectPinned);

    /*!
     * \brief Returns true if the feeds should be sorted by their folder.
     * \sa sortByFolder
     */
    bool sortByFolder() const;

    /*!
     * \brief Set this to true to sort the feeds by their folder.
     */
    void setSortByFolder(bool nSortByFolder);

    bool loaded() const override;

Q_SIGNALS:
    /*!
     * \brief This is emitted if the \link FeedListFilterModel::respectPinned \endlink property changes.
     * \sa respectPinned
     */
    void respectPinnedChanged(bool respectPinned);

    /*!
     * \brief This is emitted if the \link FeedListFilterModel::sortByFolder sortByFolder \endlink property changes.
     * \sa sortByFolder
     */
    void sortByFolderChanged(bool sortByFolder);

protected:
    FeedListFilterModel(FeedListFilterModelPrivate &dd, QObject *parent = nullptr);

    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    Q_DISABLE_COPY(FeedListFilterModel)
    Q_DECLARE_PRIVATE(FeedListFilterModel)
};

}

#endif // FUOTENFEEDLISTFILTERMODEL_H
