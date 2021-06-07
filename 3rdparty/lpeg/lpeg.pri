INCLUDEPATH += \
    $$PWD

SOURCES += \
    $$files($$PWD/*.c, false)

HEADERS += \
    $$files($$PWD/*.h, false)

DEFINES += NO_DLL=1 
CONFIG(release, debug|release) {
    DEFINES += NDEBUG=1
}
