/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016-2020 Hüssenbergnetz/Matthias Fehring
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

#include "error_p.h"
#include <QNetworkReply>
#include <QJsonParseError>
#include <QSqlError>

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

    d->printOut();
}



Error::Error(QNetworkReply *reply, QObject *parent) :
    QObject(parent), d_ptr(new ErrorPrivate)
{
    if (reply) {

        Q_D(Error);

        d->type = RequestError;
        d->severity = Critical;

        const quint16 httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<quint16>();

        if ((httpStatus >= 400) && (httpStatus < 600)) {

            if (httpStatus >= 500) {
                d->type = ServerError;
            }

            switch (httpStatus) {
            case 400:
                //% "Bad request — The server cannot or will not process the request due to an apparent client error."
                d->text = qtTrId("libfuoten-err-http-400");
                break;
            case 401:
                //% "Unauthorized — You need to provide authentication credentials to perform the request."
                d->text = qtTrId("libfuoten-err-http-401");
                d->type = AuthorizationError;
                break;
            case 403:
                //% "Fobidden — You might not have the necessary permissions to perform this action."
                d->text = qtTrId("libfuoten-err-http-403");
                d->type = AuthorizationError;
                break;
            case 404:
                //% "Not found — The requested resource can not be found on the server."
                d->text = qtTrId("libfuoten-err-http-404");
                break;
            case 405:
                //% "Method not allowed — The used method is not allowed for the requested resource."
                d->text = qtTrId("libfuoten-err-http-405");
                break;
            case 406:
                //% "Not Acceptable — The requested resource is capable of generating only content not acceptable according to the Accept headers sent in the request."
                d->text = qtTrId("libfuoten-err-http-406");
                break;
            case 407:
                //% "Proxy authentication required — The client must first authenticate itself with the proxy server."
                d->text = qtTrId("libfuoten-err-http-407");
                break;
            case 408:
                //% "Request timeout — The server timed out waiting for the request."
                d->text = qtTrId("libfuoten-err-http-408");
                break;
            case 409:
                //% "Conflict — The request could not be processed because of conflict in the request, such as an edit conflict between multiple simultaneous updates."
                d->text = qtTrId("libfuoten-err-http-409");
                break;
            case 410:
                //% "Gone — The resource requested is no longer available."
                d->text = qtTrId("libfuoten-err-http-410");
                break;
            case 411:
                //% "Length required — The request did not specify the length of its content, which is required by the requested resource."
                d->text = qtTrId("libfuoten-err-http-411");
                break;
            case 412:
                //% "Precondition failed — The server does not meet one of the preconditions that the requester put on the request."
                d->text = qtTrId("libfuoten-err-http-412");
                break;
            case 413:
                //% "Payload too large — The request is larger than the server is willing or able to process."
                d->text = qtTrId("libfuoten-err-http-413");
                break;
            case 414:
                //% "URI too long — The URI provided was too long for the server to process."
                d->text = qtTrId("libfuoten-err-http-414");
                break;
            case 415:
                //% "Unsupported media type — The request entity has a media type which the server or resource does not support."
                d->text = qtTrId("libfuoten-err-http-415");
                break;
            case 416:
                //% "Range not satisfiable — The client has asked for a portion of the resource, but the server cannot supply that portion."
                d->text = qtTrId("libfuoten-err-http-416");
                break;
            case 417:
                //% "Expectation failed — The server cannot meet the requirements of the Expect request-header field."
                d->text = qtTrId("libfuoten-err-http-417");
                break;
            case 421:
                //% "Misdirected request — The request was directed at a server that is not able to produce a response."
                d->text = qtTrId("libfuoten-err-http-421");
                break;
            case 426:
                //% "Upgrade required — The client should switch to a different protocol."
                d->text = qtTrId("libfuoten-err-http-426");
                break;
            case 428:
                //% "Precondition required — The origin server requires the request to be conditional."
                d->text = qtTrId("libfuoten-err-http-428");
                break;
            case 429:
                //% "Too many requests — The user has sent too many requests in a given amount of time."
                d->text = qtTrId("libfuoten-err-http-429");
                break;
            case 431:
                //% "Request header fields too large — The server is unwilling to process the request because either an individual header field, or all the header fields collectively, are too large."
                d->text = qtTrId("libfuoten-err-http-431");
                break;
            case 500:
                //% "Internal server error — An unexpected condition was encountered on the server."
                d->text = qtTrId("libfuoten-err-http-500");
                break;
            case 501:
                //% "Not implemented — The server either does not recognize the request method, or it lacks the ability to fulfill the request."
                d->text = qtTrId("libfuoten-err-http-501");
                break;
            case 502:
                //% "Bad gateway — The gateway or proxy received an invalid response from the upstream server."
                d->text = qtTrId("libfuoten-err-http-502");
                break;
            case 503:
                //% "Service unavailable — The server is currently unavailable (because it is overloaded or down for maintenance)."
                d->text = qtTrId("libfuoten-err-http-503");
                break;
            case 504:
                //% "Gateway timeout — The gateway or proxy and did not receive a timely response from the upstream server."
                d->text = qtTrId("libfuoten-err-http-504");
                break;
            case 505:
                //% "HTTP version not supported — The server does not support the HTTP protocol version used in the request."
                d->text = qtTrId("libfuoten-err-http-505");
                break;
            case 506:
                //% "Variant also negotiates — Transparent content negotiation for the request results in a circular reference."
                d->text = qtTrId("libfuoten-err-http-506");
                break;
            case 509:
                //% "Bandwidth limit exceeded — The request could not be processes cause it would exceed the bandwidth limit of the server."
                d->text = qtTrId("libfuoten-err-http-509");
                break;
            case 510:
                //% "Not extended — Further extensions to the request are required for the server to fulfil it."
                d->text = qtTrId("libfuoten-err-http-510");
                break;
            case 511:
                //% "Network authentication required — The client needs to authenticate to gain network access."
                d->text = qtTrId("libfuoten-err-http-511");
                d->text = RequestError;
                break;
            default:
                //% "An unknown network-related error was detected."
                d->text = qtTrId("err-net-reply-99");
                break;
            }

        } else if (reply->error() != QNetworkReply::NoError) {

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
            case QNetworkReply::TooManyRedirectsError:
                //% "While following redirects, the maximum limit was reached."
                d->text = qtTrId("err-net-reply-10");
                break;
            case QNetworkReply::InsecureRedirectError:
                //% "While following redirects, the network access API detected a redirect from an encrypted protocol (https) to an unencrypted one (http)."
                d->text = qtTrId("err-net-reply-11");
                break;
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
                d->type = ServerError;
                break;
            case QNetworkReply::OperationNotImplementedError:
                //% "The server does not support the functionality required to fulfill the request."
                d->text = qtTrId("err-net-reply-402");
                d->type = ServerError;
                break;
            case QNetworkReply::ServiceUnavailableError:
                //% "The server is unable to handle the request at this time."
                d->text = qtTrId("err-net-reply-403");
                break;
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
            case QNetworkReply::UnknownServerError:
                //% "An unknown error related to the server response was detected."
                d->text = qtTrId("err-net-reply-499");
                d->type = ServerError;
                break;
            default:
                d->text = qtTrId("err-net-reply-499");
                d->type = ServerError;
                break;
            }
        }

        d->data = reply->request().url().toString();

        d->printOut();
    }
}




