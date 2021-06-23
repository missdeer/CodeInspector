TEMPLATE      = lib
CONFIG       += plugin static
QT           += widgets
TARGET        = $$qtLibraryTarget(tool_strings)
DESTDIR       = $$OUT_PWD/../../../plugins

include($$PWD/../../plugins.pri)

CONFIG += install_ok  # Do not cargo-cult this!
uikit: CONFIG += debug_and_release

HEADERS += \
    stringsplugin.h

SOURCES += \
    stringsplugin.cpp
