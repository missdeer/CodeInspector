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
        desktop
        3rdparty/scintilla/qt/ScintillaEdit
        3rdparty/scintilla/qt/ScintillaEditBase
        3rdparty/scintilla/include
        3rdparty/scintilla/src
        3rdparty/scintilla/lexlib
        3rdparty/scintilla/include
        )

file(GLOB
        desktop_qrc
        desktop/*.qrc
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
        desktop/*.ui
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
        core/LLVMIRView.h
)

set(desktop_headers
        desktop/codeeditorpane.h
        desktop/codeinspectorapp.h
        desktop/codeinspectorpane.h
        desktop/codeinspectortabwidget.h
        desktop/comboboxdelegate.h
        desktop/compilationtabwidget.h
        desktop/mainwindow.h
        desktop/sessionwidget.h
        )

file(GLOB
        scintilla_src
        3rdparty/scintilla/src/*.cxx
        3rdparty/scintilla/lexlib/*.cxx
        3rdparty/scintilla/lexers/*.cxx
        )

set(SOURCES
        desktop/main.cpp
        desktop/mainwindow.cpp
        desktop/comboboxdelegate.cpp
        desktop/codeeditorpane.cpp
        desktop/codeinspectortabwidget.cpp
        desktop/codeinspectorpane.cpp
        desktop/compilationtabwidget.cpp
        desktop/sessionwidget.cpp
        desktop/codeinspectorapp.cpp
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
        core/LLVMIRView.cpp
        3rdparty/scintilla/qt/ScintillaEdit/ScintillaEdit.cpp
        3rdparty/scintilla/qt/ScintillaEdit/ScintillaDocument.cpp
        3rdparty/scintilla/qt/ScintillaEditBase/PlatQt.cpp
        3rdparty/scintilla/qt/ScintillaEditBase/ScintillaQt.cpp
        3rdparty/scintilla/qt/ScintillaEditBase/ScintillaEditBase.cpp
        ${scintilla_src}
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
        desktop/CodeInspector.rc
        ${UIS}
        ${SOURCES}
        ${RESOURCES}
        )

target_link_libraries(CodeInspector Qt5::Widgets Qt5::Gui Qt5::Core Qt5::Network Qt5::Xml)

set_target_properties(CodeInspector PROPERTIES BUNDLE TRUE)