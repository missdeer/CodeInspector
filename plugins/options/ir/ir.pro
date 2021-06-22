TEMPLATE      = lib
CONFIG       += plugin
QT           += widgets
TARGET        = $$qtLibraryTarget(option_ir)
DESTDIR       = $$PWD/../../../plugins

include($$PWD/../../plugins.pri)

macx: {
    CONFIG+=static
}

CONFIG += install_ok  # Do not cargo-cult this!
uikit: CONFIG += debug_and_release

HEADERS += \
    llvmirplugin.h

SOURCES += \
    llvmirplugin.cpp
