#include "Engine.h"

// Constructors
Engine::Engine(char* gameName) {
	engineDebugger = Debug::Debugger("Engine Debugger");
	debugConfigurationFile = nullptr;
	configFile = nullptr;
	mainCamera = nullptr;
	mainCameraFocus = nullptr;
	player = nullptr;
	deviceKeyboard = nullptr;

	windowTitle = gameName;
	defaultWindowTitle = gameName;
	oldFrameTime = 0.0f;
	currentFrameTime = 0.0f;

	contentDirectory = "content\\";

	indexOfDefaultShader = 0;
	indexOfCurrentLevel = -1;

	nameOfDefaultTileModel = "";
	nameOfDefaultSpriteModel = "";
	nameOfDefaultTexture = "";
	indexOfTileModel = -1;
	indexOfDefaultTexture = -1;
	indexOfSpriteModel = -1;

	// Set each of the classes static engine pointer to this engine instance
	Shader::Engine_Pointer = this;
	Texture::Engine_Pointer = this;
	Model::Engine_Pointer = this;
	Font::Engine_Pointer = this;
	TextObject::Engine_Pointer = this;
	SoundEffect::Engine_Pointer = this;
	Keyboard::Engine_Pointer = this;
	KeybindingHandler::Engine_Pointer = this;
	GameController::Engine_Pointer = this;
	BoundingBox::Engine_Pointer = this;
	Animation::Engine_Pointer = this;
	Animation::Frame::Engine_Pointer = this;
	GameObject::Engine_Pointer = this;
	Tileset::Engine_Pointer = this;
	Level::Engine_Pointer = this;
	Camera::Engine_Pointer = this;

	freeTypeLibrary = NULL;
}
Engine::~Engine(void) {

}

// Clean Up functions
void Engine::CleanUp(void) {
	// Clear the Debugging config file from memory
	if (debugConfigurationFile != nullptr) {
		delete debugConfigurationFile;
	}

	// Clear the config file from memory
	if (configFile != nullptr) {
		delete configFile;
	}

	// Delete all the levels.
	const size_t levelRegisterSize = levelRegister.size();
	for (size_t i = 0; i < levelRegisterSize; i++) {
		delete levelRegister[i];
	}

	// Delete all the Tileset
	const size_t tilesetRegisterSize = tilesetRegister.size();
	for (size_t i = 0; i < tilesetRegisterSize; i++) {
		delete tilesetRegister[i];
	}

	// Delete all the items loaded.
	const size_t itemRegisterSize = itemRegister.size();
	for (size_t i = 0; i < itemRegisterSize; i++) {
		delete itemRegister[i];
	}

	// Delete the camera
	if (mainCamera != nullptr) {
		delete mainCamera;
	}

	// Delete all the Entities.
	if (player != nullptr) {
		delete player;
	}
	const size_t entityRegisterSize = entityRegister.size();
	for (size_t i = 0; i < entityRegisterSize; i++) {
		delete entityRegister[i];
	}

	// Delete all the Input Devices.
	if (deviceKeyboard != nullptr) {
		delete deviceKeyboard;
	}
	const size_t gameControllerRegisterSize = deviceGameControllerRegister.size();
	for (size_t i = 0; i < gameControllerRegisterSize; i++) {
		delete deviceGameControllerRegister[i];
	}

	// Delete the loaded Renderable Texts
	const size_t renderableTextRegisterSize = textObjectRegister.size();
	for (size_t i = 0; i < renderableTextRegisterSize; i++) {
		delete textObjectRegister[i];
	}

	// Delete the loaded Fonts
	const size_t fontRegisterSize = fontRegister.size();
	for (size_t i = 0; i < fontRegisterSize; i++) {
		delete fontRegister[i];
	}

	// Delete the loaded Soundeffects
	const size_t soundRegisterSize = soundRegister.size();
	for (size_t i = 0; i < soundRegisterSize; i++) {
		delete soundRegister[i];
	}

	// Delete the loaded Shaders.
	const size_t shaderRegisterSize = shaderRegister.size();
	for (size_t i = 0; i < shaderRegisterSize; i++) {
		delete shaderRegister[i];
	}

	// Delete the game controllers
	if (SDL_NumJoysticks() > 0) {
		const size_t deviceGameControllerRegisterSize = deviceGameControllerRegister.size();
		for (size_t i = 0; i < deviceGameControllerRegisterSize; i++) {
			SDL_GameControllerClose(deviceGameControllerRegister[i]->GetSDLHook());	// Close the controller.
		}
	}

	// Cleanup OpenAL
	alcMakeContextCurrent(NULL);
	alcDestroyContext(audioContext);
	alcCloseDevice(audioDevice);

	// Clean up our extenal libraries
	if (freeTypeLibrary != NULL) {
		FT_Done_FreeType(freeTypeLibrary);
	}
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(sdlWindow);
}
void Engine::Close(bool isClean) {
	// Clear up and close the engine.
	if (isClean) {
		CleanUp();
		SDL_Quit();
	}
	else {
		CleanUp();
		SDL_Quit();
		engineDebugger.~Debugger();	// Explicit deconstructor call on the core debugger to force flush the file stream and get our log
		exit(1);
	}
}

