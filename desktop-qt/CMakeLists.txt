cmake_minimum_required(VERSION 3.15)
project(CodeInspector VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network Xml LinguistTools)
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
        desktop_qrc
        ./*.qrc
        )

file(GLOB
        desktop_ui
        ./*.ui
        )

set(desktop_headers
        ./codeeditorpane.h
        ./codeinspectorapp.h
        ./codeinspectorpane.h
        ./codeinspectortabwidget.h
        ./comboboxdelegate.h
        ./compilationtabwidget.h
        ./mainwindow.h
        ./sessionwidget.h
        )

set(desktop_src
        ./main.cpp
        ./mainwindow.cpp
        ./comboboxdelegate.cpp
        ./codeeditorpane.cpp
        ./codeinspectortabwidget.cpp
        ./codeinspectorpane.cpp
        ./compilationtabwidget.cpp
        ./sessionwidget.cpp
        ./codeinspectorapp.cpp
        )

add_executable(CodeInspector
        WIN32
        MACOSX_BUNDLE
        ./CodeInspector.rc
        ./CodeInspector.icns
        ${desktop_headers}
        ${desktop_src}
        ${desktop_qrc}
        ${desktop_ui}
        )

if (APPLE)
    set(MACOSX_BUNDLE true)
    set(MACOSX_BUNDLE_BUNDLE_NAME CodeInspector)
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "info.minidump.codeinspector")
    set(MACOSX_BUNDLE_ICON_FILE "CodeInspector.icns")
    set(MACOSX_BUNDLE_INFO_STRING "Code Inspector")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING "1.0.0.1")
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0")
    set(MACOSX_BUNDLE_BUNDLE_VERSION "1.0")

    set_source_files_properties("${CMAKE_CURRENT_SOURCE_DIR}/CodeInspector.icns" PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set_target_properties(
            CodeInspector
            PROPERTIES
            RESOURCE "./CodeInspector.icns"
            WIN32_EXECUTABLE TRUE
            MACOSX_BUNDLE TRUE
    )
    target_link_libraries(CodeInspector PRIVATE z)
endif()

target_link_libraries(CodeInspector PRIVATE Qt::Widgets Qt::Network Qt::Xml
        core
        ast cfg gcctreertl ir optimization
        rextester wandbox
        clang_format clang_query clangtidy include_what_you_use ldd llvmmca osaca pahole psv_studio readelf strings x86to6502)
