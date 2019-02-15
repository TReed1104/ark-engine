# ark-engine
ark-engine is an open source customisable c++ game engine.

# Introductiom
The engines itself is written entirely in c++ and uses cmake and conan for its build system.

Customisation of the engine is done via Json config files within the content directory. These are loaded and edited using my "cpp-json-parser" api also found on my github profile.

# Dependencies
The following are the libraries used within the engine, these are almost entirely compiled and built via the conan package manager.
- SDL2 - window and event management
- SDL2_image - texture loading
- GLEW - extension of the OpenGl API
- GLM - maths library used for OpenGL
- assimp - model object loading
- rapidjson - used at the core of my "cpp-json-parser" api
- freetype - used for loading and configuring true type fonts (.ttf)

