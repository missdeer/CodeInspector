TEMPLATE      = lib
CONFIG       += plugin static c++17
QT           += widgets
TARGET        = $$qtLibraryTarget(option_gcctreertl)
DESTDIR       = $$OUT_PWD/../../../plugins

include($$PWD/../../plugins.pri)

CONFIG += install_ok  # Do not cargo-cult this!
uikit: CONFIG += debug_and_release

HEADERS += \
    gcctreertloutput.h \
    gcctreertlplugin.h

SOURCES += \
    gcctreertloutput.cpp \
    gcctreertlplugin.cpp
