TEMPLATE      = lib
CONFIG       += plugin static c++latest
QT           += widgets
TARGET        = $$qtLibraryTarget(tool_clangquery)
DESTDIR       = $$OUT_PWD/../../../plugins

include($$PWD/../../plugins.pri)

CONFIG += install_ok  # Do not cargo-cult this!
uikit: CONFIG += debug_and_release

HEADERS += \
    $$PWD/clangqueryoutput.h \
    $$PWD/clangqueryplugin.h

SOURCES += \
    $$PWD/clangqueryoutput.cpp \
    $$PWD/clangqueryplugin.cpp
