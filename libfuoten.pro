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

target = $$TARGET
target.path = $$INSTALL_LIB_DIR
INSTALLS += target

HEADERS += \
    src/fuoten_global.h \
    src/error.h \
    src/error_p.h \
    src/component.h \
    src/component_p.h \
    src/Generic/version.h \
    src/Generic/version_p.h \
    src/Helpers/configuration.h \
    src/Helpers/newsappversion.h \
    src/Generic/status.h \
    src/Generic/status_p.h \
    src/Generic/user.h \
    src/Generic/user_p.h \
    src/Generic/accountvalidator.h \
    src/Generic/accountvalidator_p.h \
    src/fuoten.h

SOURCES += \
    src/error.cpp \
    src/component.cpp \
    src/Generic/version.cpp \
    src/Helpers/configuration.cpp \
    src/Helpers/newsappversion.cpp \
    src/Generic/status.cpp \
    src/Generic/user.cpp \
    src/Generic/accountvalidator.cpp

