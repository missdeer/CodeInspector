TEMPLATE      = lib
CONFIG       += plugin static c++17
QT           += widgets
TARGET        = $$qtLibraryTarget(tool_ldd)
DESTDIR       = $$OUT_PWD/../../../plugins

include($$PWD/../../plugins.pri)

CONFIG += install_ok  # Do not cargo-cult this!
uikit: CONFIG += debug_and_release

HEADERS += \
    lddoutput.h \
    lddplugin.h

SOURCES += \
    lddoutput.cpp \
    lddplugin.cpp
