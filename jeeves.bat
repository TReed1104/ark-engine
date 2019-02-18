@ECHO OFF
ECHO --------------------------------
ECHO Jeeves, the cache cleaner
ECHO --------------------------------

ECHO - Cleaning Build Folder
RD /s /q build

ECHO - Cleaning Conan Cache
conan remove -f Assimp/4.1.0
conan remove -f IrrXML/1.2
conan remove -f bzip2/1.0.6
conan remove -f freetype/2.9.0
conan remove -f glew/2.1.0
conan remove -f glm/0.9.8.5
conan remove -f libjpeg-turbo/1.5.2
conan remove -f libpng/1.6.34
conan remove -f libtiff/4.0.9
conan remove -f libwebp/1.0.0
conan remove -f nasm/2.13.01
conan remove -f rapidjson/1.1.0
conan remove -f sdl2/2.0.9
conan remove -f sdl2_image/2.0.4
conan remove -f zlib/1.2.11