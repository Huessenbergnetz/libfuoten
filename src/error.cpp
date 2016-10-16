/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016 Buschtrommel / Matthias Fehring
 * https://www.buschmann23.de/entwicklung/bibliotheken/libfuoten/
 * https://github.com/Buschtrommel/libfuoten
 *
 * error.cpp
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
            d->text = tr("The remote server refused the connection.");
            break;
        case QNetworkReply::RemoteHostClosedError:
            d->text = tr("The remote server closed the connection prematurely, before the entire reply was received and processed.");
            break;
        case QNetworkReply::HostNotFoundError:
            d->text = tr("The remote host name was not found.");
            break;
        case QNetworkReply::TimeoutError:
            d->text = tr("The connection to the server timed out.");
            break;
        case QNetworkReply::OperationCanceledError:
            d->text = tr("The operation was canceled before it was finished.");
            break;
        case QNetworkReply::SslHandshakeFailedError:
            d->text = tr("The SSL/TLS handshake failed and the encrypted channel could not be established.");
            break;
        case QNetworkReply::TemporaryNetworkFailureError:
            d->text = tr("The connection was broken due to disconnection from the network.");
            break;
        case QNetworkReply::NetworkSessionFailedError:
            d->text = tr("The connection was broken due to disconnection from the network or failure to start the network.");
            break;
        case QNetworkReply::BackgroundRequestNotAllowedError:
            d->text = tr("The background request is not currently allowed due to platform policy.");
            break;
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
        case QNetworkReply::TooManyRedirectsError:
            d->text = tr("While following redirects, the maximum limit was reached.");
            break;
        case QNetworkReply::InsecureRedirectError:
            d->text = tr("While following redirects, the network access API detected a redirect from an encrypted protocol (https) to an unencrypted one (http).");
            break;
#endif
        case QNetworkReply::ProxyConnectionRefusedError:
            d->text = tr("The connection to the proxy server was refused (the proxy server is not accepting requests).");
            break;
        case QNetworkReply::ProxyConnectionClosedError:
            d->text = tr("The proxy server closed the connection prematurely, before the entire reply was received and processed.");
            break;
        case QNetworkReply::ProxyNotFoundError:
            d->text = tr("The proxy host name was not found (invalid proxy hostname).");
            break;
        case QNetworkReply::ProxyTimeoutError:
            d->text = tr("The connection to the proxy timed out or the proxy did not reply in time to the request sent");
            break;
        case QNetworkReply::ProxyAuthenticationRequiredError:
            d->text = tr("The proxy requires authentication in order to honour the request but did not accept any credentials offered (if any).");
            break;
        case QNetworkReply::ContentAccessDenied:
            d->text = tr("The access to the remote content was denied.");
            break;
        case QNetworkReply::ContentOperationNotPermittedError:
            d->text = tr("The operation requested on the remote content is not permitted.");
            break;
        case QNetworkReply::ContentNotFoundError:
            d->text = tr("The remote content was not found at the server.");
            break;
        case QNetworkReply::AuthenticationRequiredError:
            d->text = tr("The remote server requires authentication to serve the content but the credentials provided were not accepted (if any).");
            break;
        case QNetworkReply::ContentReSendError:
            d->text = tr("The request needed to be sent again, but this failed for example because the upload data could not be read a second time.");
            break;
#if QT_VERSION >= QT_VERSION_CHECK(5, 3, 0)
        case QNetworkReply::ContentConflictError:
            d->text = tr("The request could not be completed due to a conflict with the current state of the resource.");
            break;
        case QNetworkReply::ContentGoneError:
            d->text = tr("The requested resource is no longer available at the server.");
            break;
        case QNetworkReply::InternalServerError:
            d->text = tr("The server encountered an unexpected condition which prevented it from fulfilling the request.");
            break;
        case QNetworkReply::OperationNotImplementedError:
            d->text = tr("The server does not support the functionality required to fulfill the request.");
            break;
        case QNetworkReply::ServiceUnavailableError:
            d->text = tr("The server is unable to handle the request at this time.");
            break;
#endif
        case QNetworkReply::ProtocolUnknownError:
            d->text = tr("The Network Access API cannot honor the request because the protocol is not known.");
            break;
        case QNetworkReply::ProtocolInvalidOperationError:
            d->text = tr("The requested operation is invalid for this protocol.");
            break;
        case QNetworkReply::UnknownNetworkError:
            d->text = tr("An unknown network-related error was detected.");
            break;
        case QNetworkReply::UnknownProxyError:
            d->text = tr("An unknown proxy-related error was detected.");
            break;
        case QNetworkReply::UnknownContentError:
            d->text = tr("An unknown error related to the remote content was detected.");
            break;
        case QNetworkReply::ProtocolFailure:
            d->text = tr("A breakdown in protocol was detected (parsing error, invalid or unexpected responses, etc.).");
            break;
#if QT_VERSION >= QT_VERSION_CHECK(5, 3, 0)
        case QNetworkReply::UnknownServerError:
            d->text = tr("An unknown error related to the server response was detected.");
            break;
#endif
        default:
            d->text = tr("An unknown error related to the server response was detected.");
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
    if (jsonError.error != QJsonParseError::NoError) {
        Q_D(Error);
        d->type = OutputError;
        d->severity = Critical;
        d->text = jsonError.errorString();
    }
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




