TEMPLATE      = lib
CONFIG       += plugin static c++latest
QT           += widgets
TARGET        = $$qtLibraryTarget(tool_readelf)
DESTDIR       = $$OUT_PWD/../../../plugins

include($$PWD/../../plugins.pri)

CONFIG += install_ok  # Do not cargo-cult this!
uikit: CONFIG += debug_and_release

HEADERS += \
    $$PWD/readelfoutput.h \
    $$PWD/readelfplugin.h

SOURCES += \
    $$PWD/readelfoutput.cpp \
    $$PWD/readelfplugin.cpp
