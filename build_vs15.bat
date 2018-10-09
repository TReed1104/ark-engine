@echo off
mkdir build
cd build
conan install .. -s build_type=Debug -s compiler="Visual Studio" -s compiler.runtime=MDd --build=missing
cmake .. -G "Visual Studio 14 2015 Win64"
cmake --build . --config Debug
cd ..