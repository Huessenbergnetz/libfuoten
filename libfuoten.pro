# SPDX-FileCopyrightText: (C) 2016-2022 Matthias Fehring <https://www.huessenbergnetz.de>
# SPDX-License-Identifier: LGPL-3.0-or-later

TARGET = fuoten
TEMPLATE = lib

VER_MAJ = 0
VER_MIN = 8
VER_PAT = 2
VERSION = $${VER_MAJ}.$${VER_MIN}.$${VER_PAT}

QT += network sql
QT -= gui

CONFIG -= console
CONFIG -= app_bundle
CONFIG += shared
CONFIG += c++11
CONFIG += no_keywords

DEFINES += FUOTEN_LIBRARY

DEFINES += VERSION_STRING=\"\\\"$${VERSION}\\\"\"

CONFIG(release, debug|release) {
    DEFINES += QT_NO_DEBUG_OUTPUT
}

contains(CONFIG, asan) {
    QMAKE_CXXFLAGS += "-fsanitize=address -fno-omit-frame-pointer -Wformat -Werror=format-security -Werror=array-bounds -g -ggdb"
    QMAKE_LFLAGS += "-fsanitize=address"
}

isEmpty(INSTALL_PREFIX_DIR): INSTALL_PREFIX_DIR = $$[QT_INSTALL_PREFIX]
isEmpty(INSTALL_LIB_DIR): INSTALL_LIB_DIR = $$[QT_INSTALL_LIBS]
isEmpty(INSTALL_TRANSLATIONS_DIR): INSTALL_TRANSLATIONS_DIR = $$[QT_INSTALL_TRANSLATIONS]

!contains(CONFIG, no_install_dev_files) {
    isEmpty(INSTALL_HEADERS_DIR): INSTALL_HEADERS_DIR = $$[QT_INSTALL_HEADERS]
    INSTALL_HEADERS += \
        Fuoten/Helpers/synchronizer.h \
        Fuoten/Helpers/AbstractConfiguration \
        Fuoten/Helpers/Synchronizer \
        Fuoten/Helpers/VersionNumber \
        Fuoten/Helpers/abstractconfiguration.h \
        Fuoten/Helpers/versionnumber.h \
        Fuoten/folder.h \
        Fuoten/Storage/SQLiteStorage \
        Fuoten/Storage/abstractstorage.h \
        Fuoten/Storage/sqlitestorage.h \
        Fuoten/Storage/AbstractStorage \
        Fuoten/error.h \
        Fuoten/fuoten_global.h \
        Fuoten/fuoten.h \
        Fuoten/Models/feedlistmodel.h \
        Fuoten/Models/abstractfoldermodel.h \
        Fuoten/Models/AbstractFeedModel \
        Fuoten/Models/BaseFilterModel \
        Fuoten/Models/FeedListModel \
        Fuoten/Models/folderlistfiltermodel.h \
        Fuoten/Models/FolderListFilterModel \
        Fuoten/Models/BaseModel \
        Fuoten/Models/AbstractFolderModel \
        Fuoten/Models/FeedListFilterModel \
        Fuoten/Models/abstractfeedmodel.h \
        Fuoten/Models/feedlistfiltermodel.h \
        Fuoten/Models/basemodel.h \
        Fuoten/Models/basefiltermodel.h \
        Fuoten/Models/FolderListModel \
        Fuoten/Models/folderlistmodel.h \
        Fuoten/BaseItem \
        Fuoten/Folder \
        Fuoten/feed.h \
        Fuoten/API/GetVersion \
        Fuoten/API/markfeedread.h \
        Fuoten/API/CreateFolder \
        Fuoten/API/markfolderread.h \
        Fuoten/API/createfeed.h \
        Fuoten/API/RenameFeed \
        Fuoten/API/renamefolder.h \
        Fuoten/API/MarkFolderRead \
        Fuoten/API/getuser.h \
        Fuoten/API/renamefeed.h \
        Fuoten/API/deletefolder.h \
        Fuoten/API/MarkFeedRead \
        Fuoten/API/RenameFolder \
        Fuoten/API/MoveFeed \
        Fuoten/API/getversion.h \
        Fuoten/API/CreateFeed \
        Fuoten/API/getstatus.h \
        Fuoten/API/GetUser \
        Fuoten/API/component.h \
        Fuoten/API/movefeed.h \
        Fuoten/API/GetStatus \
        Fuoten/API/getfeeds.h \
        Fuoten/API/deletefeed.h \
        Fuoten/API/GetFolders \
        Fuoten/API/DeleteFeed \
        Fuoten/API/DeleteFolder \
        Fuoten/API/Component \
        Fuoten/API/createfolder.h \
        Fuoten/API/getfolders.h \
        Fuoten/API/GetFeeds \
        Fuoten/Error \
        Fuoten/Feed \
        Fuoten/FuotenEnums \
        Fuoten/baseitem.h \
        Fuoten/Article \
        Fuoten/article.h \
        Fuoten/Models/abstractarticlemodel.h \
        Fuoten/Models/AbstractArticleModel \
        Fuoten/API/GetItems \
        Fuoten/API/getitems.h \
        Fuoten/API/GetUpdatedItems \
        Fuoten/API/getupdateditems.h \
        Fuoten/Models/ArticleListModel \
        Fuoten/Models/articlelistmodel.h \
        Fuoten/Models/ArticleListFilterModel \
        Fuoten/Models/articlelistfiltermodel.h \
        Fuoten/API/markitem.h \
        Fuoten/API/MarkItem \
        Fuoten/API/MarkMultipleItems \
        Fuoten/API/markmultipleitems.h \
        Fuoten/API/StarItem \
        Fuoten/API/staritem.h \
        Fuoten/API/StarMultipleItems \
        Fuoten/API/starmultipleitems.h \
        Fuoten/API/MarkAllItemsRead \
        Fuoten/API/markallitemsread.h \
        Fuoten/API/GetServerStatus \
        Fuoten/API/getserverstatus.h \
        Fuoten/API/LoginFlowV2 \
        Fuoten/API/loginflowv2.h \
        Fuoten/API/ConvertToAppPassword \
        Fuoten/API/converttoapppassword.h \
        Fuoten/API/DeleteAppPassword \
        Fuoten/API/deleteapppassword.h \
        Fuoten/API/GetWipeStatus \
        Fuoten/API/getwipestatus.h \
        Fuoten/API/PostWipeSuccess \
        Fuoten/API/postwipesuccess.h \
        Fuoten/Helpers/abstractnamfactory.h \
        Fuoten/Helpers/AbstractNamFactory \
        Fuoten/Helpers/abstractnotificator.h \
        Fuoten/Helpers/AbstractNotificator \
        Fuoten/Helpers/WipeManager \
        Fuoten/Helpers/wipemanager.h

    basePath = $${dirname(PWD)}
    for(header, INSTALL_HEADERS) {
        relPath = $${relative_path($$header, $$basePath)}
        path = $${INSTALL_HEADERS_DIR}/$${dirname(relPath)}
        eval(headers_$${path}.files += $$relPath)
        eval(headers_$${path}.path = $$path)
        eval(INSTALLS *= headers_$${path})
    }

    pkgconfigfile.input = fuoten.pc.in
    pkgconfigfile.output = fuoten.pc
    pkgconfigfile.path = $$[QT_INSTALL_LIBS]/pkgconfig
    pkgconfigfile.files = $$pkgconfigfile.output

    QMAKE_SUBSTITUTES += pkgconfigfile

    INSTALLS += pkgconfigfile

}

