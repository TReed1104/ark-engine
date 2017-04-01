#ifndef ARKENGINE_ENGINE_H_
#define ARKENGINE_ENGINE_H_

// C++ libraries
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
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
#include "Keyboard.h"
#include "GameController.h"
#include "BoundingBox.h"
#include "Camera.h"
#include "GameObject.h"
#include "Tileset.h"
#include "Tile.h"
#include "Level.h"
#include "Entity.h"
#include "Player.h"
#include "LuaScript.h"

class Engine {
public:
	// Core Engine Variables
	std::string contentDirectory;
	glm::vec2 tileSize;
	glm::vec2 windowGridSize;
	glm::vec2 windowScaler;
	int maxNumberOfControllers;
	int thumbStickDeadZone;
	int triggerDeadZone;
	int pressedStateFlag;


	// Input Interfaces
	Keyboard* deviceKeyboard;
	std::vector<GameController*> deviceGameControllerRegister;

	// Shader Locations
	ShaderPointers shaderPointers;

	// Cameras
	Camera camera;

	// Registers
	std::vector<Model> modelRegister;
	std::vector<Tileset> tilesetRegister;
	std::vector<GameObject*> itemRegister;
	std::vector<Entity*> entityRegister;
	std::vector<Level*> levelRegister;
	std::vector<Texture> textureRegister;
	Player* player;

	// Indexers
	int indexOfPlayerController;
	int indexCurrentLevel;
	int indexOfDefaultTexture;

	// Keybinding stores
	Keyboard::Keys keybindMovementUp;
	Keyboard::Keys keybindMovementDown;
	Keyboard::Keys keybindMovementLeft;
	Keyboard::Keys keybindMovementRight;

	// Constructors
	Engine(char* gameName);
	~Engine(void);

	// Functions
	void Run(void);
	glm::vec2 ConvertToGridPosition(const glm::vec2& position);

private:
	// GL variables
	SDL_Window* sdlWindow;
	SDL_GLContext glContext;
	GLuint glProgram;
	bool isRunning;

	// Program variables
	std::string exeName;
	glm::vec2 windowDimensions;
	float oldFrameTime;
	float currentFrameTime;

	// Engine config related functions
	void LoadEngineConfig();
	void LoadKeyBindings();
	void LoadEnginePointers(void);
	// OpenGL and SDL related functions
	void InitialiseSDL(void);
	void CreateSDLWindow(void);
	void CreateSDLContext(void);
	void CheckForInputDevices(void);
	void InitialiseGlew(void);
	GLuint CreateGLProgram(const std::vector<Shader>& shaderList);
	void InitialiseProgram(void);
	void LoadGraphicsEnvironment(void);
	void CleanUp(void);
	// Content loading related functions
	void ImportTexture(const char* texturePath);
	void LoadTextures(void);
	Model LoadModel(const std::string& modelPath);
	void LoadModels(void);
	void LoadTilesets(void);
	void LoadLevels(void);
	void LoadItems(void);
	void LoadPlayer(void);
	void LoadEntities(void);
	void LoadCameras(void);
	void Load(void);
	// Game loop related functions
	void EventHandler(void);
	void Update(const float& deltaTime);
	void Draw(void);
	void Renderer(void);
};
#endif