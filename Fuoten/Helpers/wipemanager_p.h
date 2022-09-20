/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENWIPEMANAGER_P_H
#define FUOTENWIPEMANAGER_P_H

#include "wipemanager.h"
#include "abstractconfiguration.h"

namespace Fuoten {

class WipeManagerConfig : public AbstractConfiguration
{
public:
    explicit WipeManagerConfig(QObject *parent = nullptr);
    ~WipeManagerConfig() override;

    QString getUsername() const override;
    void setUsername(const QString &username) override;

    QString getPassword() const override;
    void setPassword(const QString &password) override;

    QString getHost() const override;
    void setHost(const QString &host) override;

    QString getInstallPath() const override;
    void setInstallPath(const QString &path) override;

    int getServerPort() const override;
    void setServerPort(int port) override;

    bool getUseSSL() const override;
    void setUseSSL(bool useSSL) override;

    bool getIgnoreSSLErrors() const override;
    void setIgnoreSSLErrors(bool ignoreSSLErrors);

    bool isAccountValid() const override;

protected:
    void setIsAccountValid(bool nIsAccountValid) override;

private:
    QString m_username;
    QString m_password;
    QString m_host;
    QString m_installPath;
    int m_serverPort = 0;
    bool m_useSSL = true;
    bool m_ignoreSSLErrors = false;

    Q_DISABLE_COPY(WipeManagerConfig)
    Q_DISABLE_MOVE(WipeManagerConfig)
};

class WipeManagerPrivate {
public:
    explicit WipeManagerPrivate(WipeManager *q);
    ~WipeManagerPrivate();

    void setInOperation(bool _inOperation);

    void doWipe();

private:
    Q_DECLARE_PUBLIC(WipeManager)
    WipeManager *q_ptr;
    AbstractNotificator *notificator = nullptr;
    AbstractConfiguration *configuration = nullptr;
    AbstractStorage *storage = nullptr;
    WipeManagerConfig *wipeconfig = nullptr;
    bool inOperation = false;
};

}

#endif // FUOTENWIPEMANAGER_P_H
