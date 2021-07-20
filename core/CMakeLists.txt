cmake_minimum_required(VERSION 3.15)
project(core VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets Network Xml REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON) # if you have any .ui files
set(CMAKE_AUTORCC ON) # if you have any .qrc files

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_BINARY_DIR})

add_compile_definitions(SCINTILLA_QT=1
        SCI_LEXER=1
        _CRT_SECURE_NO_DEPRECATE=1
        SCI_STATIC_LINK=1
        NO_DLL=1
        EXPORT_IMPORT_API=
        )

include_directories(../desktop-qt
        ../3rdparty/scintilla/qt/ScintillaEdit
        ../3rdparty/scintilla/qt/ScintillaEditBase
        ../3rdparty/scintilla/include
        ../3rdparty/scintilla/src
        ../3rdparty/lexilla/lexlib
        ../3rdparty/lexilla/include
        ../3rdparty/scintillua
        ../3rdparty/lua
        ../3rdparty/lpeg
        ${Qt5Widgets_INCLUDE_DIRS}
        ${Qt5Network_INCLUDE_DIRS}
        ${Qt5Xml_INCLUDE_DIRS}
        )

file(GLOB
        core_qrc
        *.qrc
        )

qt5_add_resources(RESOURCES
        ${core_qrc}
        )

file(GLOB
        Headers
        *.h)

set(core_headers
        ${Headers}
        )

include_directories(
        ../3rdparty/zlib
)

file(GLOB
        zlib_src
        ../3rdparty/zlib/*.c
        )

file(GLOB
        scintilla_src
        ../3rdparty/scintilla/src/*.cxx
        ../3rdparty/scintillua/*.cxx
        ../3rdparty/lexilla/src/*.cxx
        ../3rdparty/lexilla/lexers/*.cxx
        ../3rdparty/lexilla/lexlib/*.cxx
        ../3rdparty/scintilla/qt/ScintillaEdit/*.cpp
        ../3rdparty/scintilla/qt/ScintillaEditBase/*.cpp
        )

file(GLOB
        core_src
        ./*.cpp
        )

set(lua_src
        ../3rdparty/lua/lapi.c
        ../3rdparty/lua/lcode.c
        ../3rdparty/lua/lctype.c
        ../3rdparty/lua/ldebug.c
        ../3rdparty/lua/ldo.c
        ../3rdparty/lua/ldump.c
        ../3rdparty/lua/lfunc.c
        ../3rdparty/lua/lgc.c
        ../3rdparty/lua/linit.c
        ../3rdparty/lua/llex.c
        ../3rdparty/lua/lmem.c
        ../3rdparty/lua/lobject.c
        ../3rdparty/lua/lopcodes.c
        ../3rdparty/lua/lparser.c
        ../3rdparty/lua/lstate.c
        ../3rdparty/lua/lstring.c
        ../3rdparty/lua/ltable.c
        ../3rdparty/lua/ltm.c
        ../3rdparty/lua/lundump.c
        ../3rdparty/lua/lvm.c
        ../3rdparty/lua/lzio.c
        ../3rdparty/lua/lauxlib.c
        ../3rdparty/lua/lbaselib.c
        ../3rdparty/lua/lbitlib.c
        ../3rdparty/lua/lcorolib.c
        ../3rdparty/lua/ldblib.c
        ../3rdparty/lua/liolib.c
        ../3rdparty/lua/lmathlib.c
        ../3rdparty/lua/loadlib.c
        ../3rdparty/lua/loslib.c
        ../3rdparty/lua/lstrlib.c
        ../3rdparty/lua/ltablib.c
        ../3rdparty/lua/lutf8lib.c
        )

file(GLOB
        lpeg_src
        ../3rdparty/lpeg/*.c
        )

set(SOURCES
        ${core_src}
        ${lua_src}
        ${lpeg_src}
        ${scintilla_src}
        ${zlib_src}
        )

qt5_wrap_cpp(SOURCES
        ../3rdparty/scintilla/qt/ScintillaEdit/ScintillaEdit.h
        ../3rdparty/scintilla/qt/ScintillaEdit/ScintillaDocument.h
        ../3rdparty/scintilla/qt/ScintillaEditBase/ScintillaQt.h
        ../3rdparty/scintilla/qt/ScintillaEditBase/ScintillaEditBase.h
        ${core_headers}
        )

add_library(core
        ${SOURCES}
        ${RESOURCES}
        )