cmake_minimum_required(VERSION 3.15)
project(CodeInspector)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt5 COMPONENTS Widgets Core Gui Network Xml REQUIRED)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON) # if you have any .ui files
set(CMAKE_AUTORCC ON) # if you have any .qrc files

# Add compiler flags for building executables (-fPIE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

add_compile_definitions(SCINTILLA_QT=1
        SCI_LEXER=1
        _CRT_SECURE_NO_DEPRECATE=1
        SCI_STATIC_LINK=1
        EXPORT_IMPORT_API=
        )

include_directories(core
        desktop-qt
        3rdparty/scintilla/qt/ScintillaEdit
        3rdparty/scintilla/qt/ScintillaEditBase
        3rdparty/scintilla/include
        3rdparty/scintilla/src
        3rdparty/scintilla/lexlib
        3rdparty/scintilla/include
        )

file(GLOB
        desktop_qrc
        desktop-qt/*.qrc
        )
file(GLOB
        core_qrc
        core/*.qrc
        )

qt5_add_resources(RESOURCES
        ${core_qrc}
        ${desktop_qrc}
        )

file(GLOB
        desktop_ui
        desktop-qt/*.ui
        )

set(core_headers
        core/astoutput.h
        core/clangqueryoutput.h
        core/clangtidyoutput.h
        core/codeeditor.h
        core/codeinspector.h
        core/gcctreertloutput.h
        core/godboltagent.h
        core/library.h
        core/libraryversion.h
        core/llvmmachinecodeanalyzeroutput.h
        core/networkreplyhelper.h
        core/optimizationoutput.h
        core/outputwindow.h
        core/paholeoutput.h
        core/readelfoutput.h
        core/x86to6502output.h
        core/llvmirview.h
        core/lddoutput.h 
        core/includewhatyouuseoutput.h 
)

set(desktop_headers
        desktop-qt/codeeditorpane.h
        desktop-qt/codeinspectorapp.h
        desktop-qt/codeinspectorpane.h
        desktop-qt/codeinspectortabwidget.h
        desktop-qt/comboboxdelegate.h
        desktop-qt/compilationtabwidget.h
        desktop-qt/mainwindow.h
        desktop-qt/sessionwidget.h
        )

if (WIN32)
include_directories(
        3rdparty/zlib
        )
file(GLOB
        zlib_src
        3rdparty/zlib/*.c
        )
else()
file(GLOB zlib_src)
endif()

file(GLOB
        scintilla_src
        3rdparty/scintilla/src/*.cxx
        3rdparty/scintilla/lexlib/*.cxx
        3rdparty/scintilla/lexers/*.cxx
        )

set(SOURCES
        desktop-qt/main.cpp
        desktop-qt/mainwindow.cpp
        desktop-qt/comboboxdelegate.cpp
        desktop-qt/codeeditorpane.cpp
        desktop-qt/codeinspectortabwidget.cpp
        desktop-qt/codeinspectorpane.cpp
        desktop-qt/compilationtabwidget.cpp
        desktop-qt/sessionwidget.cpp
        desktop-qt/codeinspectorapp.cpp
        core/clangqueryoutput.cpp
        core/readelfoutput.cpp
        core/scintillaconfig.cpp
        core/scopedguard.cpp
        core/settings.cpp
        core/codeeditor.cpp
        core/codeinspector.cpp
        core/godboltagent.cpp
        core/networkreplyhelper.cpp
        core/library.cpp
        core/libraryversion.cpp
        core/optimizationoutput.cpp
        core/astoutput.cpp
        core/gcctreertloutput.cpp
        core/llvmmachinecodeanalyzeroutput.cpp
        core/outputwindow.cpp
        core/paholeoutput.cpp
        core/clangtidyoutput.cpp
        core/x86to6502output.cpp
        core/llvmirview.cpp
        core/lddoutput.cpp
        core/includewhatyouuseoutput.cpp 
        3rdparty/scintilla/qt/ScintillaEdit/ScintillaEdit.cpp
        3rdparty/scintilla/qt/ScintillaEdit/ScintillaDocument.cpp
        3rdparty/scintilla/qt/ScintillaEditBase/PlatQt.cpp
        3rdparty/scintilla/qt/ScintillaEditBase/ScintillaQt.cpp
        3rdparty/scintilla/qt/ScintillaEditBase/ScintillaEditBase.cpp
        ${scintilla_src}
        ${zlib_src}
        )

qt5_wrap_ui (UIS  ${desktop_ui})

qt5_wrap_cpp(SOURCES
        3rdparty/scintilla/qt/ScintillaEdit/ScintillaEdit.h
        3rdparty/scintilla/qt/ScintillaEdit/ScintillaDocument.h
        3rdparty/scintilla/qt/ScintillaEditBase/ScintillaQt.h
        3rdparty/scintilla/qt/ScintillaEditBase/ScintillaEditBase.h
        ${core_headers}
        ${desktop_headers}
        )

add_executable(CodeInspector 
		WIN32
		MACOSX_BUNDLE
		desktop-qt/CodeInspector.rc
		desktop-qt/CodeInspector.icns
		${UIS}
		${SOURCES}
		${RESOURCES}
		)
		
if (APPLE)
    set(MACOSX_BUNDLE true)
    set(MACOSX_BUNDLE_BUNDLE_NAME CodeInspector)
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "info.minidump.codeinspector")
    set(MACOSX_BUNDLE_ICON_FILE "CodeInspector.icns")
    set(MACOSX_BUNDLE_INFO_STRING "Code Inspector")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING "1.0.0.1")
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0")
    set(MACOSX_BUNDLE_BUNDLE_VERSION "1.0")

    set_source_files_properties("${CMAKE_CURRENT_SOURCE_DIR}/desktop-qt/CodeInspector.icns" PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set_target_properties(
            CodeInspector
            PROPERTIES
            RESOURCE "desktop-qt/CodeInspector.icns"
    )
    set_target_properties(CodeInspector PROPERTIES BUNDLE TRUE)
    target_link_libraries(CodeInspector z)
endif()

target_link_libraries(CodeInspector Qt5::Widgets Qt5::Gui Qt5::Core Qt5::Network Qt5::Xml)
