TEMPLATE      = lib
CONFIG       += plugin static
QT           += widgets
TARGET        = $$qtLibraryTarget(option_optimization)
DESTDIR       = $$OUT_PWD/../../../plugins

include($$PWD/../../plugins.pri)

CONFIG += install_ok  # Do not cargo-cult this!
uikit: CONFIG += debug_and_release

HEADERS += \
    optimizationplugin.h

SOURCES += \
    optimizationplugin.cpp