Error::Error(QJsonParseError jsonError, QObject *parent) :
    QObject(parent), d_ptr(new ErrorPrivate)
{
    Q_D(Error);

    if (jsonError.error != QJsonParseError::NoError) {
        d->type = JSONParsingError;
        d->severity = Critical;
        //: intro for JSON parsing errors, %1 will be replaced by an error string
        //% "JSON parsing error: %1"
        d->text = qtTrId("libfuoten-err-json-parsing").arg(jsonError.errorString());
    }

    d->printOut();
}


Error::Error(const QSqlError &sqlError, const QString &errorText, QObject *parent) :
    QObject(parent), d_ptr(new ErrorPrivate)
{
    Q_D(Error);

    if (sqlError.type() != QSqlError::NoError) {
        d->type = StorageError;
        d->severity = Critical;
        if (errorText.isEmpty()) {
            //% "An error occurred while performing a SQL operation."
            d->text = qtTrId(("err-generic-sql"));
        } else {
            d->text = errorText;
        }
        d->data = sqlError.text();
    }

    d->printOut();
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
        qDebug("Changed text to \"%s\".", qUtf8Printable(d->text));
        Q_EMIT textChanged(text());
    }
}


Error::Type Error::type() const { Q_D(const Error); return d->type; }

void Error::setType(Type nType)
{
    Q_D(Error); 
    if (nType != d->type) {
        d->type = nType;
        qDebug("Changed type to %s.", metaObject()->enumerator(metaObject()->indexOfEnumerator("Type")).valueToKey(d->type));
        Q_EMIT typeChanged(type());
    }
}


QString Error::data() const { Q_D(const Error); return d->data; }

void Error::setData(const QString &nData)
{
    Q_D(Error); 
    if (nData != d->data) {
        d->data = nData;
        qDebug("Changed data to \"%s\".", qUtf8Printable(d->data));
        Q_EMIT dataChanged(data());
    }
}




Error::Severity Error::severity() const { Q_D(const Error); return d->severity; }

void Error::setSeverity(Severity nSeverity)
{
    Q_D(Error); 
    if (nSeverity != d->severity) {
        d->severity = nSeverity;
        qDebug("Changed severity to %s.", metaObject()->enumerator(metaObject()->indexOfEnumerator("Severity")).valueToKey(d->severity));
        Q_EMIT severityChanged(severity());
    }
}

#include "moc_error.cpp"