// Core Engine Loading
void Engine::ConfigureDebugging(void) {
	debugConfigurationFile = new JsonFile(contentDirectory + "debug.json");
	if (debugConfigurationFile->IsLoaded()) {
		// Debugging setup
		engineDebugger.SetName(debugConfigurationFile->Get<std::string>("debug.core engine debug.id"));
		engineDebugger.SetDebuggingState(debugConfigurationFile->Get<bool>("debug.core engine debug.debugging enabled"));
		engineDebugger.SetLoggingState(debugConfigurationFile->Get<bool>("debug.core engine debug.logging enabled"));
		engineDebugger.SetLoggingFilePath(debugConfigurationFile->Get<std::string>("debug.core engine debug.log file"));

		// TODO: Load additional debuggers from the config
		size_t numberOfAdditionalDebuggers = debugConfigurationFile->SizeOfObjectArray("debug.additional debuggers");
		if (numberOfAdditionalDebuggers > 0) {
			// Load additional Debuggers
			engineDebugger.WriteLine(">>>> NOT IMPLEMENTED - Addition Debugger loading");
		}
	}
	else {
		// No debug config was found, disable the engine debugger
		engineDebugger.SetDebuggingState(false);
		engineDebugger.SetLoggingState(false);
		engineDebugger.SetLoggingFilePath("");
	}
}
void Engine::LoadEngineConfig(void) {
	engineDebugger.WriteLine(">> 1 - Loading Engine Configs");
	configFile = new JsonFile(contentDirectory + "engine.json");	// Load the config file into memory
	if (configFile->IsLoaded()) {
		// Window Engine setup
		windowTitle = configFile->Get<std::string>("engine.configuration.window.title");
		defaultWindowTitle = configFile->Get<std::string>("engine.configuration.window.title");
		tileSize = glm::vec2(configFile->Get<int>("engine.configuration.window.tile size.width"), configFile->Get<int>("engine.configuration.window.tile size.height"));
		windowGridSize = glm::vec2(configFile->Get<int>("engine.configuration.window.grid size.width"), configFile->Get<int>("engine.configuration.window.grid size.height"));
		windowScaler = glm::vec2(configFile->Get<int>("engine.configuration.window.scalar.x"), configFile->Get<int>("engine.configuration.window.scalar.y"));
		windowDimensions = (tileSize * windowGridSize) * windowScaler;

		// Texture Source Frame setup
		textureBorderSize = glm::ivec2(configFile->Get<int>("engine.configuration.content.texture config.texture border size.width"), configFile->Get<int>("engine.configuration.content.texture config.texture border size.height"));
		tileTextureFrameSize = glm::ivec2(configFile->Get<int>("engine.configuration.content.texture config.tile frame dimensions.width"), configFile->Get<int>("engine.configuration.content.texture config.tile frame dimensions.height"));
		entityTextureFrameSize = glm::ivec2(configFile->Get<int>("engine.configuration.content.texture config.sprite frame dimensions.width"), configFile->Get<int>("engine.configuration.content.texture config.sprite frame dimensions.height"));

		// Default content setup
		nameOfDefaultTileModel = configFile->Get<std::string>("engine.configuration.content.default content.tile");
		nameOfDefaultSpriteModel = configFile->Get<std::string>("engine.configuration.content.default content.sprite");
		nameOfDefaultTexture = configFile->Get<std::string>("engine.configuration.content.default content.texture");

		// Controller setup
		maxNumberOfControllers = configFile->Get<int>("engine.configuration.game controller.max number of controllers");
		indexOfPlayerController = configFile->Get<int>("engine.configuration.game controller.index of player");
		thumbStickDeadZone = configFile->Get<int>("engine.configuration.game controller.thumb stick dead zone");
		triggerDeadZone = configFile->Get<int>("engine.configuration.game controller.trigger dead zone");
		pressedStateFlag = configFile->Get<int>("engine.configuration.game controller.pressed state flag");

	}
	else {
		// Config failed to load.
		engineDebugger.WriteLine(">>>> ERROR!!!! - Engine config wasn't loaded");
		engineDebugger.WriteLine(">> 1 - FAILED");
		this->Close();
	}
	engineDebugger.WriteLine(">> 1 - COMPLETE");
}
void Engine::LoadUserSettings(void) {
	engineDebugger.WriteLine(">> 3 - Loading User Settings");

	if (configFile->IsLoaded()) {
		// Loading User settings
		SetVSyncState(configFile->Get<bool>("engine.user settings.vsync"));

		// Loading keybinds
		size_t numberOfKeybinds = configFile->SizeOfObjectArray("engine.user settings.key bindings");
		for (size_t i = 0; i < numberOfKeybinds; i++) {
			std::string nameOfBinding = configFile->Get<std::string>("engine.user settings.key bindings." + std::to_string(i) + ".binding.id");

			Keybind bind{
				configFile->Get<std::string>("engine.user settings.key bindings." + std::to_string(i) + ".binding.id"),
				configFile->Get<std::string>("engine.user settings.key bindings." + std::to_string(i) + ".binding.friendly name"),
				(Keyboard::Keys)configFile->Get<int>("engine.user settings.key bindings." + std::to_string(i) + ".binding.key id")
			};

			keybindingHandler.AddKeybinding(bind);
		}
	}
	else {
		// Config failed to load.
		engineDebugger.WriteLine(">>>> ERROR!!!! - Engine config wasn't loaded ");
		engineDebugger.WriteLine(">> 3 - FAILED");
		this->Close();
	}

	engineDebugger.WriteLine(">> 3 - COMPLETE");
}

