cmake_minimum_required(VERSION 3.15)
project(CodeInspector VERSION 1.0.0)

set(CMAKE_EXPORT_COMPILECOMMANDS ON)

if(NOT DEFINED CMAKE_OSX_ARCHITECTURES)
  set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    set(BUILD_TYPE Debug)
    add_compile_definitions(LOGS_ENABLED)
ELSE(CMAKE_BUILD_TYPE MATCHES Debug)
    set(BUILD_TYPE Release)
ENDIF(CMAKE_BUILD_TYPE MATCHES Debug)

set(CURRENT_COMPILER_ID "${CMAKE_CXX_COMPILER_ID}")
message("Compiler ID is: ${CURRENT_COMPILER_ID}")

set(CURRENT_SIMULATE_ID "${CMAKE_CXX_SIMULATE_ID}")
message("Simulate ID is: ${CURRENT_SIMULATE_ID}")

if(WIN32)
    add_compile_definitions(STRSAFE_NO_DEPRECATE UNICODE)
endif()

if (MSVC)
    if("${CURRENT_COMPILER_ID}" STREQUAL "IntelLLVM")
        add_compile_options(/EHsc)
    else()
        add_compile_options(/utf-8)
    endif()
endif()

add_subdirectory(3rdparty)
add_subdirectory(core)
add_subdirectory(plugins)
add_subdirectory(desktop-qt)
