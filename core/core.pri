QT       += core gui widgets xml network concurrent sql qml

include($$PWD/../3rdparty/scintilla/qt/ScintillaEdit/ScintillaEdit.pri)
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS SCINTILLA_QT=1 SCI_LEXER=1 _CRT_SECURE_NO_DEPRECATE=1 SCI_STATIC_LINK=1 STRSAFE_NO_DEPRECATE
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/scintillaconfig.cpp \
    $$PWD/settings.cpp \
    $$PWD/codeeditor.cpp \
    $$PWD/codeinspector.cpp \
    $$PWD/godboltagent.cpp \
    $$PWD/networkreplyhelper.cpp  \
    $$PWD/library.cpp \
    $$PWD/libraryversion.cpp \
    $$PWD/optimizationoutput.cpp \
    $$PWD/astoutput.cpp \
    $$PWD/gcctreertloutput.cpp \
    $$PWD/llvmmachinecodeanalyzeroutput.cpp \
    $$PWD/codeeditorpane.cpp \
    $$PWD/outputwindow.cpp \
    $$PWD/codeinspectortabwidget.cpp \
    $$PWD/codeinspectorpane.cpp \
    $$PWD/compilationtabwidget.cpp \
    $$PWD/sessionwidget.cpp

HEADERS += \
    $$PWD/scintillaconfig.h \
    $$PWD/settings.h \
    $$PWD/codeeditor.h \
    $$PWD/codeinspector.h \
    $$PWD/godboltagent.h \
    $$PWD/networkreplyhelper.h  \
    $$PWD/library.h \
    $$PWD/libraryversion.h \
    $$PWD/optimizationoutput.h \
    $$PWD/astoutput.h \
    $$PWD/gcctreertloutput.h \
    $$PWD/llvmmachinecodeanalyzeroutput.h \
    $$PWD/codeeditorpane.h \
    $$PWD/outputwindow.h \
    $$PWD/codeinspectortabwidget.h \
    $$PWD/codeinspectorpane.h \
    $$PWD/compilationtabwidget.h \
    $$PWD/sessionwidget.h

	
RESOURCES += \
    $$PWD/CodeInspector.qrc \
    $$PWD/example.qrc \
    $$PWD/scintilla.qrc \
    $$PWD/coreImage.qrc
