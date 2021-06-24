TEMPLATE      = lib
CONFIG       += plugin static c++17
QT           += widgets
TARGET        = $$qtLibraryTarget(tool_llvmmca)
DESTDIR       = $$OUT_PWD/../../../plugins

include($$PWD/../../plugins.pri)

CONFIG += install_ok  # Do not cargo-cult this!
uikit: CONFIG += debug_and_release

HEADERS += \
    llvmmachinecodeanalyzeroutput.h \
    llvmmcaplugin.h

SOURCES += \
    llvmmachinecodeanalyzeroutput.cpp \
    llvmmcaplugin.cpp
