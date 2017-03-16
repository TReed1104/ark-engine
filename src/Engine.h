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
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <assimp/mesh.h>
#include <assimp/material.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
// Engine Source
#include "ShaderPointers.hpp"
#include "Shader.h"
#include "Camera.h"
#include "GameObject.h"
#include "Tile.h"
#include "Level.h"
#include "Entity.h"
#include "Player.h"
#include "LuaScript.h"

class Engine {
public:
	// Core Engine Variables
	std::string contentDirectory = "content/";
	glm::vec2 tileSize;
	glm::vec2 windowGridSize;
	glm::vec2 windowScaler;

	// Shader Locations
	ShaderPointers shaderPointers;

	// Cameras
	Camera camera;

	// Registers
	std::vector<Model> modelRegister;
	std::vector<Tile*> tileRegister;
	std::vector<GameObject*> itemRegister;
	std::vector<Entity*> entityRegister;
	std::vector<Level*> levelRegister;
	std::vector<Texture> textureRegister;
	Player* player;

	// Indexers
	int indexCurrentLevel = 0;
	int indexOfDefaultTexture = -1;

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
	glm::vec2 windowDimensions;
	float oldFrameTime;
	float currentFrameTime;

	// Functions
	void LoadEngineConfig();
	void InitialiseSDL(void);
	void CreateSDLWindow(void);
	void CreateSDLContext(void);
	void InitialiseGlew(void);
	GLuint CreateGLProgram(const std::vector<Shader>& shaderList);
	void InitialiseProgram(void);
	Model LoadModel(const std::string& modelPath);
	void ImportTexture(const char* texturePath);
	void LoadTextures(void);
	void LoadModels(void);
	void LoadTiles(void);
	void LoadLevels(void);
	void LoadItems(void);
	void LoadPlayer(void);
	void LoadEntities(void);
	void LoadContent(void);
	void InitialiseEngine(void);
	void CleanupSDL(void);
	void EventHandler(void);
	void Event_Window(const SDL_WindowEvent& windowEvent);
	void Event_KeyDown(const SDL_KeyboardEvent& keyboardEvent);
	void Event_KeyUp(const SDL_KeyboardEvent& keyboardEvent);
	void Update(float deltaTime);
	void Draw(void);
	void Renderer(void);
};
#endif