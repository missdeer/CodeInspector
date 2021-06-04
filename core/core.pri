QT       += core gui widgets xml network 

android|ios: {
    include($$PWD/../3rdparty/quickscintilla.pri)
} else: {
    include($$PWD/../3rdparty/scintilla-latest.pri)
    include($$PWD/../3rdparty/lexilla-latest.pri)
    include($$PWD/../3rdparty/scintillua-latest.pri)
}

include($$PWD/../3rdparty/zlib.pri)

INCLUDEPATH += $$PWD 

SOURCES += \
    $$PWD/clangqueryoutput.cpp \
    $$PWD/includewhatyouuseoutput.cpp \
    $$PWD/lddoutput.cpp \
    $$PWD/readelfoutput.cpp \
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
    $$PWD/clangtidyoutput.cpp \
    $$PWD/x86to6502output.cpp \
    $$PWD/llvmirview.cpp

HEADERS += \
    $$PWD/clangqueryoutput.h \
    $$PWD/includewhatyouuseoutput.h \
    $$PWD/lddoutput.h \
    $$PWD/readelfoutput.h \
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
    $$PWD/clangtidyoutput.h \
    $$PWD/x86to6502output.h \
    $$PWD/llvmirview.h

	
RESOURCES += \
    $$PWD/CodeInspector.qrc \
    $$PWD/example.qrc \
    $$PWD/scintilla.qrc \
    $$PWD/coreImage.qrc
