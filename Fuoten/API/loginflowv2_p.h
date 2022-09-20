/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
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
