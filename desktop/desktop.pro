QT       += core gui widgets 

lessThan(QT_MAJOR_VERSION, 5): error("Qt 5 is required")

TARGET = CodeInspector
TEMPLATE = app
CONFIG += c++11 precompile_header
PRECOMPILED_HEADER = stdafx.h

include($$PWD/../core/core.pri)

RC_FILE = CodeInspector.rc

SOURCES += main.cpp \
    mainwindow.cpp

HEADERS += stdafx.h \
    mainwindow.h

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

