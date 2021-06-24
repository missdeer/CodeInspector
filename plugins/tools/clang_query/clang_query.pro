TEMPLATE      = lib
CONFIG       += plugin static c++17
QT           += widgets
TARGET        = $$qtLibraryTarget(tool_clangquery)
DESTDIR       = $$OUT_PWD/../../../plugins

include($$PWD/../../plugins.pri)

CONFIG += install_ok  # Do not cargo-cult this!
uikit: CONFIG += debug_and_release

HEADERS += \
    clangqueryoutput.h \
    clangqueryplugin.h

SOURCES += \
    clangqueryoutput.cpp \
    clangqueryplugin.cpp
