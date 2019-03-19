QT       += core gui widgets quickwidgets

lessThan(QT_MAJOR_VERSION, 5): error("Qt 5 is required")
greaterThan(QT_MINOR_VERSION, 10): CONFIG += qtquickcompiler

TARGET = CodeInspector
TEMPLATE = app
CONFIG += c++11 precompile_header
PRECOMPILED_HEADER = stdafx.h

include($$PWD/../core/core.pri)

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    qmldialog.cpp \
    quickwidgetapi.cpp \
    applicationui.cpp

HEADERS += \
    stdafx.h \
    mainwindow.h \
    qmldialog.h \
    quickwidgetapi.h \
    applicationui.hpp \
    uiconstants.hpp

FORMS += \
    mainwindow.ui \
    qmldialog.ui

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    qml.qrc \ 
    image.qrc

ios {
    QMAKE_ASSET_CATALOGS = $$PWD/appicon/ios/Images.xcassets
    QMAKE_ASSET_CATALOGS_APP_ICON = "AppIcon"
    xcode_product_bundle_identifier_setting.value = "com.dfordsoft.codeinspector"
    LaunchImages.files=$$system("find $$PWD/launchimage/ios/ -name '*.png'")
    LaunchImages.path=./
    QMAKE_BUNDLE_DATA += LaunchImages
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

DISTFILES += \
    resource/qml/*.qml \
    resource/qml/pages/*.qml \
    resource/qml/common/*.qml \
    resource/qml/navigation/*.qml \
    resource/qml/popups/*.qml \
    resource/qml/tabs/*.qml\

