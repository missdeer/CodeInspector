TEMPLATE      = lib
CONFIG       += c++latest static
TARGET        = $$qtLibraryTarget(core)
DESTDIR       = $$OUT_PWD/../libs
QT           += core gui widgets xml network

lessThan(QT_MAJOR_VERSION, 5): error("Qt 5 or higher is required")
equals(QT_MAJOR_VERSION, 6): QT += core5compat

win32-msvc: {
    MSVC_VER = $$(VisualStudioVersion)
    lessThan(MSVC_VER, 14.1): error("Compiler supports C++17 is required")
}

include($$PWD/../3rdparty/lua/lua.pri)
include($$PWD/../3rdparty/zlib.pri)
include($$PWD/../3rdparty/lpeg/lpeg.pri)
include($$PWD/../3rdparty/scintillua-latest.pri)

android|ios: {
    include($$PWD/../3rdparty/quickscintilla.pri)
} else: {
    include($$PWD/../3rdparty/scintilla-latest.pri)
    include($$PWD/../3rdparty/lexilla-latest.pri)
}

CONFIG += install_ok  # Do not cargo-cult this!
uikit: CONFIG += debug_and_release

SOURCES += \
    $$PWD/luavm.cpp \
    $$PWD/networkrequesthelper.cpp \
    $$PWD/scintillaconfig.cpp \
    $$PWD/scopedguard.cpp \
    $$PWD/settings.cpp \
    $$PWD/codeeditor.cpp \
    $$PWD/codeinspector.cpp \
    $$PWD/godboltagent.cpp \
    $$PWD/networkreplyhelper.cpp  \
    $$PWD/library.cpp \
    $$PWD/libraryversion.cpp \
    $$PWD/outputwindow.cpp

HEADERS += \
    $$PWD/luavm.h \
    $$PWD/networkrequesthelper.h \
    $$PWD/scintillaconfig.h \
    $$PWD/scopedguard.h \
    $$PWD/settings.h \
    $$PWD/codeeditor.h \
    $$PWD/codeinspector.h \
    $$PWD/godboltagent.h \
    $$PWD/networkreplyhelper.h  \
    $$PWD/library.h \
    $$PWD/libraryversion.h \
    $$PWD/outputwindow.h \
    $$PWD/language.hpp \
    $$PWD/compiler.hpp \
    $$PWD/compilerinfo.hpp \
    $$PWD/asmlink.hpp \
    $$PWD/asmitem.hpp \
    $$PWD/backendinterface.hpp \
    $$PWD/optimizationitem.hpp


RESOURCES += \
    $$PWD/CodeInspector.qrc \
    $$PWD/example.qrc \
    $$PWD/scintilla.qrc \
    $$PWD/coreImage.qrc
