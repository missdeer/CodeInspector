cmake_minimum_required(VERSION 3.15)
project(lpeg VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB
        lpeg_src
        ./*.c
        )

add_library(${PROJECT_NAME} STATIC ${lpeg_src})

target_link_libraries(${PROJECT_NAME} PRIVATE lua)

target_include_directories (${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
