QT       += core gui widgets xml network concurrent sql qml quickwidgets widgets

lessThan(QT_MAJOR_VERSION, 5): error("Qt 5 is required")
greaterThan(QT_MINOR_VERSION, 10): CONFIG += qmlcompiler

TARGET = CodeInspector
TEMPLATE = app
CONFIG += c++11 precompile_header
PRECOMPILED_HEADER = stdafx.h

include($$PWD/3rdparty/scintilla/qt/ScintillaEdit/ScintillaEdit.pri)
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS SCINTILLA_QT=1 SCI_LEXER=1 _CRT_SECURE_NO_DEPRECATE=1 SCI_STATIC_LINK=1 STRSAFE_NO_DEPRECATE
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RC_FILE = codeinspector.rc

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    scintillaconfig.cpp \
    settings.cpp \
    codeeditor.cpp \
    codeinspector.cpp \
    godboltagent.cpp \
    networkreplyhelper.cpp \
    outputwindow.cpp \
    qmldialog.cpp \
    quickwidgetapi.cpp \
    applicationui.cpp \
    library.cpp \
    libraryversion.cpp

HEADERS += \
    stdafx.h \
    mainwindow.h \
    scintillaconfig.h \
    settings.h \
    codeeditor.h \
    codeinspector.h \
    godboltagent.h \
    networkreplyhelper.h \
    outputwindow.h \
    qmldialog.h \
    quickwidgetapi.h \
    applicationui.hpp \
    uiconstants.hpp \
    library.h \
    libraryversion.h

FORMS += \
    mainwindow.ui \
    qmldialog.ui

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    ci.qrc

win32-msvc* {
    QMAKE_LFLAGS += "/LTCG"
    QMAKE_CXXFLAGS_RELEASE += /Zi
    QMAKE_LFLAGS_RELEASE += /DEBUG
}
macx: {
    ICON = codeinspector.icns
    icon.path = $$PWD
    INSTALLS += icon
}

ios {
    AppIcons.files=$$system("find $$PWD/appicon/ios/ -name '*.png'")
    AppIcons.path=./
    LaunchImages.files=$$system("find $$PWD/launchimage/ios/ -name '*.png'")
    LaunchImages.path=./
    QMAKE_BUNDLE_DATA += AppIcons LaunchImages
    QMAKE_INFO_PLIST = iosInfo.plist
}

android: {
    QT += androidextras
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
    OTHER_FILES += $$PWD/android/AndroidManifest.xml \
        $$PWD/android/res/values/strings.xml \
        $$PWD/android/res/values/styles.xml
    # prebuilt OpenSSL binaries from http://doc.qt.io/qt-5/opensslsupport.html
    contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
        ANDROID_EXTRA_LIBS = \
            $$PWD/android/libs/armeabi-v7a/libcrypto.so \
            $$PWD/android/libs/armeabi-v7a/libssl.so
    }
}

