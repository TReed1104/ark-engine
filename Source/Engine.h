#ifndef ARKENGINE_ENGINE_H_
#define ARKENGINE_ENGINE_H_

// C++ libraries
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <vector>
// Graphics dependencies
#include "GL/glew.h"
#include "SDL.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <assimp/mesh.h>
#include <assimp/material.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
// Engine Source
#include "EngineConfig.h"
#include "ShaderPointers.hpp"
#include "Shader.h"
#include "Camera.h"
#include "GameObject.h"
#include "Tile.h"
#include "Entity.h"
#include "Player.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

class Engine {
public:
	// Shader Locations
	ShaderPointers shaderPointers;
	
	// Cameras
	Camera camera;

	// Registers
	std::vector<GameObject*> levelRegister;
	std::vector<Model> modelRegister;
	std::vector<Tile*> tileRegister;
	std::vector<GameObject*> itemRegister;
	std::vector<Entity*> entityRegister;
	Player* player;

	// Constructors
	Engine(char* gameName);
	~Engine(void);

	// Functions
	void Run(void);

private:
	// GL variables
	SDL_Window* sdlWindow;
	SDL_GLContext glContext;
	GLuint glProgram;
	bool isRunning = true;

	// Program variables
	std::string exeName;
	float aspectRatio;
	glm::vec2 windowSize;
	float fieldOfView;
	float oldFrameTime;
	float currentFrameTime;

	// Functions
	void InitialiseSDL(void);
	void CreateSDLWindow(void);
	void CreateSDLContext(void);
	void InitialiseGlew(void);
	GLuint CreateGLProgram(const std::vector<Shader>& shaderList);
	void InitialiseProgram(void);
	Model LoadModel(const std::string& modelPath);
	void LoadLevels(void);
	void LoadContent(void);
	void SetupEnvironment(void);
	void CleanupSDL(void);
	void InitialiseWorldCamera(void);
	void EventHandler(void);
	void Event_Window(const SDL_WindowEvent& windowEvent);
	void Event_KeyDown(const SDL_KeyboardEvent& keyboardEvent);
	void Event_KeyUp(const SDL_KeyboardEvent& keyboardEvent);
	void Update(float deltaTime);
	void Draw(void);
	void Renderer(void);
	

};


#endif