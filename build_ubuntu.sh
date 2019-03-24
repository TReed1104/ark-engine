#!/bin/sh
# Create the build directory
mkdir build
# Move into the build directory
cd build
# Tell conan to build the libraries in our conanfile.txt
conan install .. --build missing
# Tell cmake to make our build system
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release     # I think this is right?
# Tell cmake to actually build the project using our systems preferred compiler
cmake --build . --config Release
# Return to the parent directory (our repo head)
cd ..