// External Library Loading functions
void Engine::LoadSDL(void) {
	engineDebugger.WriteLine(">>>> 2.1 - Loading Library - SDL2");

	// Initialise the SDL2 Library
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		engineDebugger.WriteLine(">>>>>> ERROR!!!! - SDL_Init Error: " + (std::string)SDL_GetError());
		engineDebugger.WriteLine(">>>>>> 2.1 - FAILED");
		this->Close();
	}

	// SDL2 window creation
	sdlWindow = SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)windowDimensions.x, (int)windowDimensions.y, SDL_WINDOW_OPENGL);	// Create window
	// Error handling for the SDL Window.
	if (sdlWindow == nullptr) {
		engineDebugger.WriteLine(">>>>>> ERROR!!!! - SDL_CreateWindow Error: " + (std::string)SDL_GetError());
		engineDebugger.WriteLine(">>>>>> 2.1.2 - FAILED");
		this->Close();
	}

	// Sets up the OpenGL context for OpenGL version 3.3, TODO: Expand to allow for more recent versions of OpenGL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// OpenGL Context creation, this is where we actually link our SDL2 window to the OpenGL API
	glContext = SDL_GL_CreateContext(sdlWindow);
	// Error handling for failing to create the OpenGL Context
	if (glContext == nullptr) {
		SDL_DestroyWindow(sdlWindow);
		engineDebugger.WriteLine(">>>>>> ERROR!!!! - SDL_GL_CreateContext Error: " + (std::string)SDL_GetError());
		engineDebugger.WriteLine(">>>>>> 2.1.3 - FAILED");
		this->Close();
	}

	engineDebugger.WriteLine(">>>> 2.1 - COMPLETE");
}
void Engine::LoadGLEW(void) {
	engineDebugger.WriteLine(">>>> 2.2 - Loading Library - GLEW");

	GLenum rev;
	glewExperimental = GL_TRUE;
	rev = glewInit();
	if (GLEW_OK != rev) {
		// If GLEW fails, close the program.
		engineDebugger.WriteLine(">>>>>> ERROR!!!! - GLEW Error: " + *((const char*)glewGetErrorString(rev)));
		engineDebugger.WriteLine(">>>>>> 2.2 - FAILED");
		this->Close();
	}
	// Setup Core OpenGL
	glViewport(0, 0, (int)windowDimensions.x, (int)windowDimensions.y);
	SDL_GL_SwapWindow(sdlWindow);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);

	engineDebugger.WriteLine(">>>> 2.2 - COMPLETE");
}
void Engine::LoadFreeType(void) {
	engineDebugger.WriteLine(">>>> 2.3 - Loading Library - FreeType2");

	// Initialise the FreeType library, return is "are there errors, true = errors"
	if (FT_Init_FreeType(&freeTypeLibrary)) {
		engineDebugger.WriteLine(">>>>>> ERROR!!!! - FreeType Failed to Initialise");
		engineDebugger.WriteLine(">>>>>> 2.3 - FAILED");
		this->Close();
	}

	engineDebugger.WriteLine(">>>> 2.3 - COMPLETE");
}
void Engine::LoadOpenAL(void) {
	engineDebugger.WriteLine(">>>> 2.4 - Loading Library - OpenAL");

	// Get the list of available audio devices, this code is taken from an example online and editted to match the engines style
	const ALCchar* deviceList = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
	const ALCchar *currentDevice = deviceList;
	const ALCchar* nextDevice = deviceList + 1;
	size_t len = 0;
	// Audio devices are listed as a string with NULL characters seperating them
	engineDebugger.WriteLine(">>>>>> Available Audio Devices:");
	while (currentDevice && *currentDevice != '\0' && nextDevice && *nextDevice != '\0') {
		engineDebugger.WriteLine(">>>>>>>> Device: " + (std::string)currentDevice);
		len = strlen(currentDevice);
		currentDevice += (len + 1);
		nextDevice += (len + 2);
	}


	// TODO: Expand this to find out what devices are available and actually ensure we use the right one
	audioDevice = alcOpenDevice(NULL);	// Load the default device -> Should be system default?
	if (audioDevice) {
		engineDebugger.WriteLine(">>>>>> Audio Device: " + (std::string)alcGetString(audioDevice, ALC_DEVICE_SPECIFIER) + " - initialised");
		audioContext = alcCreateContext(audioDevice, NULL);	// Create the audio context for the engine to use
		
		if (audioContext) {
			engineDebugger.WriteLine(">>>>>> Audio context created");
			if (alcMakeContextCurrent(audioContext)) {
				// Success!
				engineDebugger.WriteLine(">>>>>> Audio context set");
				engineDebugger.WriteLine(">>>>>> SUCCESS! - OpenAL initialised");
			}
			else {
				engineDebugger.WriteLine(">>>>>> ERROR!!!! - Failed to set Audio context");
				engineDebugger.WriteLine(">>>>>> ERROR!!!! - OpenAL Error: " + alGetError());
				engineDebugger.WriteLine(">>>> 2.4 - FAILED");
				
				// Cleanup
				alcDestroyContext(audioContext);
				alcCloseDevice(audioDevice);
				return;
			}
		}
		else {
			engineDebugger.WriteLine(">>>>>> ERROR!!!! - Failed to create Audio context");
			engineDebugger.WriteLine(">>>>>> ERROR!!!! - OpenAL Error: " + alGetError());
			engineDebugger.WriteLine(">>>> 2.4 - FAILED");
			
			// Cleanup
			alcCloseDevice(audioDevice);
			return;
		}
	}
	else {
		engineDebugger.WriteLine(">>>>>> ERROR!!!! - Failed to Load Audio Device");
		engineDebugger.WriteLine(">>>>>> ERROR!!!! - OpenAL Error: " + alGetError());
		engineDebugger.WriteLine(">>>> 2.4 - FAILED");
		return;
	}

	engineDebugger.WriteLine(">>>> 2.4 - COMPLETE");
}
void Engine::LoadExternalLibraries(void) {
	engineDebugger.WriteLine(">> 2 - Loading External Libraries");

	// Call each fo the functions for loading each library
	LoadSDL();
	LoadGLEW();
	LoadFreeType();
	LoadOpenAL();

	engineDebugger.WriteLine(">> 2 - COMPLETE");
}

