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

#include "error_p.h"
#include <QNetworkReply>
#include <QJsonParseError>
#include <QSqlError>
#ifdef QT_DEBUG
#include <QtDebug>
#endif

using namespace Fuoten;


Error::Error(QObject *parent) :
    QObject(parent), d_ptr(new ErrorPrivate)
{
}



Error::Error(Type errorType, Severity errorSeverity, const QString &errorText, const QString &errorData, QObject *parent) :
    QObject(parent), d_ptr(new ErrorPrivate)
{
    Q_D(Error);
    d->type = errorType;
    d->severity = errorSeverity;
    d->text = errorText;
    d->data = errorData;

#ifdef QT_DEBUG
    d->printOut();
#endif
}



Error::Error(QNetworkReply *reply, QObject *parent) :
    QObject(parent), d_ptr(new ErrorPrivate)
{
    if (reply && reply->error() != QNetworkReply::NoError) {

        Q_D(Error);

        d->type = RequestError;
        d->severity = Critical;

        switch(reply->error()) {
        case QNetworkReply::ConnectionRefusedError:
            //% "The remote server at %1 refused the connection."
            d->text = qtTrId("err-net-reply-1").arg(reply->request().url().host());
            break;
        case QNetworkReply::RemoteHostClosedError:
            //% "The remote server at %1 closed the connection prematurely, before the entire reply was received and processed."
            d->text = qtTrId("err-net-reply-2").arg(reply->request().url().host());
            break;
        case QNetworkReply::HostNotFoundError:
            //% "The remote host name %1 was not found."
            d->text = qtTrId("err-net-reply-3").arg(reply->request().url().host());
            break;
        case QNetworkReply::TimeoutError:
            //% "The connection to the server at %1 timed out."
            d->text = qtTrId("err-net-reply-4").arg(reply->request().url().host());
            break;
        case QNetworkReply::OperationCanceledError:
            //% "The operation was canceled before it was finished."
            d->text = qtTrId("err-net-reply-5");
            break;
        case QNetworkReply::SslHandshakeFailedError:
            //% "The SSL/TLS handshake failed and the encrypted channel could not be established."
            d->text = qtTrId("err-net-reply-6");
            break;
        case QNetworkReply::TemporaryNetworkFailureError:
            //% "The connection was broken due to disconnection from the network."
            d->text = qtTrId("err-net-reply-7");
            break;
        case QNetworkReply::NetworkSessionFailedError:
            //% "The connection was broken due to disconnection from the network or failure to start the network."
            d->text = qtTrId("err-net-reply-8");
            break;
        case QNetworkReply::BackgroundRequestNotAllowedError:
            //% "The background request is not currently allowed due to platform policy."
            d->text = qtTrId("err-net-reply-9");
            break;
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
        case QNetworkReply::TooManyRedirectsError:
            //% "While following redirects, the maximum limit was reached."
            d->text = qtTrId("err-net-reply-10");
            break;
        case QNetworkReply::InsecureRedirectError:
            //% "While following redirects, the network access API detected a redirect from an encrypted protocol (https) to an unencrypted one (http)."
            d->text = qtTrId("err-net-reply-11");
            break;
#endif
        case QNetworkReply::ProxyConnectionRefusedError:
            //% "The connection to the proxy server was refused (the proxy server is not accepting requests)."
            d->text = qtTrId("err-net-reply-101");
            break;
        case QNetworkReply::ProxyConnectionClosedError:
            //% "The proxy server closed the connection prematurely, before the entire reply was received and processed."
            d->text = qtTrId("err-net-reply-102");
            break;
        case QNetworkReply::ProxyNotFoundError:
            //% "The proxy host name was not found (invalid proxy hostname)."
            d->text = qtTrId("err-net-reply-103");
            break;
        case QNetworkReply::ProxyTimeoutError:
            //% "The connection to the proxy timed out or the proxy did not reply in time to the request sent"
            d->text = qtTrId("err-net-reply-104");
            break;
        case QNetworkReply::ProxyAuthenticationRequiredError:
            //% "The proxy requires authentication in order to honour the request but did not accept any credentials offered (if any)."
            d->text = qtTrId("err-net-reply-105");
            break;
        case QNetworkReply::ContentAccessDenied:
            //% "The access to the remote content was denied."
            d->text = qtTrId("err-net-reply-201");
            break;
        case QNetworkReply::ContentOperationNotPermittedError:
            //% "The operation requested on the remote content is not permitted."
            d->text = qtTrId("err-net-reply-202");
            break;
        case QNetworkReply::ContentNotFoundError:
            //% "The remote content was not found at the server."
            d->text = qtTrId("err-net-reply-203");
            break;
        case QNetworkReply::AuthenticationRequiredError:
            //% "The remote server requires authentication to serve the content but the credentials provided were not accepted (if any)."
            d->text = qtTrId("err-net-reply-204");
            break;
        case QNetworkReply::ContentReSendError:
            //% "The request needed to be sent again, but this failed for example because the upload data could not be read a second time."
            d->text = qtTrId("err-net-reply-205");
            break;
#if QT_VERSION >= QT_VERSION_CHECK(5, 3, 0)
        case QNetworkReply::ContentConflictError:
            //% "The request could not be completed due to a conflict with the current state of the resource."
            d->text = qtTrId("err-net-reply-206");
            break;
        case QNetworkReply::ContentGoneError:
            //% "The requested resource is no longer available at the server."
            d->text = qtTrId("err-net-reply-207");
            break;
        case QNetworkReply::InternalServerError:
            //% "The server encountered an unexpected condition which prevented it from fulfilling the request."
            d->text = qtTrId("err-net-reply-401");
            break;
        case QNetworkReply::OperationNotImplementedError:
            //% "The server does not support the functionality required to fulfill the request."
            d->text = qtTrId("err-net-reply-402");
            break;
        case QNetworkReply::ServiceUnavailableError:
            //% "The server is unable to handle the request at this time."
            d->text = qtTrId("err-net-reply-403");
            break;
#endif
        case QNetworkReply::ProtocolUnknownError:
            //% "The Network Access API cannot honor the request because the protocol is not known."
            d->text = qtTrId("err-net-reply-301");
            break;
        case QNetworkReply::ProtocolInvalidOperationError:
            //% "The requested operation is invalid for this protocol."
            d->text = qtTrId("err-net-reply-302");
            break;
        case QNetworkReply::UnknownNetworkError:
            //% "An unknown network-related error was detected."
            d->text = qtTrId("err-net-reply-99");
            break;
        case QNetworkReply::UnknownProxyError:
            //% "An unknown proxy-related error was detected."
            d->text = qtTrId("err-net-reply-199");
            break;
        case QNetworkReply::UnknownContentError:
            //% "An unknown error related to the remote content was detected."
            d->text = qtTrId("err-net-reply-299");
            break;
        case QNetworkReply::ProtocolFailure:
            //% "A breakdown in protocol was detected (parsing error, invalid or unexpected responses, etc.)."
            d->text = qtTrId("err-net-reply-399");
            break;
#if QT_VERSION >= QT_VERSION_CHECK(5, 3, 0)
        case QNetworkReply::UnknownServerError:
            //% "An unknown error related to the server response was detected."
            d->text = qtTrId("err-net-reply-499");
            break;
#endif
        default:
            //% "An unknown error related to the server response was detected."
            d->text = qtTrId("err-net-reply-499");
            break;
        }

        d->data = reply->request().url().toString();

#ifdef QT_DEBUG
        d->printOut();
#endif

    }
}