isEmpty(QHG_BIN_PATH): QHG_BIN_PATH = $$[QT_INSTALL_BINS]/qhelpgenerator
isEmpty(QT_TAG_FILES_PATH): QT_TAG_FILES_PATH = $$[QT_INSTALL_DOCS]
DOXYGEN_INPUT_DIR = $$PWD
DOXYGEN_OUTPUT_DIR = $$PWD
QT_TAG_FILES += \
        qtcore \
        qtnetwork \
        qtsql

for (qt_tag, QT_TAG_FILES) {
    exists( $${QT_TAG_FILES_PATH}/$${qt_tag}/$${qt_tag}.tags ) {
        DOXYGEN_QHP_TAGS += $${QT_TAG_FILES_PATH}/$${qt_tag}/$${qt_tag}.tags=qthelp://org.qt-project.$${qt_tag}.$${QT_MAJOR_VERSION}$${QT_MINOR_VERSION}$${QT_PATCH_VERSION}/$${qt_tag}/
        DOXYGEN_HTML_TAGS += $${QT_TAG_FILES_PATH}/$${qt_tag}/$${qt_tag}.tags=$${QT_TAG_FILES_PATH}/$${qt_tag}/
        DOXYGEN_WEB_TAGS += $${QT_TAG_FILES_PATH}/$${qt_tag}/$${qt_tag}.tags=http://doc.qt.io/qt-5/
    }
}

QHP_NAMESPACE_VERSION = $${VER_MAJ}$${VER_MIN}$${VER_PAT}
QHP_VIRT_FOLDER_VERSION = $${VER_MAJ}$${VER_MIN}
doxyfile.input = scripts/Doxyfile.in
doxyfile.output = $$PWD/Doxyfile
QMAKE_SUBSTITUTES += doxyfile