// Content Loading Functions
void Engine::LoadShaders(void) {
	engineDebugger.WriteLine(">> 4 - Loading Shaders");

	if (configFile->IsLoaded()) {
		size_t numberOfShaders = configFile->SizeOfObjectArray("engine.configuration.shaders");

		// If no shaders are found, close the program
		if (numberOfShaders == 0) {
			engineDebugger.WriteLine(">>>> ERROR!!!! - No shaders were found ABORTING RUNTIME");
			engineDebugger.WriteLine(">> 4 - FAILED");
			this->Close();
		}

		// Load each of the shaders
		for (size_t i = 0; i < numberOfShaders; i++) {
			std::string nameOfShader = contentDirectory + "shaders\\" + configFile->Get<std::string>("engine.configuration.shaders." + std::to_string(i) + ".shader.id");
			std::string vertexShaderName = contentDirectory + "shaders\\" + configFile->Get<std::string>("engine.configuration.shaders." + std::to_string(i) + ".shader.vertex");
			std::string fragmentShaderName = contentDirectory + "shaders\\" + configFile->Get<std::string>("engine.configuration.shaders." + std::to_string(i) + ".shader.fragment");

			// Compile the shader
			Shader* newShader = new Shader(nameOfShader, vertexShaderName, fragmentShaderName);
			if (newShader->IsLoaded()) {
				shaderRegister.push_back(newShader);	// Loaded successfully, storing it for use
			}
			else {
				engineDebugger.WriteLine(">>>> ERROR!!!! - Failed to compile Shader " + nameOfShader);
				engineDebugger.WriteLine(">> 4 - FAILED");
				this->Close();
			}
		}
	}
	else {
		engineDebugger.WriteLine(">>>> ERROR!!!! - Engine config wasn't loaded");
		engineDebugger.WriteLine(">> 4 - FAILED");
		this->Close();
	}

	engineDebugger.WriteLine(">> 4 - COMPLETE");
}
void Engine::LoadInputDevices(void) {
	engineDebugger.WriteLine(">> 5 - Loading Input Devices");

	// Initialise the keyboard instance
	engineDebugger.WriteLine(">>>> 5.1 - Keyboard Initialised...");
	deviceKeyboard = new Keyboard();

	// Search for any Keyboards.
	const int numberOfConnectedControllers = SDL_NumJoysticks();
	if (numberOfConnectedControllers < 1) {
		engineDebugger.WriteLine(">>>> 5.2 - No Controllers were found...");
	}
	else {
		for (int i = 0; i < numberOfConnectedControllers; i++) {
			if (i < maxNumberOfControllers) {
				deviceGameControllerRegister.push_back(new GameController(SDL_GameControllerOpen(i)));
				if (deviceGameControllerRegister[i]->GetSDLHook() != NULL) {
					engineDebugger.WriteLine(">>>> 5.2 - Game controller found: Controller has been opened for input");
				}
				else {
					engineDebugger.WriteLine(">>>> ERROR!!!! - Unable to Open game controller for use! SDL Error: " + (std::string)SDL_GetError());
				}
			}
		}

	}

	engineDebugger.WriteLine(">> 5 - COMPLETE");
}
void Engine::LoadFonts(void) {
	engineDebugger.WriteLine(">> 6 - Loading Fonts");

	if (configFile->IsLoaded()) {
		size_t numberOfFonts = configFile->SizeOfObjectArray("engine.configuration.content.fonts");
		for (size_t i = 0; i < numberOfFonts; i++) {
			std::string fontName = configFile->Get<std::string>("engine.configuration.content.fonts." + std::to_string(i) + ".font.id");
			std::string fontPath = contentDirectory + "fonts/" + configFile->Get<std::string>("engine.configuration.content.fonts." + std::to_string(i) + ".font.source");
			int fontSize = configFile->Get<int>("engine.configuration.content.fonts." + std::to_string(i) + ".font.size");
			Font* newFont = new Font(fontName, fontPath, fontSize);
			if (newFont->isLoaded) {
				fontRegister.push_back(newFont);
			}
			else {
				engineDebugger.WriteLine(">>>> ERROR!!!! - Failed to load Font: " + fontPath);
				engineDebugger.WriteLine(">> 6 - FAILED");
				this->Close();
			}
		}
	}
	else {
		// Config failed to load.
		engineDebugger.WriteLine(">>>> ERROR!!!! - Engine config wasn't loaded");
		engineDebugger.WriteLine(">> 6 - FAILED");
		this->Close();
	}

	engineDebugger.WriteLine(">> 6 - COMPLETE");
}
void Engine::LoadTextObjects(void) {
	engineDebugger.WriteLine(">> 7 - Loading Text Objects");
	const int indexOfFont = GetIndexOfFont("Arial");
	textObjectRegister.push_back(new TextObject("UI: Health", "Health: 100%", fontRegister[indexOfFont], glm::vec3(20.0f, 20.0f, 0.02f), glm::vec3(255 / 255.0f, 255 / 255.0f, 255 / 255.0f), true));
	textObjectRegister.push_back(new TextObject("DEBUG: FPS Counter", "FPS: 0", fontRegister[indexOfFont], glm::vec3(20.0f, 30.0f, 0.02f), glm::vec3(255 / 255.0f, 255 / 255.0f, 255 / 255.0f), true));
	textObjectRegister.push_back(new TextObject("DEBUG: Grid Position", "GridPos: (0, 0)", fontRegister[indexOfFont], glm::vec3(20.0f, 40.0f, 0.02f), glm::vec3(255 / 255.0f, 255 / 255.0f, 255 / 255.0f), true));
	engineDebugger.WriteLine(">> 7 - COMPLETE");
}
void Engine::LoadSoundEffects(void) {
	engineDebugger.WriteLine(">> 8 - Loading Audio");

	std::vector<std::string> listOfAudioFiles = FileSystemUtilities::GetFileList(contentDirectory + "sounds");
	const size_t numberOfAudioFiles = listOfAudioFiles.size();
	for (size_t i = 0; i < numberOfAudioFiles; i++) {
		SoundEffect* newSound = new SoundEffect("temp", listOfAudioFiles[i]);
		if (newSound->IsLoaded()) {
			soundRegister.push_back(newSound);
		}
		else {
			// Failed to load the item, runtime does continue
			delete newSound;
			engineDebugger.WriteLine(">>>> ERROR!!!! - Failed to load Audio File: " + listOfAudioFiles[i]);
			engineDebugger.WriteLine(">> 8 - FAILED");
		}
	}

	engineDebugger.WriteLine(">> 8 - COMPLETE");
}
void Engine::LoadTextures(void) {
	engineDebugger.WriteLine(">> 9 - Loading Textures");

	std::vector<std::string> listOfTextures = FileSystemUtilities::GetFileList(contentDirectory + "textures");
	const size_t numberOfTextues = listOfTextures.size();
	// If no textures are found, close the program

	if (numberOfTextues == 0) {
		engineDebugger.WriteLine(">>>> ERROR!!!! - Failed to find textures ABORTING RUNTIME");
		engineDebugger.WriteLine(">>>> 9 - FAILED");
		this->Close();
	}

	// Load the found textures
	for (size_t i = 0; i < numberOfTextues; i++) {
		Texture newTexture = Texture(listOfTextures[i], true, true);
		if (newTexture.IsLoaded()) {
			textureRegister.push_back(newTexture);
		}
		else {
			engineDebugger.WriteLine(">>>> ERROR!!!! - Failed to load texture" + listOfTextures[i]);
			engineDebugger.WriteLine(">>>> 9 - FAILED");
			this->Close();
		}
	}

	// Find the default texture for when textures are failed to be found.
	if ((indexOfDefaultTexture = GetIndexOfTexture(nameOfDefaultTexture)) == -1) {
		engineDebugger.WriteLine(">>>> ERROR!!!! - Unable to find default texture");
		engineDebugger.WriteLine(">>>> 9 - FAILED");
		this->Close();
	}

	engineDebugger.WriteLine(">> 9 - COMPLETE");
}
void Engine::LoadModels(void) {
	engineDebugger.WriteLine(">> 10 - Loading Models");

	std::vector<std::string> listOfModels = FileSystemUtilities::GetFileList(contentDirectory + "models");
	const size_t numberOfModels = listOfModels.size();
	// Check we found any model files to load, if not exit as these are required for the program to work
	if (numberOfModels == 0) {
		engineDebugger.WriteLine(">>>> ERROR!!!! - No to Model files found");
		engineDebugger.WriteLine(">>>> 10 - FAILED");
		this->Close();
	}

	// Load our model files
	for (size_t i = 0; i < numberOfModels; i++) {
		Model newModel = Model(listOfModels[i], true);
		if (newModel.IsLoaded()) {
			modelRegister.push_back(newModel);
		}
		else {
			engineDebugger.WriteLine(">>>> ERROR!!!! - Failed to load Model" + listOfModels[i]);
			engineDebugger.WriteLine(">>>> 10 - FAILED");
			this->Close();
		}
	}

	// Find the default models
	if ((indexOfTileModel = GetIndexOfModel(nameOfDefaultTileModel)) == -1) {
		engineDebugger.WriteLine(">>>> ERROR!!!! - Unable to find default model");
		engineDebugger.WriteLine(">>>> 10 - FAILED");
		this->Close();
	}
	// If we can't find the default sprite model, use the tile model
	if ((indexOfSpriteModel = GetIndexOfModel(nameOfDefaultSpriteModel)) == -1) {
		indexOfSpriteModel = indexOfTileModel;
	}

	engineDebugger.WriteLine(">> 10 - COMPLETE");
}
void Engine::LoadTilesets(void) {
	engineDebugger.WriteLine(">> 11 - Loading Tilesets");

	std::vector<std::string> listOfTilesets = FileSystemUtilities::GetFileList(contentDirectory + "tilesets");
	const size_t numberOfTilesets = listOfTilesets.size();

	// Make sure we have found atleast one tileset
	if (numberOfTilesets == 0) {
		engineDebugger.WriteLine(">>>> ERROR!!!! - No Tilesets found");
		engineDebugger.WriteLine(">> 11 - FAILED");
		this->Close();
	}

	// Load each of the tilesets found in the content directory
	for (size_t i = 0; i < numberOfTilesets; i++) {
		Tileset* newTileSet = new Tileset(listOfTilesets[i]);
		if (newTileSet->IsLoaded()) {
			tilesetRegister.push_back(newTileSet);
		}
		else {
			engineDebugger.WriteLine(">>>> ERROR!!!! - Failed to load Tileset " + listOfTilesets[i]);
			engineDebugger.WriteLine(">> 11 - FAILED");
			this->Close();
		}
	}

	engineDebugger.WriteLine(">> 11 - COMPLETE");
}
void Engine::LoadLevels(void) {
	engineDebugger.WriteLine(">> 12 - Loading Levels");

	std::vector<std::string> listOfLevelFiles = FileSystemUtilities::GetFileList(contentDirectory + "levels");
	const size_t numberOfLevels = listOfLevelFiles.size();
	// Ensure we found atleast one level to load
	if (numberOfLevels == 0) {
		engineDebugger.WriteLine(">>>> ERROR!!!! - Failed to find levels");
		engineDebugger.WriteLine(">> 12 - FAILED");
		this->Close();
	}

	// Load each of the levels found in the directory
	for (size_t i = 0; i < numberOfLevels; i++) {
		Level* newLevel = new Level(listOfLevelFiles[i]);
		if (newLevel->IsLoaded()) {
			levelRegister.push_back(newLevel);
		}
		else {
			engineDebugger.WriteLine(">>>> ERROR!!!! - Failed to load Level " + listOfLevelFiles[i]);
			engineDebugger.WriteLine(">> 12 - FAILED");
			this->Close();
		}
	}
	indexOfCurrentLevel = 0;

	engineDebugger.WriteLine(">> 12 - COMPLETE");
}
void Engine::LoadItems(void) {
	engineDebugger.WriteLine(">> 13 - Loading Items");
	std::vector<std::string> listOfItemFiles = FileSystemUtilities::GetFileList(contentDirectory + "items");
	const size_t numberOfItems = listOfItemFiles.size();
	for (size_t i = 0; i < numberOfItems; i++) {
		Item* newItem = new Item(listOfItemFiles[i]);
		if (newItem->IsLoaded()) {
			itemRegister.push_back(newItem);
		}
		else {
			// Failed to load the item, runtime does continue
			engineDebugger.WriteLine(">>>> ERROR!!!! - Failed to load Item " + listOfItemFiles[i]);
			engineDebugger.WriteLine(">> 13 - FAILED");
		}
	}

	engineDebugger.WriteLine(">> 13 - COMPLETE");
}
void Engine::LoadEntities(void) {
	engineDebugger.WriteLine(">> 14 - Loading Entities");

	std::vector<std::string> listOfEntityFiles = FileSystemUtilities::GetFileList(contentDirectory + "entities");
	const size_t numberOfEntities = listOfEntityFiles.size();
	// If we haven't found any entities, close the engine.
	if (numberOfEntities == 0) {
		engineDebugger.WriteLine(">>>> ERROR!!!! - No entity files were found in the content directory");
		engineDebugger.WriteLine(">> 14 - FAILED");
		this->Close();
	}

	// Go through each config in the entity directory
	for (size_t i = 0; i < numberOfEntities; i++) {
		if (listOfEntityFiles[i].find("player.json") != std::string::npos) {
			// Load the player config
			player = new Player(contentDirectory + "entities\\player.json");
			if (!player->IsLoaded()) {
				engineDebugger.WriteLine(">>>> ERROR!!!! - Failed to load player");
				engineDebugger.WriteLine(">> 14 - FAILED");
				this->Close();
			}
		}
		else {
			// Load the NPCs
			if (listOfEntityFiles[i].find("boss") != std::string::npos) {
				// TODO: Implement Boss class, use NPC as base
			}
			else {
				NonPlayerCharacter* newEntity = new NonPlayerCharacter(listOfEntityFiles[i]);
				if (newEntity->IsLoaded()) {
					// Success!
					entityRegister.push_back(newEntity);
				}
				else {
					// Failed to load the entity, runtime does continue
					engineDebugger.WriteLine(">>>> ERROR!!!! - Failed to load entity " + listOfEntityFiles[i]);
					engineDebugger.WriteLine(">> 14 - FAILED");
				}
			}
		}
	}

	// Make sure we have a player loaded
	if (player == nullptr) {
		engineDebugger.WriteLine(">>>> ERROR!!!! - No Player was loaded");
		engineDebugger.WriteLine(">> 14 - FAILED");
		this->Close();
	}

	engineDebugger.WriteLine(">> 14 - COMPLETE");
}
void Engine::LoadCameras(void) {
	engineDebugger.WriteLine(">> 15 - Loading Cameras");

	mainCamera = new Camera(glm::vec3(0.0f, 0.0f, 1.0f));
	mainCameraFocus = player;

	engineDebugger.WriteLine(">> 15 - COMPLETE");
}