Error::Error(QJsonParseError jsonError, QObject *parent) :
    QObject(parent), d_ptr(new ErrorPrivate)
{
    Q_D(Error);

    if (jsonError.error != QJsonParseError::NoError) {
        d->type = OutputError;
        d->severity = Critical;
        d->text = jsonError.errorString();
    }

#ifdef QT_DEBUG
        d->printOut();
#endif
}





Error::Error(const QSqlError &sqlError, const QString &errorText, QObject *parent) :
    QObject(parent), d_ptr(new ErrorPrivate)
{
    Q_D(Error);

    if (sqlError.type() != QSqlError::NoError) {
        d->type = StorageError;
        d->severity = Critical;
        if (errorText.isEmpty()) {
            //% "An error occured while performing a SQL operation."
            d->text = qtTrId(("err-generic-sql"));
        } else {
            d->text = errorText;
        }
        d->data = sqlError.text();
    }

#ifdef QT_DEBUG
        d->printOut();
#endif
}




Error::~Error()
{
}






QString Error::text() const { Q_D(const Error); return d->text; }

void Error::setText(const QString &nText)
{
    Q_D(Error); 
    if (nText != d->text) {
        d->text = nText;
#ifdef QT_DEBUG
        qDebug() << "Changed text to" << d->text;
#endif
        Q_EMIT textChanged(text());
    }
}




Error::Type Error::type() const { Q_D(const Error); return d->type; }

void Error::setType(Type nType)
{
    Q_D(Error); 
    if (nType != d->type) {
        d->type = nType;
#ifdef QT_DEBUG
        qDebug() << "Changed type to" << d->type;
#endif
        Q_EMIT typeChanged(type());
    }
}




QString Error::data() const { Q_D(const Error); return d->data; }

void Error::setData(const QString &nData)
{
    Q_D(Error); 
    if (nData != d->data) {
        d->data = nData;
#ifdef QT_DEBUG
        qDebug() << "Changed data to" << d->data;
#endif
        Q_EMIT dataChanged(data());
    }
}




Error::Severity Error::severity() const { Q_D(const Error); return d->severity; }

void Error::setSeverity(Severity nSeverity)
{
    Q_D(Error); 
    if (nSeverity != d->severity) {
        d->severity = nSeverity;
#ifdef QT_DEBUG
        qDebug() << "Changed severity to" << d->severity;
#endif
        Q_EMIT severityChanged(severity());
    }
}




