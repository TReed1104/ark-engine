project(ArkEngine)
cmake_minimum_required(VERSION 3.0)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
CONAN_BASIC_SETUP()

file(GLOB header_files "src/*.h" "src/*.hpp")
file(GLOB src_files "src/*.c" "src/*.cpp")

ADD_EXECUTABLE(ArkEngine ${header_files} ${src_files})
TARGET_LINK_LIBRARIES(ArkEngine ${CONAN_LIBS})

set (CMAKE_CXX_STANDARD 11)
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "ArkEngine")

FILE(COPY Content/ DESTINATION bin/Content)