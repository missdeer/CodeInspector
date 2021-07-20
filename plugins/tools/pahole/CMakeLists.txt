cmake_minimum_required(VERSION 3.15)
project(pahole VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets Core Gui Network Xml REQUIRED)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON) # if you have any .ui files
set(CMAKE_AUTORCC ON) # if you have any .qrc files

# Add compiler flags for building executables (-fPIE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")


include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_BINARY_DIR})

add_compile_definitions(SCINTILLA_QT=1
        SCI_LEXER=1
        _CRT_SECURE_NO_DEPRECATE=1
        SCI_STATIC_LINK=1
        EXPORT_IMPORT_API=
        QT_STATICPLUGIN=
        )

add_definitions(${Qt5Widgets_DEFINITIONS}
        ${Qt5Network_DEFINITIONS}
        ${Qt5Xml_DEFINITIONS}
        )

include_directories(../../include
        ../../../3rdparty/scintilla/qt/ScintillaEdit
        ../../../3rdparty/scintilla/qt/ScintillaEditBase
        ../../../3rdparty/scintilla/include
        ../../../3rdparty/scintilla/src
        ../../../3rdparty/lexilla/lexlib
        ../../../3rdparty/lexilla/include
        ../../../core
        ${Qt5Widgets_INCLUDE_DIRS}
        ${Qt5Network_INCLUDE_DIRS}
        ${Qt5Xml_INCLUDE_DIRS}
        )


file(GLOB
        headers
        ./*.h
        )

file(GLOB
        src
        ./*.cpp
        ./*.cpp
        ./*.cpp
        )

set(SOURCES
        ${src}
        )

qt5_wrap_cpp(SOURCES
        ${headers}
        )

add_library(pahole
        ${SOURCES}
        )

target_include_directories(pahole PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})