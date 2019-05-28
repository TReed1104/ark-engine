# Ark-Engine

## The Aim
The aim of Ark-Engine was to create a customisable multiplatform game engine, to be used for my personal game development projects.

The engines itself is written entirely in c++, using cmake and conan as a build system.

The Customisation of the engine is done via Json config files within the content directory. These are loaded and edited using my "cpp-json-parser" api also found on my github profile ([here](https://github.com/TReed1104/cpp-json-parser)).

<br/>

---

## Engine Dependencies (Libraries)
The following are the libraries used within the engine, these are almost entirely compiled and built via the conan package manager.
* SDL2 - window and event management
* SDL2_image - texture loading
* GLEW - extension of the OpenGl API
* GLM - maths library used for OpenGL
* assimp - model object loading
* rapidjson - used at the core of my "cpp-json-parser" api
* freetype - used for loading and configuring true type fonts (.ttf)
* OpenAL - used for rendering 3D sounds

<br/>

---

## Building The Engine
Currently Ark-Engine only has a stable build for Windows. However, the Linux build is being worked on, with MacOS to follow afterwords.

### Guide
1. Ensure CMake is installed on your system.
2. Ensure Conan.io is installed on your system (**1.15 onwards, a recent change to external packages made this required**).
3. Ensure that Bincrafters and Zimmerk are registered with Conan as repositories for recipes.
    * If not, run the commands in the gist file found [here](https://gist.github.com/TReed1104/14e9e9c4e0f07c4fdc9373cb287bdf6b)
4. Run the build script for your platform:
    * Windows 10 - build_vs17.bat
    * Linux - build_ubuntu.sh **(EXPERIMENTAL)**
    * MacOS - TO BE IMPLEMENTED
5. Find the compiled engine in the "build/bin" directory, or find the generated IDE solution in "build/"

<br/>

---

## TODO
UNDER CONSTRUCTION
