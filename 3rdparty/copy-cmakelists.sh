#!/bin/sh
cp cmakes/CMakeLists.txt.lexilla lexilla/CMakeLists.txt
cp cmakes/CMakeLists.txt.lua lua/CMakeLists.txt
cp cmakes/CMakeLists.txt.lpeg lpeg/CMakeLists.txt
cp cmakes/CMakeLists.txt.scintilla scintilla/CMakeLists.txt
cp cmakes/CMakeLists.txt.scintillua scintillua/CMakeLists.txt
cd zlib
git apply ../cmakes/zlib.patch
