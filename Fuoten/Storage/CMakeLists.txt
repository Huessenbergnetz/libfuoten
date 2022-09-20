# SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
# SPDX-License-Identifier: LGPL-3.0-or-later

target_sources(FuotenQt${QT_VERSION_MAJOR}
    PRIVATE
        abstractstorage.h
        abstractstorage_p.h
        abstractstorage.cpp
        sqlitestorage.h
        sqlitestorage_p.h
        sqlitestorage.cpp
)

set(Storage_PUBLIC_HEADER
    AbstractStorage
    abstractstorage.h
    SQLiteStorage
    sqlitestorage.h
)

install(FILES ${Storage_PUBLIC_HEADER} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fuoten-qt${QT_VERSION_MAJOR}/Fuoten/Storage COMPONENT development)
