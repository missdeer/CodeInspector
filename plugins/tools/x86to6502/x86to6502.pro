TEMPLATE      = lib
CONFIG       += plugin
QT           += widgets
INCLUDEPATH  += $$PWD/../include
TARGET        = $$qtLibraryTarget(tool_x86to6502)
DESTDIR       = $$PWD/../../plugins

macx: {
    CONFIG+=static
}

CONFIG += install_ok  # Do not cargo-cult this!
uikit: CONFIG += debug_and_release
