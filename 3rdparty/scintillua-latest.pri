INCLUDEPATH += \
    $$PWD/scintillua

SOURCES += \
    $$files($$PWD/scintillua/*.cxx, false)  

HEADERS += \
    $$files($$PWD/scintillua/*.h, false) 

DEFINES += NO_DLL=1 SCI_LEXER=1
CONFIG(release, debug|release) {
    DEFINES += NDEBUG=1
}

RESOURCES += \
    $$PWD/scintillua-lexers.qrc
