#-------------------------------------------------
#
# Project created by QtCreator 2011-05-05T12:41:23
#
#-------------------------------------------------

QT       += core gui widgets network

CONFIG += c++17

SOURCES += \
    $$PWD/KF5SyntaxHighlighting/src/lib/*.cpp

HEADERS  += \
    $$PWD/KF5SyntaxHighlighting/src/lib/*.h


OTHER_FILES +=

INCLUDEPATH += $$PWD/KF5SyntaxHighlighting/src/lib/

DEFINES += KSYNTAXHIGHLIGHTING_LIBRARY=

RESOURCES += $$PWD/KF5SyntaxHighlighting/data/themes/theme-data.qrc

CONFIG(release, debug|release) {
    DEFINES += NDEBUG=1
}

macx {
    QMAKE_LFLAGS_SONAME = -Wl,-install_name,@executable_path/../Frameworks/
}
