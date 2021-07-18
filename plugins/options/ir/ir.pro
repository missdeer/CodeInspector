TEMPLATE      = lib
CONFIG       += plugin static c++17
QT           += widgets
TARGET        = $$qtLibraryTarget(option_ir)
DESTDIR       = $$OUT_PWD/../../../plugins

include($$PWD/../../plugins.pri)

CONFIG += install_ok  # Do not cargo-cult this!
uikit: CONFIG += debug_and_release

HEADERS += \
    $$PWD/llvmirplugin.h \
    $$PWD/llvmirview.h

SOURCES += \
    $$PWD/llvmirplugin.cpp \
    $$PWD/llvmirview.cpp
