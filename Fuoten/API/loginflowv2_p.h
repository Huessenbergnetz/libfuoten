/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016-2019 Hüssenbergnetz/Matthias Fehring
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

#ifndef FUOTENLOGINFLOWV2_P_H
#define FUOTENLOGINFLOWV2_P_H

#include "loginflowv2.h"
#include "component_p.h"

namespace Fuoten {

class LoginFlowV2Private : public ComponentPrivate
{
public:
    LoginFlowV2Private(LoginFlowV2 *q);
    ~LoginFlowV2Private() override;

    void startPolling(const QUrl &endpoint, const QString &token);
    void sendPollingRequest();
    void pollingRequestFinished();
    void pollingRequestTimedOut();
    void emitSecondsLeft();
    void overallTimerTimedOut();

    QNetworkRequest pollingRequest;
    QByteArray pollingData;
    QTimer *overallTimeoutTimer = nullptr;
    QTimer *secondsLeftTimer = nullptr;
    QTimer *pollingRequestTimeoutTimer = nullptr;
    QNetworkReply *pollingReply = nullptr;
    int pollingRequestTimeout = 5;
    int pollingInterval = 2;
    int pollingTimeout = 300;
    int timeoutsInARow = 0;
    int maxTimeoutsInARow = 5;

private:
    LoginFlowV2 *q_ptr;
    Q_DECLARE_PUBLIC(LoginFlowV2)
};

}

#endif // FUOTENLOGINFLOWV2_P_H
