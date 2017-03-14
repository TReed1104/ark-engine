project(ArkEngine)
cmake_minimum_required(VERSION 3.0)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
CONAN_BASIC_SETUP()

file(GLOB ArkEngineSrc "Source/*.c" "Source/*.h" "Source/*.cpp" "Source/*.hpp")

ADD_EXECUTABLE(ArkEngine ${ArkEngineSrc})
TARGET_LINK_LIBRARIES(ArkEngine ${CONAN_LIBS})

set (CMAKE_CXX_STANDARD 11)
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "ArkEngine")

FILE(COPY Content/ DESTINATION bin/Content)