#ifndef ARKENGINE_ENGINE_H_
#define ARKENGINE_ENGINE_H_

// C++ libraries
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
// Graphics dependencies
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/mesh.h>
#include <assimp/material.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
// Engine Source
#include "Shader.h"
#include "LuaScript.h"
#include "Keyboard.h"
#include "GameController.h"
#include "Texture.h"
#include "Model.h"
#include "RenderableText.h"
#include "Font.h"
#include "GameObject.h"
#include "Camera.h"
#include "Tileset.h"
#include "Tile.h"
#include "Level.h"
#include "Entity.h"
#include "Player.h"

class Engine {
public:
	// Core Engine Variables
	std::string contentDirectory;
	glm::vec2 tileSize;
	std::string defaultWindowTitle;
	std::string windowTitle;
	glm::vec2 windowDimensions;
	glm::vec2 windowGridSize;
	glm::vec2 windowScaler;
	int maxNumberOfControllers;
	int thumbStickDeadZone;
	int triggerDeadZone;
	int pressedStateFlag;

	// Input Interfaces
	Keyboard* deviceKeyboard;
	std::vector<GameController*> deviceGameControllerRegister;

	// Cameras
	Camera* mainCamera;
	GameObject* mainCameraFocus;

	// Registers
	std::vector<Shader*> shaderRegister;
	std::vector<Model> modelRegister;
	std::vector<Texture> textureRegister;
	std::vector<Font*> fontRegister;
	std::vector<RenderableText*> renderableTextRegister;
	std::vector<Tileset> tilesetRegister;
	std::vector<GameObject*> itemRegister;
	std::vector<Entity*> entityRegister;
	std::vector<Level*> levelRegister;
	Player* player;

	// Indexers
	int indexOfPlayerController;
	int indexOfDefaultShader;
	int indexCurrentLevel;

	std::string nameOfTileModel;
	std::string nameOfSpriteModel;
	std::string nameOfDefaultTexture;
	int indexOfTileModel;
	int indexOfSpriteModel;
	int indexOfDefaultTexture;

	// Keybinding stores
	Keyboard::Keys keybindMovementUp;
	Keyboard::Keys keybindMovementDown;
	Keyboard::Keys keybindMovementLeft;
	Keyboard::Keys keybindMovementRight;

	// Texture frame sizes
	glm::ivec2 textureBorderSize;
	glm::ivec2 tileTextureFrameSize;
	glm::ivec2 entityTextureFrameSize;

	// Constructors
	Engine(char* gameName);
	~Engine(void);

	// Functions
	void Run(void);
	glm::ivec2 ConvertToGridPosition(const glm::vec2& position);
	int GetIndexOfShader(const std::string& shaderName);
	int GetIndexOfModel(const std::string& modelName);
	int GetIndexOfTexture(const std::string& textureName);
	int GetIndexOfFont(const std::string& fontName);
	int GetIndexOfRenderableText(const std::string& renderableTextName);
	int GetIndexOfTileset(const std::string& tilesetName);
	int GetIndexOfEntity(const std::string& entityName);
	int GetIndexOfLevel(const std::string& levelName);

private:
	// GL variables
	SDL_Window* sdlWindow;
	SDL_GLContext glContext;
	bool isRunning;

	// DeltaTime math
	float oldFrameTime;
	float currentFrameTime;

	// Engine config related functions
	void LoadEngineConfig(void);
	void LoadKeyBindings(void);
	void LoadEnginePointers(void);
	// OpenGL and SDL related functions
	void InitialiseSDL(void);
	void InitialiseTTF(void);
	void CreateSDLWindow(void);
	void CreateSDLContext(void);
	void CheckForInputDevices(void);
	void InitialiseGlew(void);
	void LoadShaders(void);
	void LoadGraphicsEnvironment(void);
	void CleanUp(void);
	void Close(bool isClean = false);
	// Content loading related functions
	void LoadTextures(void);
	void LoadModels(void);
	void LoadFonts(void);
	void LoadRenderableText(void);
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
	void Render(void);
	// Window Control functions
	void WindowResize(const glm::vec2& newScaler);
	void WindowRename(const std::string& newName);

};
#endif
