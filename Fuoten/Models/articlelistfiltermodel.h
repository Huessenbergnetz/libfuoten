/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENARTICLELISTFILTERMODEL_H
#define FUOTENARTICLELISTFILTERMODEL_H

#include <QObject>
#include "basefiltermodel.h"
#include "../fuoten.h"
#include "../fuoten_global.h"
#include "fuoten_export.h"

namespace Fuoten {

class ArticleListFilterModelPrivate;

/*!
 * \brief Proxy filter model for ArticleListModel.
 *
 * This proxy model filters and sorts the data of a ArticleListModel that is internally created.
 *
 * \headerfile "" <Fuoten/Models/ArticleListFilterModel>
 */
class FUOTEN_EXPORT ArticleListFilterModel : public BaseFilterModel
{
    Q_OBJECT
    /*!
     * \brief Defines the type of the parentId used in the underlying model.
     *
     * Defaults to FuotenEnums::All
     *
     * \par Access functions:
     * <TABLE><TR><TD>FuotenEnums::Type</TD><TD>parentIdType() const</TD></TR><TR><TD>void</TD><TD>setParentIdType(FuotenEnums::Type nParentIdType)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>parentIdTypeChanged(FuotenEnums::Type parentIdType)</TD></TR></TABLE>
     */
    Q_PROPERTY(Fuoten::FuotenEnums::Type parentIdType READ parentIdType WRITE setParentIdType NOTIFY parentIdTypeChanged)
    /*!
     * \brief Limits the size of the body text in number of characters.
     *
     * Values lower than \c 0 will return no body text, \c 0 will return the full body text, any other positive
     * value will return a body stripped from HTML tags and limited to the amount of characters. Defaults to \c -1.
     *
     * \par Access functions:
     * <TABLE><TR><TD>int</TD><TD>bodyLimit() const</TD></TR><TR><TD>void</TD><TD>setBodyLimit(int nBodyLimit)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>bodyLimitChanged(int bodyLimit)</TD></TR></TABLE>
     */
    Q_PROPERTY(int bodyLimit READ bodyLimit WRITE setBodyLimit NOTIFY bodyLimitChanged)
public:
    /*!
     * \brief Constructs a new ArticleListFilterModel object with the given \a parent.
     */
    explicit ArticleListFilterModel(QObject *parent = nullptr);

    /*!
     * \brief Destroys the %ArticleListFilterModel object.
     */
    ~ArticleListFilterModel() override;

    /*!
     * \brief Returns true while the underlying ArticleListModel is loading.
     */
    bool inOperation() const override;

    /*!
     * \brief Returns the pointer to the local storage handler set in the underlying ArticleListModel.
     */
    AbstractStorage *storage() const override;

    /*!
     * \brief Returns the parent ID that is currently set in the underlying ArticleListModel.
     */
    qint64 parentId() const override;

    /*!
     * \brief Getter function for the \link AbstractArticleModel::bodyLimit bodyLimit \endlink property.
     * \sa AbstractArticleModel::setBodyLimit(), AbstractArticleModel::bodyLimitChanged()
     */
    int bodyLimit() const;

    /*!
     * \brief Sets the pointer to a local storage object in the underlying ArticleListModel.
     * \param nStorage reimplemented local storage
     */
    void setStorage(AbstractStorage *nStorage) override;

    /*!
     * \brief Sets the parent ID in the underlying ArticleListModel.
     * \param nParentId ID of the parent folder, \c -1 (default) to show all feeds
     */
    void setParentId(qint64 nParentId) override;

    /*!
     * \brief Setter function for the \link AbstractArticleModel::bodyLimit bodyLimit \endlink property.
     * Emits the bodyLimitChanged() signal if \a nBodyLimit is not equal to the stored value.
     * \sa AbstractArticleModel::bodyLimit(), AbstractArticleModel::bodyLimitChanged()
     */
    void setBodyLimit(int nBodyLimit);

    /*!
     * \brief Loads the data in the underlying ArticleListModel.
     */
    Q_INVOKABLE void load(const QString &locale = QString()) override;

    /*!
     * \brief Reloads the complete underlying ArticleListModel.
     */
    Q_INVOKABLE void reload(const QString &locale = QString()) override;

    /*!
     * \brief Getter function for the \link ArticleListFilterModel::parentIdType parentIdType \endlink property.
     * \sa ArticleListFilterModel::setParentIdType(), ArticleListFilterModel::parentIdTypeChanged()
     */
    FuotenEnums::Type parentIdType() const;

    /*!
     * \brief Setter function for the \link ArticleListFilterModel::parentIdType parentIdType \endlink property.
     * Emits the parentIdTypeChanged() signal if \a nParentIdType is not equal to the stored value.
     * \sa ArticleListFilterModel::parentIdType(), ArticleListFilterModel::parentIdTypeChanged()
     */
    void setParentIdType(FuotenEnums::Type nParentIdType);

    bool loaded() const override;

Q_SIGNALS:
    /*!
     * \brief This is emitted if the value of the \link ArticleListFilterModel::parentIdType parentIdType \endlink property changes.
     * \sa ArticleListFilterModel::parentIdType(), ArticleListFilterModel::setParentIdType()
     */
    void parentIdTypeChanged(FuotenEnums::Type parentIdType);
    /*!
     * \brief This is emitted if the value of the \link AbstractArticleModel::bodyLimit bodyLimit \endlink property changes.
     * \sa AbstractArticleModel::bodyLimit(), AbstractArticleModel::setBodyLimit()
     */
    void bodyLimitChanged(int bodyLimit);

protected:
    ArticleListFilterModel(ArticleListFilterModelPrivate &&dd, QObject *parent = nullptr);

    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

private:
    Q_DISABLE_COPY(ArticleListFilterModel)
    Q_DECLARE_PRIVATE(ArticleListFilterModel)
};

}


#endif // FUOTENARTICLELISTFILTERMODEL_H
