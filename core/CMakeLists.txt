cmake_minimum_required(VERSION 3.15)
project(core VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network Xml LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network Xml LinguistTools)
find_package(Qt6 COMPONENTS Core5Compat REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON) # if you have any .ui files
set(CMAKE_AUTORCC ON) # if you have any .qrc files

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_BINARY_DIR})

add_compile_definitions(SCINTILLA_QT=1
        SCI_LEXER=1
        _CRT_SECURE_NO_DEPRECATE=1
        SCI_STATIC_LINK=1
        NO_DLL=1
        EXPORT_IMPORT_API=
        )

file(GLOB
        core_qrc
        *.qrc
        )

file(GLOB
        Headers
        *.h)

file(GLOB
        core_src
        ./*.cpp
        )

add_library(core
        STATIC
        ${Headers}
        ${core_src}
        ${core_qrc}
        )

target_link_libraries(core PUBLIC
        zlibstatic
        lua
        lpeg
        scintilla
        scintillua
        lexilla
        Qt::Widgets
        Qt::Network
        Qt::Xml
        Qt::Core5Compat
        )

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})