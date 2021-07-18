TEMPLATE      = lib
CONFIG       += plugin static c++17
QT           += widgets
TARGET        = $$qtLibraryTarget(tool_pahole)
DESTDIR       = $$OUT_PWD/../../../plugins

include($$PWD/../../plugins.pri)

CONFIG += install_ok  # Do not cargo-cult this!
uikit: CONFIG += debug_and_release

HEADERS += \
    $$PWD/paholeoutput.h \
    $$PWD/paholeplugin.h

SOURCES += \
    $$PWD/paholeoutput.cpp \
    $$PWD/paholeplugin.cpp
