#-------------------------------------------------
#
# Project created by QtCreator 2011-05-05T12:41:23
#
#-------------------------------------------------

QT       += core gui widgets

CONFIG += c++17

VERSION = 4.1.4

SOURCES += \
    $$PWD/ScintillaEdit.cpp \
    $$PWD/ScintillaDocument.cpp \
    $$PWD/../ScintillaEditBase/PlatQt.cpp \
    $$PWD/../ScintillaEditBase/ScintillaQt.cpp \
    $$PWD/../ScintillaEditBase/ScintillaEditBase.cpp \
    $$PWD/../../src/*.cxx \
    $$PWD/../../lexlib/*.cxx \
    $$PWD/../../lexers/*.cxx

HEADERS  += \
    $$PWD/ScintillaEdit.h \
    $$PWD/ScintillaDocument.h \
    $$PWD/../ScintillaEditBase/ScintillaEditBase.h \
    $$PWD/../ScintillaEditBase/ScintillaQt.h \
    $$PWD/../../src/*.h \
    $$PWD/../../lexlib/*.h \
    $$PWD/../../include/*.h

OTHER_FILES +=

INCLUDEPATH += $$PWD/../ScintillaEdit $$PWD/../ScintillaEditBase $$PWD/../../include $$PWD/../../src $$PWD/../../lexlib $$PWD/../../include

DEFINES += SCINTILLA_QT=1 SCI_LEXER=1 _CRT_SECURE_NO_DEPRECATE=1 SCI_STATIC_LINK=1
CONFIG(release, debug|release) {
    DEFINES += NDEBUG=1
}

macx {
	QMAKE_LFLAGS_SONAME = -Wl,-install_name,@executable_path/../Frameworks/
}
