# SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
# SPDX-License-Identifier: LGPL-3.0-or-later

add_library(FuotenQt${QT_VERSION_MAJOR})

target_sources(FuotenQt${QT_VERSION_MAJOR}
    PRIVATE
        fuoten_global.h
        ${CMAKE_CURRENT_BINARY_DIR}/fuoten_export.h
        article.h
        article_p.h
        article.cpp
        baseitem.h
        baseitem_p.h
        baseitem.cpp
        error.h
        error_p.h
        error.cpp
        feed.h
        feed_p.h
        feed.cpp
        folder.h
        folder_p.h
        folder.cpp
        fuoten.h
        fuoten.cpp
)

set_property(TARGET FuotenQt${QT_VERSION_MAJOR}
    APPEND PROPERTY PUBLIC_HEADER
        fuoten_global.h
        ${CMAKE_CURRENT_BINARY_DIR}/fuoten_export.h
        article.h
        Article
        baseitem.h
        BaseItem
        error.h
        Error
        feed.h
        Feed
        folder.h
        Folder
        fuoten.h
        FuotenEnums
)

add_subdirectory(API)
add_subdirectory(Helpers)
add_subdirectory(Models)
add_subdirectory(Storage)

add_library(FuotenQt${QT_VERSION_MAJOR}::Core ALIAS FuotenQt${QT_VERSION_MAJOR})

generate_export_header(FuotenQt${QT_VERSION_MAJOR} BASE_NAME fuoten)

target_compile_features(FuotenQt${QT_VERSION_MAJOR} PUBLIC cxx_std_14)

target_link_libraries(FuotenQt${QT_VERSION_MAJOR}
    PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Network
        Qt${QT_VERSION_MAJOR}::Sql
)

target_compile_definitions(FuotenQt${QT_VERSION_MAJOR}
    PRIVATE
        $<$<NOT:$<CONFIG:Debug>>:QT_NO_DEBUG_OUTPUT>
        QT_NO_KEYWORDS
        QT_NO_CAST_TO_ASCII
        QT_NO_CAST_FROM_ASCII
        QT_STRICT_ITERATORS
        QT_NO_URL_CAST_FROM_STRING
        QT_NO_CAST_FROM_BYTEARRAY
        QT_USE_QSTRINGBUILDER
        QT_NO_SIGNALS_SLOTS_KEYWORDS
        QT_USE_FAST_OPERATOR_PLUS
        QT_DISABLE_DEPRECATED_BEFORE=0x050500
        FUOTEN_VERSION="${PROJECT_VERSION}"
)

if(ENABLE_MAINTAINER_FLAGS)
    target_compile_definitions(FuotenQt${QT_VERSION_MAJOR}
        PRIVATE
            QT_DEPRECATED_WARNINGS
    )
    target_compile_options(FuotenQt${QT_VERSION_MAJOR}
        PRIVATE
            -Wall
            -Wcast-align
            -Wno-uninitialized
            -Wempty-body
            -Wformat-security
            -Wformat
            -Winit-self
    )
endif(ENABLE_MAINTAINER_FLAGS)

get_property(_current_parent_dir DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY PARENT_DIRECTORY)

target_include_directories(FuotenQt${QT_VERSION_MAJOR} PUBLIC
    $<BUILD_INTERFACE:${_current_parent_dir}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/fuoten-qt${QT_VERSION_MAJOR}>
)

set_target_properties(FuotenQt${QT_VERSION_MAJOR} PROPERTIES
    EXPORT_NAME Core
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

install(TARGETS FuotenQt${QT_VERSION_MAJOR}
    EXPORT FuotenTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT runtime
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT runtime
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fuoten-qt${QT_VERSION_MAJOR}/Fuoten COMPONENT development
)
