TEMPLATE      = lib
CONFIG       += plugin
QT           += widgets
TARGET        = $$qtLibraryTarget(tool_clangtidy)
DESTDIR       = $$PWD/../../../plugins

include($$PWD/../../plugins.pri)

macx: {
    CONFIG+=static
}

CONFIG += install_ok  # Do not cargo-cult this!
uikit: CONFIG += debug_and_release

HEADERS += \
    clangtidyplugin.h

SOURCES += \
    clangtidyplugin.cpp
