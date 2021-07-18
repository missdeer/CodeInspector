TEMPLATE      = lib
CONFIG       += plugin static c++17
QT           += widgets
TARGET        = $$qtLibraryTarget(option_ast)
DESTDIR       = $$OUT_PWD/../../../plugins

include($$PWD/../../plugins.pri)

CONFIG += install_ok  # Do not cargo-cult this!
uikit: CONFIG += debug_and_release

HEADERS += \
    $$PWD/astoutput.h \
    $$PWD/astplugin.h

SOURCES += \
    $$PWD/astoutput.cpp \
    $$PWD/astplugin.cpp
