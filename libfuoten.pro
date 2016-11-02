TARGET = fuoten
TEMPLATE = lib

VER_MAJ = 0
VER_MIN = 2
VER_PAT = 0
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

isEmpty(PREFIX): PREFIX = $$[QT_INSTALL_PREFIX]
isEmpty(INSTALL_LIB_DIR): INSTALL_LIB_DIR = $$[QT_INSTALL_LIBS]
isEmpty(INSTALL_TRANSLATIONS_DIR): INSTALL_TRANSLATIONS_DIR = $$[QT_INSTALL_TRANSLATIONS]

!contains(CONFIG, no_install_dev_files) {
    isEmpty(INSTALL_HEADERS_DIR): INSTALL_HEADERS_DIR = $$[QT_INSTALL_HEADERS]
    INSTALL_HEADERS += \
        Fuoten/Helpers/synchronizer.h \
        Fuoten/Helpers/AbstractConfiguration \
        Fuoten/Helpers/Synchronizer \
        Fuoten/Helpers/VersionNumber \
        Fuoten/Helpers/AccountValidator \
        Fuoten/Helpers/accountvalidator.h \
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
        Fuoten/baseitem.h

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

target = $$TARGET
target.path = $$INSTALL_LIB_DIR
INSTALLS += target

langfiles.path = $$INSTALL_TRANSLATIONS_DIR
langfiles.files = translations/*.qm
INSTALLS += langfiles

HEADERS += \
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
    Fuoten/Helpers/accountvalidator.h \
    Fuoten/Helpers/accountvalidator_p.h \
    Fuoten/fuoten.h \
    Fuoten/Helpers/versionnumber.h \
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
    Fuoten/API/markfeedread_p.h

SOURCES += \
    Fuoten/error.cpp \
    Fuoten/API/component.cpp \
    Fuoten/API/getversion.cpp \
    Fuoten/API/getstatus.cpp \
    Fuoten/API/getuser.cpp \
    Fuoten/Helpers/accountvalidator.cpp \
    Fuoten/Helpers/versionnumber.cpp \
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
    Fuoten/API/markfeedread.cpp

DISTFILES += \
    fuoten.pc.in
