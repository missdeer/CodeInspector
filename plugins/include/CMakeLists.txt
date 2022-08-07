cmake_minimum_required(VERSION 3.15)
project(plugins VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(plugins INTERFACE)

target_include_directories(plugins INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})