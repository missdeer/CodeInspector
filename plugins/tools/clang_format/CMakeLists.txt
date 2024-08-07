cmake_minimum_required(VERSION 3.15)
project(clang_format VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network Xml LinguistTools)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON) # if you have any .ui files
set(CMAKE_AUTORCC ON) # if you have any .qrc files

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_BINARY_DIR})

add_compile_definitions(SCINTILLA_QT=1
        SCI_LEXER=1
        _CRT_SECURE_NO_DEPRECATE=1
        SCI_STATIC_LINK=1
        EXPORT_IMPORT_API=
        )

file(GLOB
        headers
        ./*.h
        )

file(GLOB
        src
        ./*.cpp
        )



IF(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
        qt_add_plugin(${PROJECT_NAME} STATIC CLASS_NAME ClangFormatPlugin)
ELSE()
        add_library(${PROJECT_NAME} STATIC)
        target_compile_definitions(${PROJECT_NAME} PUBLIC  QT_STATICPLUGIN)
ENDIF()

target_sources(${PROJECT_NAME} PRIVATE ${src} ${headers})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE core plugins scintilla Qt::Widgets)
