cmake_minimum_required(VERSION 3.15)
project(CodeInspector)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(ast)
add_subdirectory(cfg)
add_subdirectory(gcctreertl)
add_subdirectory(ir)
add_subdirectory(optimization)