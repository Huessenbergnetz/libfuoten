/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "abstractnotificator_p.h"
#include "../error.h"

#include <QVariant>
#include <QString>

using namespace Fuoten;

AbstractNotificator::AbstractNotificator(QObject *parent) : QObject(parent), d_ptr(new AbstractNotificatorPrivate)
{

}


AbstractNotificator::~AbstractNotificator()
{

}


QString AbstractNotificator::appIcon() const { Q_D(const AbstractNotificator); return d->appIcon; }

void AbstractNotificator::setAppIcon(const QString &appIcon)
{
    Q_D(AbstractNotificator);
    if (d->appIcon != appIcon) {
        d->appIcon = appIcon;
        qDebug("Changed appIcon to \"%s\"", qUtf8Printable(d->appIcon));
        Q_EMIT appIconChanged(d->appIcon);
    }
}


QString AbstractNotificator::appName() const { Q_D(const AbstractNotificator); return d->appName; }

void AbstractNotificator::setAppName(const QString &appName)
{
    Q_D(AbstractNotificator);
    if (d->appName != appName) {
        d->appName = appName;
        qDebug("Changed appName to \"%s\".", qUtf8Printable(d->appName));
        Q_EMIT appNameChanged(d->appName);
    }
}


bool AbstractNotificator::isEnabled() const { Q_D(const AbstractNotificator); return d->enabled; }

void AbstractNotificator::setEnabled(bool enabled)
{
    Q_D(AbstractNotificator);
    if (d->enabled != enabled) {
        d->enabled = enabled;
        qDebug("Changed enabled to %s.", d->enabled ? "true" : "false");
        Q_EMIT enabledChanged(d->enabled);
    }
}


bool AbstractNotificator::isArticlePublishingEnabled() const { Q_D(const AbstractNotificator); return d->articlePublishing; }

void AbstractNotificator::setArticlePublishingEnabled(bool enabled)
{
    Q_D(AbstractNotificator);
    if (d->articlePublishing != enabled) {
        d->articlePublishing = enabled;
        qDebug("Cahnged article publishing to %s.", d->articlePublishing ? "true" : "false");
        Q_EMIT articlePublishingChanged(d->articlePublishing);
    }
}


void AbstractNotificator::notify(const Fuoten::Error *e) const
{
    if (e && (e->type() != Error::NoError) && isEnabled()) {
        AbstractNotificator::Type t = AbstractNotificator::GeneralError;
        switch (e->type()) {
        case Error::RequestError:
            t = AbstractNotificator::RequestError;
            break;
        case Error::JSONParsingError:
            t = AbstractNotificator::ParsingError;
            break;
        case Error::InputError:
            t = AbstractNotificator::InputError;
            break;
        case Error::OutputError:
            t = AbstractNotificator::OutputError;
            break;
        case Error::ServerError:
            t = AbstractNotificator::ServerError;
            break;
        case Error::ApplicationError:
            t = AbstractNotificator::ApplicationError;
            break;
        case Error::StorageError:
            t = AbstractNotificator::StorageError;
            break;
        case Error::AuthorizationError:
            t = AbstractNotificator::AuthorizationError;
            break;
        default:
            break;
        }

        QtMsgType s = QtInfoMsg;
        switch(e->severity()) {
        case Error::Warning:
            s = QtWarningMsg;
            break;
        case Error::Critical:
            s = QtCriticalMsg;
            break;
        case Error::Fatal:
            s = QtFatalMsg;
            break;
        default:
            break;
        }

        notify(t, s, e->text());
    }
}


void AbstractNotificator::publishArticle(const QJsonObject &article, const QString &feedName) const
{
    Q_UNUSED(article)
    Q_UNUSED(feedName)
}


void AbstractNotificator::publishArticle(const Article *article) const
{
    Q_UNUSED(article)
}


bool AbstractNotificator::checkForPublishing(const Article *article) const
{
    Q_UNUSED(article)
    return false;
}


bool AbstractNotificator::checkForPublishing(const QJsonObject &article) const
{
    Q_UNUSED(article)
    return false;
}

#include "moc_abstractnotificator.cpp"