// Master Load Function, calls all the other individual loads
void Engine::LoadEngine(void) {
	engineDebugger.WriteLine("#### ARKENGINE LOAD BEGINNING");

	// Load Engine Core configs
	ConfigureDebugging();
	LoadEngineConfig();

	// Setup the Graphics Environment, Initialising OpenGL and loading our libraries
	LoadExternalLibraries();

	// Load the user settings
	LoadUserSettings();

	// Load the shaders
	LoadShaders();

	// Load the Input devices attached to the system
	LoadInputDevices();

	// Load Game Content
	LoadFonts();
	LoadTextObjects();
	LoadSoundEffects();
	LoadTextures();
	LoadModels();
	LoadTilesets();
	LoadLevels();
	LoadItems();
	LoadEntities();
	LoadCameras();

	// Load complete
	isRunning = true;	// Allows the game loop to run
	engineDebugger.WriteLine("#### ARKENGINE LOAD COMPLETE");
}

// Configuration functions
void Engine::WindowResize(const glm::vec2& newScaler) {
	// Resizes the window
	windowScaler = newScaler;
	windowDimensions = (tileSize * windowGridSize) * windowScaler;
	SDL_SetWindowSize(sdlWindow, (int)windowDimensions.x, (int)windowDimensions.y);
}
void Engine::WindowRename(const std::string& newName) {
	windowTitle = newName;
	SDL_SetWindowTitle(sdlWindow, windowTitle.c_str());
}
void Engine::SetVSyncState(const bool& newState) {
	SDL_GL_SetSwapInterval(newState);	// Toggles V-Sync on and off for the SDL Window

}

