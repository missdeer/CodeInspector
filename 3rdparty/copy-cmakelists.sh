#!/bin/sh
cp cmakes/CMakeLists.txt.lexilla lexilla/
cp cmakes/CMakeLists.txt.lua lua/
cp cmakes/CMakeLists.txt.lpeg lpeg/
cp cmakes/CMakeLists.txt.scintilla scintilla/
cp cmakes/CMakeLists.txt.scintillua scintillua/
cd zlib
git apply ../cmakes/zlib.patch
