cmake_minimum_required(VERSION 3.15)
project(ldd)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Widgets Core Gui Network Xml REQUIRED)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON) # if you have any .ui files
set(CMAKE_AUTORCC ON) # if you have any .qrc files

# Add compiler flags for building executables (-fPIE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

add_compile_definitions(SCINTILLA_QT=1
        SCI_LEXER=1
        _CRT_SECURE_NO_DEPRECATE=1
        SCI_STATIC_LINK=1
        EXPORT_IMPORT_API=
        )

include_directories(../../include
        ../../../3rdparty/scintilla/qt/ScintillaEdit
        ../../../3rdparty/scintilla/qt/ScintillaEditBase
        ../../../3rdparty/scintilla/include
        ../../../3rdparty/scintilla/src
        ../../../3rdparty/scintilla/lexlib
        ../../../3rdparty/scintilla/include
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

add_library(ldd
        ${SOURCES}
        )

target_include_directories (ldd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})