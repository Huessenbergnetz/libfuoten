/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTEN_GLOBAL_H
#define FUOTEN_GLOBAL_H

#include <QtGlobal>
#include <QMetaType>
#include <QList>

#if defined(FUOTEN_LIBRARY)
#  define FUOTENSHARED_EXPORT Q_DECL_EXPORT
#else
#  define FUOTENSHARED_EXPORT Q_DECL_IMPORT
#endif

#if QT_VERSION < QT_VERSION_CHECK(5, 13, 0)
#define Q_DISABLE_MOVE(Class) \
    Class(const Class &&) Q_DECL_EQ_DELETE;\
    Class &operator=(const Class &&) Q_DECL_EQ_DELETE;
#endif

namespace Fuoten {
class Article;
using ArticleList = QList<Fuoten::Article *>;
using IdList = QList<qint64>;
}

#endif // FUOTEN_GLOBAL_H
