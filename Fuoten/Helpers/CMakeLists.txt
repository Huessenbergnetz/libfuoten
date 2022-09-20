# SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
# SPDX-License-Identifier: LGPL-3.0-or-later

target_sources(FuotenQt${QT_VERSION_MAJOR}
    PRIVATE
        abstractconfiguration.h
        abstractconfiguration.cpp
        abstractnamfactory.h
        abstractnamfactory.cpp
        abstractnotificator.h
        abstractnotificator_p.h
        abstractnotificator.cpp
        synchronizer.h
        synchronizer_p.h
        synchronizer.cpp
        wipemanager.h
        wipemanager_p.h
        wipemanager.cpp
)

set(Helpers_PUBLIC_HEADER
    AbstractConfiguration
    abstractconfiguration.h
    AbstractNamFactory
    abstractnamfactory.h
    AbstractNotificator
    abstractnotificator.h
    Synchronizer
    synchronizer.h
    WipeManager
    wipemanager.h
)

install(FILES ${Helpers_PUBLIC_HEADER} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fuoten-qt${QT_VERSION_MAJOR}/Fuoten/Helpers COMPONENT development)
