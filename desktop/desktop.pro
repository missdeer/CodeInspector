QT       += core gui widgets 

lessThan(QT_MAJOR_VERSION, 5): error("Qt 5 is required")
*msvc* {
    MSVC_VER = $$(VisualStudioVersion)
    lessThan(MSVC_VER, 14.1): error("Compiler supports C++17 is required")
}

TARGET = CodeInspector
TEMPLATE = app
CONFIG += c++17 precompile_header
PRECOMPILED_HEADER = stdafx.h

include($$PWD/../core/core.pri)

RC_FILE = CodeInspector.rc

SOURCES += main.cpp \
    mainwindow.cpp \
    codeeditorpane.cpp \
    codeinspectortabwidget.cpp \
    codeinspectorpane.cpp \
    compilationtabwidget.cpp \
    sessionwidget.cpp \
    codeinspectorapp.cpp \
    popupmenutoolbutton.cpp

HEADERS += stdafx.h \
    mainwindow.h \
    codeeditorpane.h \
    codeinspectortabwidget.h \
    codeinspectorpane.h \
    compilationtabwidget.h \
    sessionwidget.h \
    codeinspectorapp.h \
    popupmenutoolbutton.h

FORMS += \ 
    mainwindow.ui


RESOURCES += \ 
    image.qrc

win32-msvc* {
    QMAKE_LFLAGS += "/LTCG"
    QMAKE_CXXFLAGS_RELEASE += /Zi
    QMAKE_LFLAGS_RELEASE += /DEBUG
}

macx: {
    ICON = CodeInspector.icns
    icon.path = $$PWD
    INSTALLS += icon
}

