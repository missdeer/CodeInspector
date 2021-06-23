TEMPLATE      = lib
CONFIG       += plugin static
QT           += widgets
TARGET        = $$qtLibraryTarget(option_cfg)
DESTDIR       = $$OUT_PWD/../../../plugins

include($$PWD/../../plugins.pri)

CONFIG += install_ok  # Do not cargo-cult this!
uikit: CONFIG += debug_and_release

HEADERS += \
    cfgplugin.h

SOURCES += \
    cfgplugin.cpp
