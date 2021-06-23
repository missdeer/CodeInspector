QT       += core gui widgets xml network

lessThan(QT_MAJOR_VERSION, 5): error("Qt 5 or higher is required")
greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat
win32-msvc: {
    MSVC_VER = $$(VisualStudioVersion)
    lessThan(MSVC_VER, 14.1): error("Compiler supports C++17 is required")
}

TARGET = CodeInspector
TEMPLATE = app
CONFIG += c++17 
! *-clang* {
    CONFIG += precompile_header
    PRECOMPILED_HEADER = stdafx.h
}

include($$PWD/../core/core.pri)

RC_FILE = CodeInspector.rc

SOURCES += main.cpp \
    comboboxdelegate.cpp \
    mainwindow.cpp \
    codeeditorpane.cpp \
    codeinspectortabwidget.cpp \
    codeinspectorpane.cpp \
    compilationtabwidget.cpp \
    sessionwidget.cpp \
    codeinspectorapp.cpp

HEADERS += stdafx.h \
    comboboxdelegate.h \
    mainwindow.h \
    codeeditorpane.h \
    codeinspectortabwidget.h \
    codeinspectorpane.h \
    compilationtabwidget.h \
    sessionwidget.h \
    codeinspectorapp.h

FORMS += \ 
    mainwindow.ui


RESOURCES += \ 
    image.qrc

win32-msvc* {
    #QMAKE_LFLAGS += "/LTCG"
    QMAKE_CXXFLAGS_RELEASE += /Zi
    QMAKE_LFLAGS_RELEASE += /DEBUG
}

macx: {
    ICON = CodeInspector.icns
    icon.path = $$PWD
    INSTALLS += icon
}

LIBS = -L$$PWD/../plugins -L$$OUT_PWD/../libs

macx-xcode {
    LIBS += -loption_ast$($${QMAKE_XCODE_LIBRARY_SUFFIX_SETTING}) \
        -loption_cfg$($${QMAKE_XCODE_LIBRARY_SUFFIX_SETTING}) \
        -loption_gcctreertl$($${QMAKE_XCODE_LIBRARY_SUFFIX_SETTING}) \
        -loption_ir$($${QMAKE_XCODE_LIBRARY_SUFFIX_SETTING}) \
        -loption_optimization$($${QMAKE_XCODE_LIBRARY_SUFFIX_SETTING}) \
        -lrunner_rextester$($${QMAKE_XCODE_LIBRARY_SUFFIX_SETTING}) \
        -lrunner_wandbox$($${QMAKE_XCODE_LIBRARY_SUFFIX_SETTING}) \
        -ltool_clangformat$($${QMAKE_XCODE_LIBRARY_SUFFIX_SETTING}) \
        -ltool_clangquery$($${QMAKE_XCODE_LIBRARY_SUFFIX_SETTING}) \
        -ltool_clangtidy$($${QMAKE_XCODE_LIBRARY_SUFFIX_SETTING}) \
        -ltool_iwyu$($${QMAKE_XCODE_LIBRARY_SUFFIX_SETTING}) \
        -ltool_ldd$($${QMAKE_XCODE_LIBRARY_SUFFIX_SETTING}) \
        -ltool_llvmmca$($${QMAKE_XCODE_LIBRARY_SUFFIX_SETTING}) \
        -ltool_osaca$($${QMAKE_XCODE_LIBRARY_SUFFIX_SETTING}) \
        -ltool_pahole$($${QMAKE_XCODE_LIBRARY_SUFFIX_SETTING}) \
        -ltool_psvstudio$($${QMAKE_XCODE_LIBRARY_SUFFIX_SETTING}) \
        -ltool_readelf$($${QMAKE_XCODE_LIBRARY_SUFFIX_SETTING}) \
        -ltool_strings$($${QMAKE_XCODE_LIBRARY_SUFFIX_SETTING}) \
        -ltool_x86to6502$($${QMAKE_XCODE_LIBRARY_SUFFIX_SETTING}) \
        -lcore$($${QMAKE_XCODE_LIBRARY_SUFFIX_SETTING})
} else {
    LIBS += -loption_ast \
        -loption_cfg \
        -loption_gcctreertl \
        -loption_ir \
        -loption_optimization \
        -lrunner_rextester \
        -lrunner_wandbox \
        -ltool_clangformat \
        -ltool_clangquery \
        -ltool_clangtidy \
        -ltool_iwyu \
        -ltool_ldd \
        -ltool_llvmmca \
        -ltool_osaca \
        -ltool_pahole \
        -ltool_psvstudio \
        -ltool_readelf \
        -ltool_strings \
        -ltool_x86to6502 \
        -lcore
    if(!debug_and_release|build_pass):CONFIG(debug, debug|release) {
        mac:LIBS = $$member(LIBS, 0) $$member(LIBS, 1)_debug
        win32:LIBS = $$member(LIBS, 0) $$member(LIBS, 1)d
    }
}
