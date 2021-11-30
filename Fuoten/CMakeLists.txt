# SPDX-FileCopyrightText: (C) 2016-2021 Matthias Fehring / www.huessenbergnetz.de
# SPDX-License-Identifier: LGPL-3.0-or-later

set(fuoten_SRCS
    API/converttoapppassword.cpp
    API/deleteapppassword.cpp
    API/getserverstatus.cpp
    API/getwipestatus.cpp
    API/loginflowv2.cpp
    API/postwipesuccess.cpp
    Helpers/wipemanager.cpp
    error.cpp
    API/component.cpp
    API/getversion.cpp
    API/getstatus.cpp
    API/getuser.cpp
    baseitem.cpp
    folder.cpp
    Storage/abstractstorage.cpp
    API/getfolders.cpp
    Helpers/synchronizer.cpp
    Storage/sqlitestorage.cpp
    Models/basemodel.cpp
    Models/abstractfoldermodel.cpp
    Models/folderlistmodel.cpp
    Models/basefiltermodel.cpp
    Models/folderlistfiltermodel.cpp
    API/renamefolder.cpp
    API/createfolder.cpp
    API/deletefolder.cpp
    API/markfolderread.cpp
    feed.cpp
    Helpers/abstractconfiguration.cpp
    API/getfeeds.cpp
    Models/abstractfeedmodel.cpp
    Models/feedlistmodel.cpp
    Models/feedlistfiltermodel.cpp
    API/renamefeed.cpp
    API/deletefeed.cpp
    API/createfeed.cpp
    API/movefeed.cpp
    API/markfeedread.cpp
    article.cpp
    Models/abstractarticlemodel.cpp
    API/getitems.cpp
    API/getupdateditems.cpp
    Models/articlelistmodel.cpp
    Models/articlelistfiltermodel.cpp
    API/markitem.cpp
    API/markmultipleitems.cpp
    API/staritem.cpp
    API/starmultipleitems.cpp
    API/markallitemsread.cpp
    Helpers/abstractnamfactory.cpp
    Helpers/abstractnotificator.cpp
    fuoten.cpp
)

set(fuoten_HEADERS
    Helpers/synchronizer.h
    Helpers/AbstractConfiguration
    Helpers/Synchronizer
    Helpers/abstractconfiguration.h
    folder.h
    Storage/SQLiteStorage
    Storage/abstractstorage.h
    Storage/sqlitestorage.h
    Storage/AbstractStorage
    error.h
    fuoten_global.h
    fuoten.h
    Models/feedlistmodel.h
    Models/abstractfoldermodel.h
    Models/AbstractFeedModel
    Models/BaseFilterModel
    Models/FeedListModel
    Models/folderlistfiltermodel.h
    Models/FolderListFilterModel
    Models/BaseModel
    Models/AbstractFolderModel
    Models/FeedListFilterModel
    Models/abstractfeedmodel.h
    Models/feedlistfiltermodel.h
    Models/basemodel.h
    Models/basefiltermodel.h
    Models/FolderListModel
    Models/folderlistmodel.h
    BaseItem
    Folder
    feed.h
    API/GetVersion
    API/markfeedread.h
    API/CreateFolder
    API/markfolderread.h
    API/createfeed.h
    API/RenameFeed
    API/renamefolder.h
    API/MarkFolderRead
    API/getuser.h
    API/renamefeed.h
    API/deletefolder.h
    API/MarkFeedRead
    API/RenameFolder
    API/MoveFeed
    API/getversion.h
    API/CreateFeed
    API/getstatus.h
    API/GetUser
    API/component.h
    API/movefeed.h
    API/GetStatus
    API/getfeeds.h
    API/deletefeed.h
    API/GetFolders
    API/DeleteFeed
    API/DeleteFolder
    API/Component
    API/createfolder.h
    API/getfolders.h
    API/GetFeeds
    Error
    Feed
    FuotenEnums
    baseitem.h
    Article
    article.h
    Models/abstractarticlemodel.h
    Models/AbstractArticleModel
    API/GetItems
    API/getitems.h
    API/GetUpdatedItems
    API/getupdateditems.h
    Models/ArticleListModel
    Models/articlelistmodel.h
    Models/ArticleListFilterModel
    Models/articlelistfiltermodel.h
    API/markitem.h
    API/MarkItem
    API/MarkMultipleItems
    API/markmultipleitems.h
    API/StarItem
    API/staritem.h
    API/StarMultipleItems
    API/starmultipleitems.h
    API/MarkAllItemsRead
    API/markallitemsread.h
    API/GetServerStatus
    API/getserverstatus.h
    API/LoginFlowV2
    API/loginflowv2.h
    API/ConvertToAppPassword
    API/converttoapppassword.h
    API/DeleteAppPassword
    API/deleteapppassword.h
    API/GetWipeStatus
    API/getwipestatus.h
    API/PostWipeSuccess
    API/postwipesuccess.h
    Helpers/abstractnamfactory.h
    Helpers/AbstractNamFactory
    Helpers/abstractnotificator.h
    Helpers/AbstractNotificator
    Helpers/WipeManager
    Helpers/wipemanager.h
)

