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