qhpdocs.commands = sed \'s|@TAGFILES@|$$DOXYGEN_QHP_TAGS|\' $$PWD/Doxyfile | sed \'s|@OUTPUT_DIR@|$$DOXYGEN_OUTPUT_DIR/qtdocs|\' | sed \'s|^GENERATE_QHP .*|GENERATE_QHP = YES|\' > $$PWD/Doxyfile.qhp; doxygen $$PWD/Doxyfile.qhp

htmldocs.commands = sed \'s|@TAGFILES@|$$DOXYGEN_HTML_TAGS|\' $$PWD/Doxyfile | sed \'s|@OUTPUT_DIR@|$$DOXYGEN_OUTPUT_DIR/htmldocs|\' > $$PWD/Doxyfile.html; doxygen $$PWD/Doxyfile.html

webdocs.commands = sed \'s|@TAGFILES@|$$DOXYGEN_WEB_TAGS|\' $$PWD/Doxyfile | sed \'s|@OUTPUT_DIR@|$$DOXYGEN_OUTPUT_DIR/webdocs|\' | sed \'s|^HTML_TIMESTAMP .*|HTML_TIMESTAMP = YES|\' > $$PWD/Doxyfile.web

docs.commands = @echo Documentation built
docs.depends = qhpdocs htmldocs

QMAKE_EXTRA_TARGETS += docs qhpdocs htmldocs webdocs

target = $$TARGET
target.path = $$INSTALL_LIB_DIR
INSTALLS += target

