cmake_minimum_required(VERSION 3.15)
project(CodeInspector VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets Core Gui Network Xml REQUIRED)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON) # if you have any .ui files
set(CMAKE_AUTORCC ON) # if you have any .qrc files

# Add compiler flags for building executables (-fPIE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

message(STATUS "CMAKE_PREFIX_PATH" ${CMAKE_PREFIX_PATH})

include_directories (${PROJECT_SOURCE_DIR})
include_directories (${PROJECT_BINARY_DIR})

add_compile_definitions(SCINTILLA_QT=1
        SCI_LEXER=1
        _CRT_SECURE_NO_DEPRECATE=1
        SCI_STATIC_LINK=1
        EXPORT_IMPORT_API=
        )

include_directories(../core
        ../3rdparty/scintilla/qt/ScintillaEdit
        ../3rdparty/scintilla/qt/ScintillaEditBase
        ../3rdparty/scintilla/include
        ../3rdparty/scintilla/src
        ../3rdparty/lexilla/lexlib
        ../3rdparty/lexilla/include
        )

file(GLOB
        desktop_qrc
        ./*.qrc
        )

qt5_add_resources(RESOURCES
        ${desktop_qrc}
        )

file(GLOB
        desktop_ui
        ./*.ui
        )

set(core_headers
        ../core/codeeditor.h
        ../core/codeinspector.h
        ../core/library.h
        ../core/libraryversion.h
        ../core/luavm.h
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

set(SOURCES
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

qt5_wrap_ui (UIS  ${desktop_ui})

qt5_wrap_cpp(SOURCES
        ${desktop_headers}
        )

add_executable(CodeInspector
        WIN32
        MACOSX_BUNDLE
        ./CodeInspector.rc
        ./CodeInspector.icns
        ${UIS}
        ${SOURCES}
        ${RESOURCES}
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
    )
    set_target_properties(CodeInspector PROPERTIES BUNDLE TRUE)
    target_link_libraries(CodeInspector z)
endif()

target_link_libraries(CodeInspector Qt::Widgets Qt::Gui Qt::Core Qt::Network Qt::Xml core
        ast cfg gcctreertl ir optimization
        rextester wandbox
        clang_format clang_query clangtidy include_what_you_use ldd llvmmca osaca pahole psv_studio readelf strings x86to6502)
