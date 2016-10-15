TARGET = fuoten
TEMPLATE = lib
VERSION = 0.0.1

QT += network
QT -= GUI

CONFIG -= console
CONFIG -= app_bundle
CONFIG += shared
CONFIG += c++11
CONFIG += no_keywords

DEFINES += FUOTEN_LIBRARY

isEmpty(INSTALL_LIB_DIR): INSTALL_LIB_DIR = $$[QT_INSTALL_LIBS]

target = $$TARGET
target.path = $$INSTALL_LIB_DIR
INSTALLS += target

HEADERS += \
    src/fuoten_global.h \
    src/error.h \
    src/error_p.h

SOURCES += \
    src/error.cpp

