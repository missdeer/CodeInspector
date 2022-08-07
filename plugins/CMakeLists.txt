cmake_minimum_required(VERSION 3.15)
project(CodeInspector VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(include)
add_subdirectory(options)
add_subdirectory(runners)
add_subdirectory(tools)