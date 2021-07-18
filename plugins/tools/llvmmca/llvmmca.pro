TEMPLATE      = lib
CONFIG       += plugin static c++17
QT           += widgets
TARGET        = $$qtLibraryTarget(tool_llvmmca)
DESTDIR       = $$OUT_PWD/../../../plugins

include($$PWD/../../plugins.pri)

CONFIG += install_ok  # Do not cargo-cult this!
uikit: CONFIG += debug_and_release

HEADERS += \
    $$PWD/llvmmachinecodeanalyzeroutput.h \
    $$PWD/llvmmcaplugin.h

SOURCES += \
    $$PWD/llvmmachinecodeanalyzeroutput.cpp \
    $$PWD/llvmmcaplugin.cpp
