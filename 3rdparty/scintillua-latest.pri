INCLUDEPATH += $$PWD/scintilla/include \
    $$PWD/lexilla/include \
    $$PWD/lexilla/src \
    $$PWD/lexilla/lexlib \
    $$PWD/scintillua \
    $$PWD/lpeg \
    $$PWD/lua

SOURCES += \
    $$files($$PWD/scintillua/*.cxx, false)  \
    $$files($$PWD/lpeg/*.c, false) \
    $$PWD/lua/lapi.c \
    $$PWD/lua/lcode.c \
    $$PWD/lua/lctype.c \
    $$PWD/lua/ldebug.c \
    $$PWD/lua/ldo.c \
    $$PWD/lua/ldump.c \
    $$PWD/lua/lfunc.c \
    $$PWD/lua/lgc.c \
    $$PWD/lua/linit.c \
    $$PWD/lua/llex.c \
    $$PWD/lua/lmem.c \
    $$PWD/lua/lobject.c \
    $$PWD/lua/lopcodes.c \
    $$PWD/lua/lparser.c \
    $$PWD/lua/lstate.c \
    $$PWD/lua/lstring.c \
    $$PWD/lua/ltable.c \
    $$PWD/lua/ltm.c \
    $$PWD/lua/lundump.c \
    $$PWD/lua/lvm.c \
    $$PWD/lua/lzio.c \
    $$PWD/lua/lauxlib.c \
    $$PWD/lua/lbaselib.c \
    $$PWD/lua/lbitlib.c \
    $$PWD/lua/lcorolib.c \
    $$PWD/lua/ldblib.c \
    $$PWD/lua/liolib.c \
    $$PWD/lua/lmathlib.c \
    $$PWD/lua/loadlib.c \
    $$PWD/lua/loslib.c \
    $$PWD/lua/lstrlib.c \
    $$PWD/lua/ltablib.c \
    $$PWD/lua/lutf8lib.c

HEADERS += \
    $$files($$PWD/scintillua/*.h, false) \
    $$files($$PWD/lua/*.h, false) \
    $$files($$PWD/lpeg/*.h, false)

DEFINES += NO_DLL=1 SCI_LEXER=1
CONFIG(release, debug|release) {
    DEFINES += NDEBUG=1
}

RESOURCES += \
    $$PWD/scintillua-lexers.qrc
