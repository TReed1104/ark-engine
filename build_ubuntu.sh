#!/bin/sh
mkdir build
cd build
conan install .. --build missing
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release     # I think this is right?
cmake --build . --config Release
cd ..