langfiles.path = $$INSTALL_TRANSLATIONS_DIR
langfiles.files = translations/*.qm
INSTALLS += langfiles

HEADERS += \
    Fuoten/API/converttoapppassword.h \
    Fuoten/API/converttoapppassword_p.h \
    Fuoten/API/deleteapppassword.h \
    Fuoten/API/deleteapppassword_p.h \
    Fuoten/API/getserverstatus.h \
    Fuoten/API/getserverstatus_p.h \
    Fuoten/API/getwipestatus.h \
    Fuoten/API/getwipestatus_p.h \
    Fuoten/API/loginflowv2.h \
    Fuoten/API/loginflowv2_p.h \
    Fuoten/API/postwipesuccess.h \
    Fuoten/API/postwipesuccess_p.h \
    Fuoten/Helpers/wipemanager.h \
    Fuoten/Helpers/wipemanager_p.h \
    Fuoten/fuoten_global.h \
    Fuoten/error.h \
    Fuoten/error_p.h \
    Fuoten/API/component.h \
    Fuoten/API/component_p.h \
    Fuoten/API/getversion.h \
    Fuoten/API/getversion_p.h \
    Fuoten/API/getstatus.h \
    Fuoten/API/getstatus_p.h \
    Fuoten/API/getuser.h \
    Fuoten/API/getuser_p.h \
    Fuoten/fuoten.h \
    Fuoten/baseitem.h \
    Fuoten/baseitem_p.h \
    Fuoten/folder.h \
    Fuoten/folder_p.h \
    Fuoten/Storage/abstractstorage.h \
    Fuoten/API/getfolders.h \
    Fuoten/API/getfolders_p.h \
    Fuoten/Helpers/synchronizer.h \
    Fuoten/Helpers/synchronizer_p.h \
    Fuoten/Storage/abstractstorage_p.h \
    Fuoten/Storage/sqlitestorage.h \
    Fuoten/Storage/sqlitestorage_p.h \
    Fuoten/Models/basemodel_p.h \
    Fuoten/Models/basemodel.h \
    Fuoten/Models/abstractfoldermodel.h \
    Fuoten/Models/abstractfoldermodel_p.h \
    Fuoten/Models/folderlistmodel.h \
    Fuoten/Models/folderlistmodel_p.h \
    Fuoten/Models/basefiltermodel.h \
    Fuoten/Models/basefiltermodel_p.h \
    Fuoten/Models/folderlistfiltermodel.h \
    Fuoten/Models/folderlistfiltermodel_p.h \
    Fuoten/API/renamefolder.h \
    Fuoten/API/renamefolder_p.h \
    Fuoten/API/createfolder_p.h \
    Fuoten/API/createfolder.h \
    Fuoten/API/deletefolder_p.h \
    Fuoten/API/deletefolder.h \
    Fuoten/API/markfolderread_p.h \
    Fuoten/API/markfolderread.h \
    Fuoten/feed_p.h \
    Fuoten/feed.h \
    Fuoten/Helpers/abstractconfiguration.h \
    Fuoten/API/getfeeds.h \
    Fuoten/API/getfeeds_p.h \
    Fuoten/Models/abstractfeedmodel.h \
    Fuoten/Models/abstractfeedmodel_p.h \
    Fuoten/Models/feedlistmodel.h \
    Fuoten/Models/feedlistmodel_p.h \
    Fuoten/Models/feedlistfiltermodel.h \
    Fuoten/Models/feedlistfiltermodel_p.h \
    Fuoten/API/renamefeed.h \
    Fuoten/API/renamefeed_p.h \
    Fuoten/API/deletefeed_p.h \
    Fuoten/API/deletefeed.h \
    Fuoten/API/createfeed_p.h \
    Fuoten/API/createfeed.h \
    Fuoten/API/movefeed_p.h \
    Fuoten/API/movefeed.h \
    Fuoten/API/markfeedread.h \
    Fuoten/API/markfeedread_p.h \
    Fuoten/article_p.h \
    Fuoten/article.h \
    Fuoten/Models/abstractarticlemodel.h \
    Fuoten/Models/abstractarticlemodel_p.h \
    Fuoten/API/getitems_p.h \
    Fuoten/API/getitems.h \
    Fuoten/API/getupdateditems_p.h \
    Fuoten/API/getupdateditems.h \
    Fuoten/Models/articlelistmodel_p.h \
    Fuoten/Models/articlelistmodel.h \
    Fuoten/Models/articlelistfiltermodel_p.h \
    Fuoten/Models/articlelistfiltermodel.h \
    Fuoten/API/markitem.h \
    Fuoten/API/markitem_p.h \
    Fuoten/API/markmultipleitems.h \
    Fuoten/API/markmultipleitems_p.h \
    Fuoten/API/staritem.h \
    Fuoten/API/staritem_p.h \
    Fuoten/API/starmultipleitems_p.h \
    Fuoten/API/starmultipleitems.h \
    Fuoten/API/markallitemsread.h \
    Fuoten/API/markallitemsread_p.h \
    Fuoten/Helpers/abstractnamfactory.h \
    Fuoten/Helpers/abstractnotificator.h \
    Fuoten/Helpers/abstractnotificator_p.h

SOURCES += \
    Fuoten/API/converttoapppassword.cpp \
    Fuoten/API/deleteapppassword.cpp \
    Fuoten/API/getserverstatus.cpp \
    Fuoten/API/getwipestatus.cpp \
    Fuoten/API/loginflowv2.cpp \
    Fuoten/API/postwipesuccess.cpp \
    Fuoten/Helpers/wipemanager.cpp \
    Fuoten/error.cpp \
    Fuoten/API/component.cpp \
    Fuoten/API/getversion.cpp \
    Fuoten/API/getstatus.cpp \
    Fuoten/API/getuser.cpp \
    Fuoten/baseitem.cpp \
    Fuoten/folder.cpp \
    Fuoten/Storage/abstractstorage.cpp \
    Fuoten/API/getfolders.cpp \
    Fuoten/Helpers/synchronizer.cpp \
    Fuoten/Storage/sqlitestorage.cpp \
    Fuoten/Models/basemodel.cpp \
    Fuoten/Models/abstractfoldermodel.cpp \
    Fuoten/Models/folderlistmodel.cpp \
    Fuoten/Models/basefiltermodel.cpp \
    Fuoten/Models/folderlistfiltermodel.cpp \
    Fuoten/API/renamefolder.cpp \
    Fuoten/API/createfolder.cpp \
    Fuoten/API/deletefolder.cpp \
    Fuoten/API/markfolderread.cpp \
    Fuoten/feed.cpp \
    Fuoten/Helpers/abstractconfiguration.cpp \
    Fuoten/API/getfeeds.cpp \
    Fuoten/Models/abstractfeedmodel.cpp \
    Fuoten/Models/feedlistmodel.cpp \
    Fuoten/Models/feedlistfiltermodel.cpp \
    Fuoten/API/renamefeed.cpp \
    Fuoten/API/deletefeed.cpp \
    Fuoten/API/createfeed.cpp \
    Fuoten/API/movefeed.cpp \
    Fuoten/API/markfeedread.cpp \
    Fuoten/article.cpp \
    Fuoten/Models/abstractarticlemodel.cpp \
    Fuoten/API/getitems.cpp \
    Fuoten/API/getupdateditems.cpp \
    Fuoten/Models/articlelistmodel.cpp \
    Fuoten/Models/articlelistfiltermodel.cpp \
    Fuoten/API/markitem.cpp \
    Fuoten/API/markmultipleitems.cpp \
    Fuoten/API/staritem.cpp \
    Fuoten/API/starmultipleitems.cpp \
    Fuoten/API/markallitemsread.cpp \
    Fuoten/Helpers/abstractnamfactory.cpp \
    Fuoten/Helpers/abstractnotificator.cpp \
    Fuoten/fuoten.cpp

DISTFILES += \
    fuoten.pc.in \
    scripts/Doxyfile.in