// Core game loop steps
void Engine::EventHandler(void) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_KEYDOWN:			// Key down handling.
			deviceKeyboard->UpdateKeyStates_Down(event.key);
			break;
		case SDL_KEYUP:				// Key up handling.
			deviceKeyboard->UpdateKeyStates_Up(event.key);
			break;
		case SDL_CONTROLLERAXISMOTION:
			for (int i = 0; i < deviceGameControllerRegister.size(); i++) {
				deviceGameControllerRegister[i]->UpdateThumbSticks();
			}
			break;
		case SDL_CONTROLLERBUTTONDOWN:
			for (int i = 0; i < deviceGameControllerRegister.size(); i++) {
				deviceGameControllerRegister[i]->UpdateButtonStates();
			}
			break;
		case SDL_CONTROLLERBUTTONUP:
			for (int i = 0; i < deviceGameControllerRegister.size(); i++) {
				deviceGameControllerRegister[i]->UpdateButtonStates();
			}
			break;
		}
	}
}
void Engine::Update(const float& deltaTime) {

	// DEBUGGING
	if (deviceKeyboard->GetKeyState(Keyboard::P)) {
		GetCurrentLevel()->Reload();
	}

	// Run the current Level's update function
	GetCurrentLevel()->Update(deltaTime);

	// Run the player's update function
	player->Update(deltaTime);

	// Update the text objects
	const size_t renderableTextRegisterSize = textObjectRegister.size();	// Grab size once on the update cycle, to prevent re-calculation every iteration
	for (size_t i = 0; i < renderableTextRegisterSize; i++) {
		textObjectRegister[i]->Update(deltaTime);
	}

	// Check the camera target has been initialised
	if (mainCameraFocus != nullptr) {
		// Run the camera's update function
		mainCamera->Update(deltaTime, *mainCameraFocus);
	}
}
void Engine::Render(void) {
	// Pre-render
	glViewport(0, 0, (int)windowDimensions.x, (int)windowDimensions.y);
	glClearColor(100 / 255.0f, 149 / 255.0f, 237 / 255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw the level
	GetCurrentLevel()->Draw();

	// Draw the player
	player->Draw();

	// Render the text objects
	const size_t renderableTextRegisterSize = textObjectRegister.size();	// Grab size once on the update cycle, to prevent re-calculation every iteration
	for (size_t i = 0; i < renderableTextRegisterSize; i++) {
		textObjectRegister[i]->Draw();
	}

	// Post-Render
	SDL_GL_SwapWindow(sdlWindow);	// Gives the frame buffer to the display (swapBuffers).
}

// Game Loop
void Engine::Run(void) {

	// Loads all the configs, the game content and initialises everything needed by the engine to run.
	LoadEngine();

	engineDebugger.WriteLine("\n## Game Runtime - Begun");

	// FPS variables
	int fpsCounter = 0;
	float secondsCounter = 0;
	float maxDeltatime = 0.05f;	// Equivalent to 60 FPS timer

	while (isRunning) {
		// DeltaTime math
		currentFrameTime = (float)SDL_GetTicks();
		float deltaTime = ((currentFrameTime - oldFrameTime) / 1000);

		// This caps out deltaTime to prevent the insanely high deltaTime caused by moving the window or game loading
		if (deltaTime > maxDeltatime) {
			deltaTime = maxDeltatime;
			engineDebugger.WriteLine("CAPPED DELTATIME");
		}

		// Main Game loop
		EventHandler();			// Handle any events
		Update(deltaTime);		// Update the game
		Render();				// Render the game

		// FPS math, called after the engine has finished its render function.
		fpsCounter++;						// Counts the number of frames as they've been rendered.
		secondsCounter += deltaTime;		// Counts up to the next second
		if (secondsCounter >= 1) {
			// If it has been a second since the last FPS count, reset the counter and print.
			WindowRename(defaultWindowTitle + " - FPS: " + std::to_string(fpsCounter));
			textObjectRegister[1]->UpdateText("FPS: " + std::to_string(fpsCounter));
			fpsCounter = 0;
			secondsCounter = 0;
		}
		oldFrameTime = currentFrameTime;
	}

	engineDebugger.WriteLine("## Game Runtime - Finished");
	this->Close(true);
}

// Engine Utilities
glm::ivec2 Engine::ConvertToGridPosition(const glm::vec2& position) {
	return glm::vec2(glm::floor(position.x / this->tileSize.x), glm::floor(position.y / this->tileSize.y));
}
const int Engine::GetIndexOfShader(const std::string& shaderName) {
	int indexOfDesiredShader = -1;
	const size_t shaderRegisterSize = shaderRegister.size();
	for (size_t i = 0; i < shaderRegisterSize; i++) {
		if (shaderRegister[i]->GetName().find(shaderName) != std::string::npos) {
			indexOfDesiredShader = (int)i;
		}
	}
	return indexOfDesiredShader;
}
const int Engine::GetIndexOfModel(const std::string& modelName) {
	int indexOfDesiredModel = -1;
	const size_t modelRegisterSize = modelRegister.size();
	for (size_t i = 0; i < modelRegisterSize; i++) {
		if (modelRegister[i].name.find(modelName) != std::string::npos) {
			indexOfDesiredModel = (int)i;
		}
	}
	return indexOfDesiredModel;
}
const int Engine::GetIndexOfTexture(const std::string& textureName) {
	int indexOfDesiredTexture = -1;
	const size_t textureRegisterSize = textureRegister.size();
	for (size_t i = 0; i < textureRegisterSize; i++) {
		if (textureRegister[i].GetName().find(textureName) != std::string::npos) {
			indexOfDesiredTexture = (int)i;
		}
	}
	return indexOfDesiredTexture;
}
const int Engine::GetIndexOfFont(const std::string& fontName) {
	int indexOfDesiredFont = -1;
	const size_t fontRegisterSize = fontRegister.size();
	for (size_t i = 0; i < fontRegisterSize; i++) {
		if (fontRegister[i]->name.find(fontName) != std::string::npos) {
			indexOfDesiredFont = (int)i;
		}
	}
	return indexOfDesiredFont;
}
const int Engine::GetIndexOfRenderableText(const std::string& renderableTextName) {
	int indexOfDesiredRenderableText = -1;
	const size_t renderableTextRegisterSize = textObjectRegister.size();
	for (size_t i = 0; i < renderableTextRegisterSize; i++) {
		if (textObjectRegister[i]->GetName().find(renderableTextName) != std::string::npos) {
			indexOfDesiredRenderableText = (int)i;
		}
	}
	return indexOfDesiredRenderableText;
}
const int Engine::GetIndexOfSoundEffect(const std::string& soundEffectName) {
	int indexOfDesiredSoundEffect = -1;
	const size_t soundEffectRegisterSize = soundRegister.size();
	for (size_t i = 0; i < soundEffectRegisterSize; i++) {
		if (soundRegister[i]->GetName().find(soundEffectName) != std::string::npos) {
			indexOfDesiredSoundEffect = (int)i;
		}
	}
	return indexOfDesiredSoundEffect;
}
const int Engine::GetIndexOfTileset(const std::string& tilesetName) {
	int indexOfDesiredTileset = -1;
	const size_t tilesetRegisterSize = tilesetRegister.size();
	for (size_t i = 0; i < tilesetRegisterSize; i++) {
		if (tilesetRegister[i]->GetName().find(tilesetName) != std::string::npos) {
			indexOfDesiredTileset = (int)i;
		}
	}
	return indexOfDesiredTileset;
}
const int Engine::GetIndexOfEntity(const std::string& entityName) {
	int indexOfDesiredEntity = -1;
	const size_t entityRegisterSize = entityRegister.size();
	for (size_t i = 0; i < entityRegisterSize; i++) {
		if (entityRegister[i]->GetName().find(entityName) != std::string::npos) {
			indexOfDesiredEntity = (int)i;
		}
	}
	return indexOfDesiredEntity;
}
const int Engine::GetIndexOfLevel(const std::string& levelName) {
	int indexOfDesiredLevel = -1;
	const size_t levelRegisterSize = levelRegister.size();
	for (size_t i = 0; i < levelRegisterSize; i++) {
		if (levelRegister[i]->name.find(levelName) != std::string::npos) {
			indexOfDesiredLevel = (int)i;
		}
	}
	return indexOfDesiredLevel;
}
Level* Engine::GetCurrentLevel(void) {
	if (indexOfCurrentLevel == -1 || indexOfCurrentLevel > levelRegister.size()) {
		engineDebugger.WriteLine(">>>> ERROR!!!! - Tried to access a level outside the range of the register");
		this->Close();
		return nullptr;
	}
	else {
		return levelRegister[indexOfCurrentLevel];
	}
}
