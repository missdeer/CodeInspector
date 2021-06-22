TEMPLATE      = lib
CONFIG       += plugin
QT           += widgets
TARGET        = $$qtLibraryTarget(tool_readelf)
DESTDIR       = $$PWD/../../../plugins

include($$PWD/../../plugins.pri)

macx: {
    CONFIG+=static
}

CONFIG += install_ok  # Do not cargo-cult this!
uikit: CONFIG += debug_and_release

HEADERS += \
    readelfplugin.h

SOURCES += \
    readelfplugin.cpp
