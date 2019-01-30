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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/mesh.h>
#include <assimp/material.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

// Engine Source
#include "JsonParser.hpp"
#include "Shader.h"
#include "Keyboard.h"
#include "GameController.h"
#include "Texture.h"
#include "Model.h"
#include "Font.h"
#include "TextObject.h"
#include "GameObject.h"
#include "Camera.h"
#include "Tile.h"
#include "Tileset.h"
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
	const float physicsInterval = 0.1f;

	// Input Interfaces
	Keyboard* deviceKeyboard;
	std::vector<GameController*> deviceGameControllerRegister;

	// Text Rendering
	FT_Library freeTypeLibrary;

	// Cameras
	Camera* mainCamera;
	GameObject* mainCameraFocus;

	// Registers
	std::vector<Shader*> shaderRegister;
	std::vector<Model> modelRegister;
	std::vector<Texture> textureRegister;
	std::vector<Font*> fontRegister;
	std::vector<TextObject*> textObjectRegister;
	std::vector<Tileset*> tilesetRegister;
	std::vector<GameObject*> itemRegister;
	std::vector<Entity*> entityRegister;
	std::vector<Level*> levelRegister;
	Player* player;

	// Indexers
	int indexOfPlayerController;
	int indexOfDefaultShader;
	int indexOfCurrentLevel;
	int indexOfTileModel;
	int indexOfSpriteModel;
	int indexOfDefaultTexture;

	// Default Content names
	std::string nameOfDefaultTileModel;
	std::string nameOfDefaultSpriteModel;
	std::string nameOfDefaultTexture;

	// Keybinding stores
	Keyboard::Keys keybindMovementLeft;
	Keyboard::Keys keybindMovementRight;
	Keyboard::Keys keybindMovementUp;
	Keyboard::Keys keybindMovementDown;
	Keyboard::Keys keybindMovementSprint;
	Keyboard::Keys keybindActionOne;
	Keyboard::Keys keybindActionTwo;
	Keyboard::Keys keybindAttackThree;
	Keyboard::Keys keybindAttackFour;

	// Texture frame sizes
	glm::ivec2 textureBorderSize;
	glm::ivec2 tileTextureFrameSize;
	glm::ivec2 entityTextureFrameSize;

	// Constructors
	Engine(char* gameName);
	~Engine(void);

	// The core run function of the Engine, this is the main callable function of the Engine.
	void Run(void);

	// Engine Utilities
	glm::ivec2 ConvertToGridPosition(const glm::vec2& position);
	const int GetIndexOfShader(const std::string& shaderName);
	const int GetIndexOfModel(const std::string& modelName);
	const int GetIndexOfTexture(const std::string& textureName);
	const int GetIndexOfFont(const std::string & fontName);
	const int GetIndexOfRenderableText(const std::string & renderableTextName);
	const int GetIndexOfTileset(const std::string& tilesetName);
	const int GetIndexOfEntity(const std::string& entityName);
	const int GetIndexOfLevel(const std::string& levelName);
	Level* GetCurrentLevel(void);

private:
	// GL variables
	SDL_Window* sdlWindow;
	SDL_GLContext glContext;
	bool isRunning;
	JsonFile* configFile;

	// DeltaTime math
	float oldFrameTime;
	float currentFrameTime;

	// Engine config related functions
	void LoadEngineConfig(void);
	void LoadKeyBindings(void);
	void LoadEnginePointers(void);

	// OpenGL and SDL related functions
	void LoadSDL(void);
	void CreateSDLWindow(void);
	void CreateSDLContext(void);
	void LoadInputDevices(void);
	void LoadGLEW(void);
	void LoadFreeType(void);
	void LoadShaders(void);
	void LoadExternalLibraries(void);
	
	// Cleanup
	void CleanUp(void);
	void Close(bool isClean = false);
	
	// Content loading related functions
	void LoadFonts(void);
	void LoadTextObjects(void);
	void LoadTextures(void);
	void LoadModels(void);
	void LoadTilesets(void);
	void LoadLevels(void);
	void LoadItems(void);
	void LoadPlayer(void);
	void LoadEntities(void);
	void LoadCameras(void);
	void LoadEngine(void);

	// Game loop related functions
	void EventHandler(void);
	void Update(const float& deltaTime);
	void Render(void);

	// Window Control functions
	void WindowResize(const glm::vec2& newScaler);
	void WindowRename(const std::string& newName);

};
#endif
