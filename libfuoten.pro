TARGET = fuoten
TEMPLATE = lib

VER_MAJ = 0
VER_MIN = 0
VER_PAT = 1

QT += network
QT -= GUI

CONFIG -= console
CONFIG -= app_bundle
CONFIG += shared
CONFIG += c++11
CONFIG += no_keywords

DEFINES += FUOTEN_LIBRARY

DEFINES += VERSION_STRING=\"\\\"$${VERSION}\\\"\"

isEmpty(INSTALL_LIB_DIR): INSTALL_LIB_DIR = $$[QT_INSTALL_LIBS]
isEmpty(INSTALL_TRANSLATIONS_DIR): INSTALL_TRANSLATIONS_DIR = $$[QT_INSTALL_TRANSLATIONS]

target = $$TARGET
target.path = $$INSTALL_LIB_DIR
INSTALLS += target

langfiles.path = $$INSTALL_TRANSLATIONS_DIR
langfiles.files = translations/*.qm
INSTALLS += langfiles

HEADERS += \
    src/fuoten_global.h \
    src/error.h \
    src/error_p.h \
    src/component.h \
    src/component_p.h \
    src/Generic/version.h \
    src/Generic/version_p.h \
    src/Helpers/configuration.h \
    src/Generic/status.h \
    src/Generic/status_p.h \
    src/Generic/user.h \
    src/Generic/user_p.h \
    src/Generic/accountvalidator.h \
    src/Generic/accountvalidator_p.h \
    src/fuoten.h \
    src/Helpers/versionnumber.h \
    src/baseitem.h \
    src/baseitem_p.h \
    src/folderitem.h \
    src/folderitem_p.h \
    src/Helpers/storagehandler.h \
    src/Folders/getfolders.h \
    src/Folders/getfolders_p.h

SOURCES += \
    src/error.cpp \
    src/component.cpp \
    src/Generic/version.cpp \
    src/Helpers/configuration.cpp \
    src/Generic/status.cpp \
    src/Generic/user.cpp \
    src/Generic/accountvalidator.cpp \
    src/Helpers/versionnumber.cpp \
    src/baseitem.cpp \
    src/folderitem.cpp \
    src/Helpers/storagehandler.cpp \
    src/Folders/getfolders.cpp

