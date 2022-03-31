TEMPLATE      = lib
CONFIG       += plugin static c++latest
QT           += widgets
TARGET        = $$qtLibraryTarget(tool_iwyu)
DESTDIR       = $$OUT_PWD/../../../plugins

include($$PWD/../../plugins.pri)

CONFIG += install_ok  # Do not cargo-cult this!
uikit: CONFIG += debug_and_release

HEADERS += \
    $$PWD/includewhatyouuseoutput.h \
    $$PWD/includewhatyouuseplugin.h

SOURCES += \
    $$PWD/includewhatyouuseoutput.cpp \
    $$PWD/includewhatyouuseplugin.cpp
