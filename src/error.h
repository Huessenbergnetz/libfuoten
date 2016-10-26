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

#ifndef FUOTENERROR_H
#define FUOTENERROR_H

#include <QObject>
#include "fuoten_global.h"

class QNetworkReply;
struct QJsonParseError;
class QSqlError;

namespace Fuoten {

class ErrorPrivate;

/*!
 * \brief Provides information about occured errors.
 *
 * \headerfile "" <Fuoten/Error>
 */
class FUOTENSHARED_EXPORT Error : public QObject
{
    Q_OBJECT
    /*!
     * \brief Text describing the error in a human readable way.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>text() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>textChanged(const QString &text)</TD></TR></TABLE>
     */
    Q_PROPERTY(QString text READ text NOTIFY textChanged)
    /*!
     * \brief Type of the error.
     *
     * \par Access functions:
     * <TABLE><TR><TD>Type</TD><TD>type() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>typeChanged(Type type)</TD></TR></TABLE>
     */
    Q_PROPERTY(Fuoten::Error::Type type READ type NOTIFY typeChanged)
    /*!
     * \brief Additional error data.
     *
     * \par Access functions:
     * <TABLE><TR><TD>QString</TD><TD>data() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>dataChanged(const QString &data)</TD></TR></TABLE>
     */
    Q_PROPERTY(QString data READ data NOTIFY dataChanged)
    /*!
     * \brief The severity of the error.
     *
     * \par Access functions:
     * <TABLE><TR><TD>Severity</TD><TD>severity() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>severityChanged(Severity severity)</TD></TR></TABLE>
     */
    Q_PROPERTY(Fuoten::Error::Severity severity READ severity NOTIFY severityChanged)
public:
    /*!
     * \brief Defines the error type.
     */
    enum Type {
        NoError             = 0,    /**< No error has occured. */
        RequestError        = 1,    /**< The request was not setup correctly. */
        JSONParsingError    = 2,    /**< Failed to parse JSON data. */
        InputError          = 3,    /**< An error occured while providing data to the library methods. */
        OutputError         = 4,    /**< An error occured while processing the returned data from the API. */
        ServerError         = 5,    /**< An error occured on the server. */
        ApplicationError    = 6,    /**< An error occured in the local application. */
        StorageError        = 7     /**< An error occured on the storage layer. */
    };
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
    Q_ENUM(Type)
#else
    Q_ENUMS(Type)
#endif


    /*!
     * \brief Defines the severity of the error.
     */
    enum Severity {
        Nothing     = 0,    /**< No error has occured */
        Warning     = 1,    /**< Only a warning, but operation was perfmormed */
        Critical    = 2,    /**< A critical error has occured, the operation has been canceled */
        Fatal       = 3     /**< A fatal error has occured, the application will be exited */
    };
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
    Q_ENUM(Severity)
#else
    Q_ENUMS(Severity)
#endif

    /*!
     * \brief Constructs a new empty Error object.
     */
    explicit Error(QObject *parent = nullptr);


    /*!
     * \brief Constructs a new Error object from the given arguments.
     */
    Error(Type errorType, Severity errorSeverity, const QString &errorText, const QString &errorData, QObject *parent = nullptr);


    /*!
     * \brief Constructs a new Error object from a QNetworkReply.
     */
    Error(QNetworkReply *reply, QObject *parent = nullptr);

    /*!
     * \brief Constructs a new Error object form a QJsonParseError.
     */
    Error(QJsonParseError jsonError, QObject *parent = nullptr);

    /*!
     * \brief Constructs a new Error object from a QSqlError.
     */
    Error(const QSqlError &sqlError, const QString &errorText = QString(), QObject *parent = nullptr);


    /*!
     * \brief Deconstructs the Error object.
     */
    ~Error();

    /*!
     * \brief Returns the error text.
     * \sa text
     */
    QString text() const;
    /*!
     * \brief Returns the error type.
     * \sa type
     */
    Type type() const;
    /*!
     * \brief Returns additional error data.
     * \sa data
     */
    QString data() const;
    /*!
     * \brief Returns the error severity.
     * \sa severity
     */
    Severity severity() const;

    /*!
     * \brief Sets the human readable text.
     * \sa text
     */
    void setText(const QString &nText);

    /*!
     * \brief Sets the error type.
     * \sa type
     */
    void setType(Type nType);

    /*!
     * \brief Sets additional error data.
     * \sa data
     */
    void setData(const QString &nData);

    /*!
     * \brief Sets the error severity.
     * \sa severity
     */
    void setSeverity(Severity nSeverity);


Q_SIGNALS:
    /*!
     * \brief This signal is emitted if the error text changes.
     * \ſa text
     */
    void textChanged(const QString &text);
    /*!
     * \brief This signal is emitted if the error type changes.
     * \sa type
     */
    void typeChanged(Type type);
    /*!
     * \brief This signal is emitted if the additional data changes.
     * \sa data
     */
    void dataChanged(const QString &data);
    /*!
     * \brief This signal is emitted if the severity changes.
     * \sa severity
     */
    void severityChanged(Severity severity);

protected:
    const QScopedPointer<ErrorPrivate> d_ptr;

private:
    Q_DISABLE_COPY(Error)
    Q_DECLARE_PRIVATE(Error)

};

}

#if QT_VERSION < QT_VERSION_CHECK(5, 5, 0)
Q_DECLARE_METATYPE(Fuoten::Error::Type)
Q_DECLARE_METATYPE(Fuoten::Error::Severity)
#endif


#endif // FUOTENERROR_H
