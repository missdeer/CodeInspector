TEMPLATE      = lib
CONFIG       += plugin static c++17
QT           += widgets
TARGET        = $$qtLibraryTarget(tool_clangtidy)
DESTDIR       = $$OUT_PWD/../../../plugins

include($$PWD/../../plugins.pri)

CONFIG += install_ok  # Do not cargo-cult this!
uikit: CONFIG += debug_and_release

HEADERS += \
    $$PWD/clangtidyoutput.h \
    $$PWD/clangtidyplugin.h

SOURCES += \
    $$PWD/clangtidyoutput.cpp \
    $$PWD/clangtidyplugin.cpp
