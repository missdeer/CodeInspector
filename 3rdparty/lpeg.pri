INCLUDEPATH += \
    $$PWD/lpeg \
    $$PWD/lua

SOURCES += \
    $$files($$PWD/lpeg/*.c, false) 

HEADERS += \
    $$files($$PWD/lpeg/*.h, false)

DEFINES += NO_DLL=1 
CONFIG(release, debug|release) {
    DEFINES += NDEBUG=1
}