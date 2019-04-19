QT       += core gui widgets xml network 

include($$PWD/../3rdparty/scintilla-latest.pri)

INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/scintillaconfig.cpp \
    $$PWD/scopedguard.cpp \
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
    $$PWD/outputwindow.cpp \
    $$PWD/paholeoutput.cpp \
    $$PWD/clangtidyoutput.cpp

HEADERS += \
    $$PWD/scintillaconfig.h \
    $$PWD/scopedguard.h \
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
    $$PWD/outputwindow.h \
    $$PWD/language.hpp \
    $$PWD/compiler.hpp \
    $$PWD/compilerinfo.hpp \
    $$PWD/asmlink.hpp \
    $$PWD/asmitem.hpp \
    $$PWD/backendinterface.hpp \
    $$PWD/optimizationitem.hpp \
    $$PWD/paholeoutput.h \
    $$PWD/clangtidyoutput.h

	
RESOURCES += \
    $$PWD/CodeInspector.qrc \
    $$PWD/example.qrc \
    $$PWD/scintilla.qrc \
    $$PWD/coreImage.qrc
