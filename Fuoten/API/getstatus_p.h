/*
 * SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef FUOTENGETSTATUS_P_H
#define FUOTENGETSTATUS_P_H

#include "getstatus.h"
#include "component_p.h"
#include <QJsonObject>
#include <QVersionNumber>

namespace Fuoten {

class GetStatusPrivate : public ComponentPrivate {
public:
    GetStatusPrivate(GetStatus *q);
    ~GetStatusPrivate() override;

    void setVersion(const QString &_version);
    void setVersion(const QVersionNumber &_version);
    void setImproperlyConfiguredCrond(bool _improperlyConfiguredCron);
    void setIncorrectDbCharset(bool _incorrectDbCharset);

    QJsonObject resultObject;
    QVersionNumber version;
    bool improperlyConfiguredCron = false;
    bool incorrectDbCharset = false;

private:
    GetStatus *q_ptr;
    Q_DECLARE_PUBLIC(GetStatus)
};

}

#endif // FUOTENGETSTATUS_P_H

