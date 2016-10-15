TARGET = fuoten
TEMPLATE = lib
VERSION = 0.0.1

QT += network
QT -= GUI

CONFIG -= console
CONFIG -= app_bundle
CONFIG += shared
CONFIG += c++11
CONFIG += ne_keywords

DEFINES += FUOTEN_LIBRARY

target.path = $$INSTALL_DIB_DIR
INSTALLS += target

HEADERS += \
    src/fuoten_global.h

