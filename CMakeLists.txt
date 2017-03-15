# Set the project name
project(ArkEngine)

# Set the CMake version
cmake_minimum_required(VERSION 3.0)

# Use Conan
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
CONAN_BASIC_SETUP()

# Create globs for the headers and source to catch all the files.
file(GLOB header_files "src/*.h" "src/*.hpp")
file(GLOB src_files "src/*.c" "src/*.cpp")

ADD_EXECUTABLE(ArkEngine ${header_files} ${src_files})
TARGET_LINK_LIBRARIES(ArkEngine ${CONAN_LIBS})

# Set the C++ version
set (CMAKE_CXX_STANDARD 11)
# Set the project for VS to use.
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "ArkEngine")

# Copy the content and shaders folders to the /build for VS to be able to find them.
FILE(COPY content/ DESTINATION content)
FILE(COPY shaders/ DESTINATION shaders)
# Copy the content and shaders folders to the /build/bin so the exe can find them.
FILE(COPY shaders/ DESTINATION bin/shaders)
FILE(COPY content/ DESTINATION bin/content)