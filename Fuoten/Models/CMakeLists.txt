# SPDX-FileCopyrightText: (C) 2016-2021 Matthias Fehring / www.huessenbergnetz.de
# SPDX-License-Identifier: LGPL-3.0-or-later

target_sources(FuotenQt${QT_VERSION_MAJOR}
    PRIVATE
        abstractarticlemodel.h
        abstractarticlemodel_p.h
        abstractarticlemodel.cpp
        abstractfeedmodel.h
        abstractfeedmodel_p.h
        abstractfeedmodel.cpp
        abstractfoldermodel.h
        abstractfoldermodel_p.h
        abstractfoldermodel.cpp
        articlelistfiltermodel.h
        articlelistfiltermodel_p.h
        articlelistfiltermodel.cpp
        articlelistmodel.h
        articlelistmodel_p.h
        articlelistmodel.cpp
        basefiltermodel.h
        basefiltermodel_p.h
        basefiltermodel.cpp
        basemodel.h
        basemodel_p.h
        basemodel.cpp
        feedlistfiltermodel.h
        feedlistfiltermodel_p.h
        feedlistfiltermodel.cpp
        feedlistmodel.h
        feedlistmodel_p.h
        feedlistmodel.cpp
        folderlistfiltermodel.h
        folderlistfiltermodel_p.h
        folderlistfiltermodel.cpp
        folderlistmodel.h
        folderlistmodel_p.h
        folderlistmodel.cpp
)

set(Models_PUBLIC_HEADER
    AbstractArticleModel
    abstractarticlemodel.h
    AbstractFeedModel
    abstractfeedmodel.h
    AbstractFolderModel
    abstractfoldermodel.h
    ArticleListFilterModel
    articlelistfiltermodel.h
    ArticleListModel
    articlelistmodel.h
    BaseFilterModel
    basefiltermodel.h
    BaseModel
    basemodel.h
    FeedListFilterModel
    feedlistfiltermodel.h
    FeedListModel
    feedlistmodel.h
    FolderListFilterModel
    folderlistfiltermodel.h
    FolderListModel
    folderlistmodel.h
)

install(FILES ${Models_PUBLIC_HEADER} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fuoten-qt${QT_VERSION_MAJOR}/Fuoten/Models COMPONENT development)
