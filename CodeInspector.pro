#-------------------------------------------------
#
# Project created by QtCreator 2017-09-04T17:15:20
#
#-------------------------------------------------

QT       += core gui widgets xml network concurrent sql qml quickwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CodeInspector
TEMPLATE = app
CONFIG += c++11 precompile_header
PRECOMPILED_HEADER = stdafx.h

include($$PWD/3rdparty/scintilla/qt/ScintillaEdit/ScintillaEdit.pri)
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS SCINTILLA_QT=1 SCI_LEXER=1 _CRT_SECURE_NO_DEPRECATE=1 SCI_STATIC_LINK=1
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    configdialog.cpp

HEADERS += \
        stdafx.h \
        mainwindow.h \
    configdialog.h

FORMS += \
        mainwindow.ui \
    configdialog.ui

CONFIG += mobility
MOBILITY = 

