INCLUDEPATH += \
    $$PWD/

SOURCES += \
    $$PWD/lapi.c \
    $$PWD/lcode.c \
    $$PWD/lctype.c \
    $$PWD/ldebug.c \
    $$PWD/ldo.c \
    $$PWD/ldump.c \
    $$PWD/lfunc.c \
    $$PWD/lgc.c \
    $$PWD/linit.c \
    $$PWD/llex.c \
    $$PWD/lmem.c \
    $$PWD/lobject.c \
    $$PWD/lopcodes.c \
    $$PWD/lparser.c \
    $$PWD/lstate.c \
    $$PWD/lstring.c \
    $$PWD/ltable.c \
    $$PWD/ltm.c \
    $$PWD/lundump.c \
    $$PWD/lvm.c \
    $$PWD/lzio.c \
    $$PWD/lauxlib.c \
    $$PWD/lbaselib.c \
    $$PWD/lbitlib.c \
    $$PWD/lcorolib.c \
    $$PWD/ldblib.c \
    $$PWD/liolib.c \
    $$PWD/lmathlib.c \
    $$PWD/loadlib.c \
    $$PWD/loslib.c \
    $$PWD/lstrlib.c \
    $$PWD/ltablib.c \
    $$PWD/lutf8lib.c

HEADERS += \
    $$files($$PWD/*.h, false) \
    $$files($$PWD/*.hpp, false)

DEFINES += NO_DLL=1 SCI_LEXER=1
CONFIG(release, debug|release) {
    DEFINES += NDEBUG=1
}
