#ifndef Engine_h_included
#define Engine_h_included

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <vector>
#include "GL\glew.h"
#include "SDL.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "Shader.h"


// Useful constants to the Engine.
const float PI = 3.14f;


class Engine {
public:
	// Camera
	// Texture Register
	// Level Register
	// Object Register
	// Player
	// Agent Register
	SDL_Window *sdlWindow;

	Engine(char* gameName, glm::vec2 initialWindowSize);
	~Engine(void);

	void Run(void);

private:
	std::string exeName;
	bool isRunning = true;
	SDL_GLContext glContext;
	GLuint glProgram;
	GLint vertexPositionAttrib;
	GLint colourAttrib;
	GLint uvAttrib;
	GLint hasTextureUniform;
	GLint textureSamplerUniform;
	GLint modelMatrixUniform;
	GLint viewMatrixUniform;
	GLint projectionMatrixUniform;
	float aspectRatio;
	glm::vec2 windowSize;
	float fieldOfView;
	float oldFrameTime;
	float currentFrameTime;


	void InitialiseSDL(void);
	void CreateSDLWindow(void);
	void CreateSDLContext(void);
	void InitialiseGlew(void);
	GLuint CreateGLProgram(const std::vector<Shader> &shaderList);
	void InitialiseProgram(void);
	void LoadContect(void);
	void SetupEnvironment(void);
	void CleanupSDL(void);

};


#endif