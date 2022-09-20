/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "wipemanager_p.h"
#include "abstractnotificator.h"
#include "../API/getwipestatus.h"
#include "../API/postwipesuccess.h"
#include "../Storage/abstractstorage.h"
#include "../error.h"

using namespace Fuoten;

WipeManagerConfig::WipeManagerConfig(QObject *parent) :
    AbstractConfiguration(parent)
{

}

WipeManagerConfig::~WipeManagerConfig()
{

}

QString WipeManagerConfig::getUsername() const
{
    return m_username;
}

void WipeManagerConfig::setUsername(const QString &username)
{
    m_username = username;
}

QString WipeManagerConfig::getPassword() const
{
    return m_password;
}

void WipeManagerConfig::setPassword(const QString &password)
{
    m_password = password;
}

QString WipeManagerConfig::getHost() const
{
    return m_host;
}

void WipeManagerConfig::setHost(const QString &host)
{
    m_host = host;
}

QString WipeManagerConfig::getInstallPath() const
{
    return m_installPath;
}

void WipeManagerConfig::setInstallPath(const QString &path)
{
    m_installPath = path;
}

int WipeManagerConfig::getServerPort() const
{
    return m_serverPort;
}

void WipeManagerConfig::setServerPort(int port)
{
    m_serverPort = port;
}

bool WipeManagerConfig::getUseSSL() const
{
    return m_useSSL;
}

void WipeManagerConfig::setUseSSL(bool useSSL)
{
    m_useSSL = useSSL;
}

bool WipeManagerConfig::getIgnoreSSLErrors() const
{
    return m_ignoreSSLErrors;
}

void WipeManagerConfig::setIgnoreSSLErrors(bool ignoreSSLErrors)
{
    m_ignoreSSLErrors = ignoreSSLErrors;
}

bool WipeManagerConfig::isAccountValid() const
{
    return true;
}

void WipeManagerConfig::setIsAccountValid(bool nIsAccountValid)
{
    Q_UNUSED(nIsAccountValid)
}

WipeManagerPrivate::WipeManagerPrivate(WipeManager *q) :
    q_ptr(q)
{

}

WipeManagerPrivate::~WipeManagerPrivate()
{

}

void WipeManagerPrivate::setInOperation(bool _inOperation)
{
    if (inOperation != _inOperation) {
        qDebug("Changed inOperation from %s to %s.", inOperation ? "true" : "false", _inOperation ? "true" : "false");
        inOperation = _inOperation;
        Q_Q(WipeManager);
        Q_EMIT q->inOperationChanged(inOperation);
    }
}

void WipeManagerPrivate::doWipe()
{
    qDebug("%s", "Start executing remote wipe request");
    setInOperation(true);

    Q_Q(WipeManager);

    Q_EMIT q->wipeStarted();

    if (!notificator) {
        notificator = Component::defaultNotificator();
    }

    if (notificator) {
        notificator->notify(AbstractNotificator::RemoteWipeRequested, QtFatalMsg, QVariant());
    }

    if (!storage) {
        storage = Component::defaultStorage();
    }

    if (!configuration) {
        configuration = Component::defaultConfiguration();
    }

    if (!configuration) {
        setInOperation(false);
        Q_EMIT q->wipeFailed();
        return;
    }

    wipeconfig = new WipeManagerConfig(q);
    wipeconfig->setUsername(configuration->getUsername());
    wipeconfig->setPassword(configuration->getPassword());
    wipeconfig->setHost(configuration->getHost());
    wipeconfig->setInstallPath(configuration->getInstallPath());
    wipeconfig->setServerPort(configuration->getServerPort());
    wipeconfig->setUseSSL(configuration->getUseSSL());
    wipeconfig->setIgnoreSSLErrors(configuration->getIgnoreSSLErrors());

    QObject::connect(configuration, &AbstractConfiguration::accountDeleted, q, [this,q](){
        auto wipeNotifier = new PostWipeSuccess(q);
        wipeNotifier->setConfiguration(wipeconfig);

        QObject::connect(wipeNotifier, &PostWipeSuccess::succeeded, q, [this,q](){
            setInOperation(false);
            Q_EMIT q->wipeDone();
            notificator->notify(AbstractNotificator::RemoteWipeDone, QtFatalMsg, QVariant());
        });

        QObject::connect(wipeNotifier, &PostWipeSuccess::failed, q, [this,q](){
            setInOperation(false);
            Q_EMIT q->wipeDone();
            notificator->notify(AbstractNotificator::RemoteWipeDone, QtFatalMsg, QVariant());
        });

        wipeNotifier->execute();
    });

    if (storage) {
        QObject::connect(storage, &AbstractStorage::storageCleared, q, [this](){
            configuration->deleteAccount();
        });
        storage->clearStorage();
    } else {
        configuration->deleteAccount();
    }
}

WipeManager::WipeManager(QObject *parent) :
    QObject(parent), d_ptr(new WipeManagerPrivate(this))
{

}

WipeManager::~WipeManager()
{

}

void WipeManager::checkForWipe()
{
    Q_D(WipeManager);
    auto wipeChecker = new GetWipeStatus(this);
    AbstractConfiguration *conf = d->configuration;
    if (!conf) {
        conf = Component::defaultConfiguration();
    }
    wipeChecker->setConfiguration(conf);
    connect(wipeChecker, &GetWipeStatus::wipeRequested, this, [d](){
        d->doWipe();
    });
    connect(wipeChecker, &GetWipeStatus::succeeded, wipeChecker, &QObject::deleteLater);
    connect(wipeChecker, &GetWipeStatus::failed, wipeChecker, &QObject::deleteLater);
    wipeChecker->execute();
}

bool WipeManager::inOperation() const
{
    Q_D(const WipeManager);
    return d->inOperation;

}

void WipeManager::setConfiguration(AbstractConfiguration *configuration)
{
    Q_D(WipeManager);
    d->configuration = configuration;
}

void WipeManager::setStorage(AbstractStorage *storage)
{
    Q_D(WipeManager);
    d->storage = storage;
}

void WipeManager::setNotificator(AbstractNotificator *notificator)
{
    Q_D(WipeManager);
    d->notificator = notificator;
}

#include "moc_wipemanager.cpp"
