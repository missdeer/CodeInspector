TEMPLATE      = lib
CONFIG       += plugin static c++latest
QT           += widgets
TARGET        = $$qtLibraryTarget(tool_x86to6502)
DESTDIR       = $$OUT_PWD/../../../plugins

include($$PWD/../../plugins.pri)

CONFIG += install_ok  # Do not cargo-cult this!
uikit: CONFIG += debug_and_release

HEADERS += \
    x86to6502output.h \
    x86to6502plugin.h

SOURCES += \
    x86to6502output.cpp \
    x86to6502plugin.cpp
