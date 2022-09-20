/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
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
 * \brief Provides information about occurred errors.
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
    enum Type : quint8 {
        NoError             = 0,    /**< No error has occurred. */
        RequestError        = 1,    /**< The request was not setup correctly. */
        JSONParsingError    = 2,    /**< Failed to parse JSON data. */
        InputError          = 3,    /**< An error occurred while providing data to the library methods. */
        OutputError         = 4,    /**< An error occurred while processing the returned data from the API. */
        ServerError         = 5,    /**< An error occurred on the server. */
        ApplicationError    = 6,    /**< An error occurred in the local application. */
        StorageError        = 7,    /**< An error occurred on the storage layer. */
        AuthorizationError  = 8     /**< Authentication credentials missing or not valid. */
    };
    Q_ENUM(Type)


    /*!
     * \brief Defines the severity of the error.
     */
    enum Severity : quint8 {
        Nothing     = 0,    /**< No error has occurred */
        Warning     = 1,    /**< Only a warning, but operation was perfmormed */
        Critical    = 2,    /**< A critical error has occurred, the operation has been canceled */
        Fatal       = 3     /**< A fatal error has occurred, the application will be exited */
    };
    Q_ENUM(Severity)

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
    explicit Error(QNetworkReply *reply, QObject *parent = nullptr);

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
    ~Error() override;

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
    void typeChanged(Fuoten::Error::Type type);
    /*!
     * \brief This signal is emitted if the additional data changes.
     * \sa data
     */
    void dataChanged(const QString &data);
    /*!
     * \brief This signal is emitted if the severity changes.
     * \sa severity
     */
    void severityChanged(Fuoten::Error::Severity severity);

protected:
    const QScopedPointer<ErrorPrivate> d_ptr;

private:
    Q_DISABLE_COPY(Error)
    Q_DECLARE_PRIVATE(Error)
};

}

#endif // FUOTENERROR_H
