cmake_minimum_required(VERSION 3.15)
project(ast VERSION 1.0.0)

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


file(GLOB
        headers
        ./*.h
        )

file(GLOB
        src
        ./*.cpp
        )

qt_add_plugin(ast STATIC CLASS_NAME AstPlugin)

target_sources(ast PRIVATE ${src} ${headers})

target_link_libraries(ast PRIVATE core plugins scintilla Qt::Widgets)

target_include_directories(ast PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})