set(fuoten_PRIVATE_HEADERS
    API/converttoapppassword_p.h
    API/deleteapppassword_p.h
    API/getserverstatus_p.h
    API/getwipestatus_p.h
    API/loginflowv2_p.h
    API/postwipesuccess_p.h
    Helpers/wipemanager_p.h
    error_p.h
    API/component_p.h
    API/getversion_p.h
    API/getstatus_p.h
    API/getuser_p.h
    baseitem_p.h
    folder_p.h
    API/getfolders_p.h
    Helpers/synchronizer_p.h
    Storage/abstractstorage_p.h
    Storage/sqlitestorage_p.h
    Models/basemodel_p.h
    Models/abstractfoldermodel_p.h
    Models/folderlistmodel_p.h
    Models/basefiltermodel_p.h
    Models/folderlistfiltermodel_p.h
    API/renamefolder_p.h
    API/createfolder_p.h
    API/deletefolder_p.h
    API/markfolderread_p.h
    feed_p.h
    API/getfeeds_p.h
    Models/abstractfeedmodel_p.h
    Models/feedlistmodel.h
    Models/feedlistmodel_p.h
    Models/feedlistfiltermodel.h
    Models/feedlistfiltermodel_p.h
    API/renamefeed_p.h
    API/deletefeed_p.h
    API/createfeed_p.h
    API/movefeed_p.h
    API/markfeedread_p.h
    article_p.h
    Models/abstractarticlemodel_p.h
    API/getitems_p.h
    API/getupdateditems_p.h
    Models/articlelistmodel_p.h
    Models/articlelistfiltermodel_p.h
    API/markitem_p.h
    API/markmultipleitems_p.h
    API/staritem_p.h
    API/starmultipleitems_p.h
    API/markallitemsread_p.h
    Helpers/abstractnotificator_p.h
)

add_library(FuotenQt${QT_VERSION_MAJOR}
    ${fuoten_SRCS}
    ${fuoten_HEADERS}
    ${fuoten_PRIVATE_HEADERS}
)

add_library(FuotenQt${QT_VERSION_MAJOR}::Core ALIAS FuotenQt${QT_VERSION_MAJOR})

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

target_include_directories(FuotenQt${QT_VERSION_MAJOR} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/fuoten-qt${QT_VERSION_MAJOR}>
)

set_target_properties(FuotenQt${QT_VERSION_MAJOR} PROPERTIES
    EXPORT_NAME Core
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADER "${fuoten_HEADERS}"
)

install(TARGETS FuotenQt${QT_VERSION_MAJOR}
    EXPORT FuotenTargets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT runtime
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT runtime
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fuoten-qt${QT_VERSION_MAJOR}/Fuoten COMPONENT development
)
