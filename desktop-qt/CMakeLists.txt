cmake_minimum_required(VERSION 3.15)
project(CodeInspector VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network Xml LinguistTools)
if(Qt6_FOUND)
        find_package(Qt6 COMPONENTS Core5Compat REQUIRED)
endif()

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

IF(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
        qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/translations/CodeInspector_zh_CN.ts)
        qt_add_executable(${PROJECT_NAME}
                MANUAL_FINALIZATION
                ./CodeInspector.rc
                ./CodeInspector.icns
                ${desktop_headers}
                ${desktop_src}
                ${desktop_qrc}
                ${desktop_ui}
                ${QM_FILES}
                )
ELSE()
        qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/translations/CodeInspector_zh_CN.ts) 
        add_executable(${PROJECT_NAME}
                ./CodeInspector.rc
                ./CodeInspector.icns
                ${desktop_headers}
                ${desktop_src}
                ${desktop_qrc}
                ${desktop_ui}
                ${QM_FILES}
                )
ENDIF()

set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE ON
        MACOSX_BUNDLE ON
        )

if (APPLE)
    set_source_files_properties("${CMAKE_CURRENT_SOURCE_DIR}/CodeInspector.icns" PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set_target_properties(${PROJECT_NAME} PROPERTIES
            RESOURCE "./CodeInspector.icns"
            MACOSX_BUNDLE_BUNDLE_NAME CodeInspector
            MACOSX_BUNDLE_GUI_IDENTIFIER "com.ismisv.codeinspector"
            MACOSX_BUNDLE_ICON_FILE "CodeInspector.icns"
            MACOSX_BUNDLE_INFO_STRING "Code Inspector"
    )
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt::Widgets Qt::Network Qt::Xml
        core
        ast cfg gcctreertl ir optimization
        rextester wandbox
        clang_format clang_query clangtidy include_what_you_use ldd llvmmca osaca pahole psv_studio readelf strings x86to6502)

if(Qt6_FOUND)
        target_link_libraries(${PROJECT_NAME} PUBLIC
                Qt::Core5Compat
                )
endif()

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()
