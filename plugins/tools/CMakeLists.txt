cmake_minimum_required(VERSION 3.15)
project(CodeInspector)

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

add_subdirectory(clang_format)
add_subdirectory(clang_query)
add_subdirectory(clangtidy)
add_subdirectory(include_what_you_use)
add_subdirectory(ldd)
add_subdirectory(llvmmca)
add_subdirectory(osaca)
add_subdirectory(pahole)
add_subdirectory(psv_studio)
add_subdirectory(readelf)
add_subdirectory(strings)
add_subdirectory(x86to6502)