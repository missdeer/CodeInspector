cmake_minimum_required(VERSION 3.15)
project(x86to6502 VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network Xml LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network Xml LinguistTools)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON) # if you have any .ui files
set(CMAKE_AUTORCC ON) # if you have any .qrc files

# Add compiler flags for building executables (-fPIE)


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

qt_add_plugin(x86to6502 STATIC CLASS_NAME X86To6502Plugin)

target_sources(x86to6502 PRIVATE ${src} ${headers})

target_link_libraries(x86to6502 PRIVATE core plugins scintilla Qt::Widgets)

target_include_directories(x86to6502 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})