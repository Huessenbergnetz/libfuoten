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
    src/API/component.h \
    src/API/component_p.h \
    src/API/getversion.h \
    src/API/getversion_p.h \
    src/Helpers/configuration.h \
    src/API/getstatus.h \
    src/API/getstatus_p.h \
    src/API/getuser.h \
    src/API/getuser_p.h \
    src/Helpers/accountvalidator.h \
    src/Helpers/accountvalidator_p.h \
    src/fuoten.h \
    src/Helpers/versionnumber.h \
    src/baseitem.h \
    src/baseitem_p.h \
    src/folder.h \
    src/folder_p.h \
    src/Helpers/storagehandler.h \
    src/API/getfolders.h \
    src/API/getfolders_p.h \
    src/Helpers/synchronizer.h

SOURCES += \
    src/error.cpp \
    src/API/component.cpp \
    src/API/getversion.cpp \
    src/Helpers/configuration.cpp \
    src/API/getstatus.cpp \
    src/API/getuser.cpp \
    src/Helpers/accountvalidator.cpp \
    src/Helpers/versionnumber.cpp \
    src/baseitem.cpp \
    src/folder.cpp \
    src/Helpers/storagehandler.cpp \
    src/API/getfolders.cpp

