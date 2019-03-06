#ifndef ARKENGINE_ENGINE_H_
#define ARKENGINE_ENGINE_H_

// C++ libraries
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
// OpenAL
#include <AL/al.h>
#include <AL/alc.h>

// Engine Source
#include "Debug.hpp"
#include "JsonParser.hpp"
#include "Shader.h"
#include "Keyboard.h"
#include "Keybindings.h"
#include "GameController.h"
#include "Texture.h"
#include "Model.h"
#include "Font.h"
#include "TextObject.h"
#include "SoundEffect.h"
#include "GameObject.h"
#include "Camera.h"
#include "Tile.h"
#include "Tileset.h"
#include "Item.h"
#include "Level.h"
#include "Entity.h"
#include "NonPlayerCharacter.h"
#include "Player.h"

class Engine {
public:
	// Debuggers
	Debug::Debugger engineDebugger;

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

	// OpenAL
	ALCdevice* audioDevice;
	ALCcontext* audioContext;


	// Cameras
	Camera* mainCamera;
	GameObject* mainCameraFocus;

	// Registers
	std::vector<Shader*> shaderRegister;
	std::vector<Model> modelRegister;
	std::vector<Texture> textureRegister;
	std::vector<Font*> fontRegister;
	std::vector<TextObject*> textObjectRegister;
	std::vector<SoundEffect*> soundRegister;
	std::vector<Tileset*> tilesetRegister;
	std::vector<Item*> itemRegister;
	std::vector<NonPlayerCharacter*> entityRegister;
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

	// Keybinding store
	KeybindingHandler keybindingHandler;

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
	const int GetIndexOfFont(const std::string& fontName);
	const int GetIndexOfRenderableText(const std::string& renderableTextName);
	const int GetIndexOfSoundEffect(const std::string& soundEffectName);
	const int GetIndexOfTileset(const std::string& tilesetName);
	const int GetIndexOfEntity(const std::string& entityName);
	const int GetIndexOfLevel(const std::string& levelName);
	Level* GetCurrentLevel(void);

	// Engine Settings
	void WindowResize(const glm::vec2& newScaler);
	void WindowRename(const std::string& newName);
	void SetVSyncState(const bool& newState);

private:
	// GL variables
	SDL_Window* sdlWindow;
	SDL_GLContext glContext;
	bool isRunning;
	JsonFile* debugConfigurationFile;
	JsonFile* configFile;

	// DeltaTime math
	float oldFrameTime;
	float currentFrameTime;

	// Cleanup
	void CleanUp(void);
	void Close(bool isClean = false);

	// Core engine loading
	void ConfigureDebugging(void);
	void LoadEngineConfig(void);
	void LoadUserSettings(void);
	
	// External Library setup functions
	void LoadSDL(void);
	void LoadGLEW(void);
	void LoadFreeType(void);
	void LoadOpenAL(void);
	void LoadExternalLibraries(void);

	// Content loading related functions
	void LoadShaders(void);
	void LoadInputDevices(void);
	void LoadFonts(void);
	void LoadTextObjects(void);
	void LoadSoundEffects(void);
	void LoadTextures(void);
	void LoadModels(void);
	void LoadTilesets(void);
	void LoadLevels(void);
	void LoadItems(void);
	void LoadEntities(void);
	void LoadCameras(void);

	// Master load function
	void LoadEngine(void);

	// Game loop related functions
	void EventHandler(void);
	void Update(const float& deltaTime);
	void Render(void);


};
#endif
