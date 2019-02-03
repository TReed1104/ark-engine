#include "Engine.h"

// Constructors
Engine::Engine(char* gameName) {
	configFile = nullptr;

	windowTitle = gameName;
	defaultWindowTitle = gameName;
	oldFrameTime = 0.0f;
	currentFrameTime = 0.0f;

	contentDirectory = "content/";

	indexOfDefaultShader = 0;
	indexOfCurrentLevel = -1;

	nameOfDefaultTileModel = "";
	nameOfDefaultSpriteModel = "";
	nameOfDefaultTexture = "";
	indexOfTileModel = -1;
	indexOfDefaultTexture = -1;
	indexOfSpriteModel = -1;

	mainCamera = nullptr;
	mainCameraFocus = nullptr;
	player = nullptr;
	deviceKeyboard = nullptr;
}
Engine::~Engine(void) {

}

// Clean Up functions
void Engine::CleanUp(void) {
	std::cout << "Cleanup - Begun" << std::endl;

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

	// Delete the loaded Shaders.
	const size_t shaderRegisterSize = shaderRegister.size();
	for (size_t i = 0; i < shaderRegisterSize; i++) {
		delete shaderRegister[i];
	}

	if (SDL_NumJoysticks() > 0) {
		const size_t deviceGameControllerRegisterSize = deviceGameControllerRegister.size();
		for (size_t i = 0; i < deviceGameControllerRegisterSize; i++) {
			SDL_GameControllerClose(deviceGameControllerRegister[i]->GetSDLHook());	// Close the controller.
		}
	}

	FT_Done_FreeType(freeTypeLibrary);
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(sdlWindow);
	std::cout << "Cleanup - Complete" << std::endl;
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
		exit(1);
	}
}

// Core Engine Loading
void Engine::LoadEngineConfig(void) {
	std::cout << ">> 1 - Loading Engine Configs" << std::endl;
	configFile = new JsonFile(contentDirectory + "engine.json");	// Load the config file into memory
	if (configFile->IsLoaded()) {
		// Debugging setup
		Debug::isDebuggingEnabled = configFile->Get<bool>("engine.debugging.debugging enabled");
		Debug::isLoggingEnabled = configFile->Get<bool>("engine.debugging.logging enabled");
		Debug::loggingFileName = configFile->Get<std::string>("engine.debugging.log file");

		// Window Engine setup
		windowTitle = configFile->Get<std::string>("engine.window.title");
		defaultWindowTitle = configFile->Get<std::string>("engine.window.title");
		tileSize = glm::vec2(configFile->Get<int>("engine.window.tile size.width"), configFile->Get<int>("engine.window.tile size.height"));
		windowGridSize = glm::vec2(configFile->Get<int>("engine.window.grid size.width"), configFile->Get<int>("engine.window.grid size.height"));
		windowScaler = glm::vec2(configFile->Get<int>("engine.window.scalar.x"), configFile->Get<int>("engine.window.scalar.y"));
		windowDimensions = (tileSize * windowGridSize) * windowScaler;

		// Texture Source Frame setup
		textureBorderSize = glm::ivec2(configFile->Get<int>("engine.content.texture config.texture border size.width"), configFile->Get<int>("engine.content.texture config.texture border size.height"));
		tileTextureFrameSize = glm::ivec2(configFile->Get<int>("engine.content.texture config.tile frame dimensions.width"), configFile->Get<int>("engine.content.texture config.tile frame dimensions.height"));
		entityTextureFrameSize = glm::ivec2(configFile->Get<int>("engine.content.texture config.sprite frame dimensions.width"), configFile->Get<int>("engine.content.texture config.sprite frame dimensions.height"));

		// Default content setup
		nameOfDefaultTileModel = configFile->Get<std::string>("engine.content.default content.tile");
		nameOfDefaultSpriteModel = configFile->Get<std::string>("engine.content.default content.sprite");
		nameOfDefaultTexture = configFile->Get<std::string>("engine.content.default content.texture");

		// Controller setup
		maxNumberOfControllers = configFile->Get<int>("engine.game controller.max number of controllers");
		indexOfPlayerController = configFile->Get<int>("engine.game controller.index of player");
		thumbStickDeadZone = configFile->Get<int>("engine.game controller.thumb stick dead zone");
		triggerDeadZone = configFile->Get<int>("engine.game controller.trigger dead zone");
		pressedStateFlag = configFile->Get<int>("engine.game controller.pressed state flag");

	}
	else {
		// Config failed to load.
		std::cout << ">>>> ERROR!!!! - Engine config wasn't loaded " << std::endl;
		std::cout << ">> 1 - FAILED" << std::endl;
		this->Close();
	}

	std::cout << ">> 1 - COMPLETE" << std::endl;
}
void Engine::LoadKeyBindings(void) {
	std::cout << ">> 2 - Loading Keybinds" << std::endl;

	if (configFile->IsLoaded()) {
		size_t numberOfKeybinds = configFile->SizeOfObjectArray("engine.key bindings");
		for (size_t i = 0; i < numberOfKeybinds; i++) {
			std::string nameOfBinding = configFile->Get<std::string>("engine.key bindings." + std::to_string(i) + ".binding.id");

			Keybind bind { 
				configFile->Get<std::string>("engine.key bindings." + std::to_string(i) + ".binding.id"),
				configFile->Get<std::string>("engine.key bindings." + std::to_string(i) + ".binding.friendly name"),
				(Keyboard::Keys)configFile->Get<int>("engine.key bindings." + std::to_string(i) + ".binding.key id")
			};

			keybindingHandler.AddKeybinding(bind);
		}
	}
	else {
		// Config failed to load.
		std::cout << ">>>> ERROR!!!! - Engine config wasn't loaded " << std::endl;
		std::cout << ">> 2 - FAILED" << std::endl;
		this->Close();
	}

	std::cout << ">> 2 - COMPLETE" << std::endl;
}
void Engine::LoadEnginePointers(void) {
	std::cout << ">> 3 - Loading Engine Pointer" << std::endl;

	Texture::Engine_Pointer = this;
	Model::Engine_Pointer = this;
	Font::Engine_Pointer = this;
	TextObject::Engine_Pointer = this;
	Keyboard::Engine_Pointer = this;
	GameController::Engine_Pointer = this;
	Camera::Engine_Pointer = this;
	BoundingBox::Engine_Pointer = this;
	Animation::Engine_Pointer = this;
	Animation::Frame::Engine_Pointer = this;
	GameObject::Engine_Pointer = this;
	Tileset::Engine_Pointer = this;
	Level::Engine_Pointer = this;

	std::cout << ">> 3 - COMPLETE" << std::endl;
}

// Library setup
void Engine::CreateSDLWindow(void) {
	std::cout << ">>>>>> 4.1.1 - Creating SDL Window" << std::endl;

	// Create window
	sdlWindow = SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)windowDimensions.x, (int)windowDimensions.y, SDL_WINDOW_OPENGL);

	// Error handling for the SDL Window.
	if (sdlWindow == nullptr) {
		std::cout << ">> SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		std::cout << ">>>>>> 4.1.1 - FAILED" << std::endl;
		this->Close();
	}

	std::cout << ">>>>>> 4.1.1 - COMPLETE" << std::endl;
}
void Engine::CreateSDLContext(void) {
	std::cout << ">>>>>> 4.1.2 - Loading SDL_GL Context" << std::endl;

	// Sets up the OpenGL context for OpenGL version 3.3 - This is for 32bit Windows.
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);		// Use the Core profile.
	glContext = SDL_GL_CreateContext(sdlWindow);		// Try and Setup the SDL Context for OpenGL 3.3, if the platform does not support OpenGL 3.3 the context will be a null pointer.

	if (glContext == nullptr) {
		// If setup for OpenGL 3.3 and OpenGL 2.0 both failed, The program will display an error and close.
		SDL_DestroyWindow(sdlWindow);
		std::cout << ">>>>>> ERROR!!!! - SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;	// Print the error.
		std::cout << ">>>>>> 4.1.2 - FAILED" << std::endl;
		this->Close();
	}
	SDL_GL_SetSwapInterval(0);	// Toggles V-Sync on and off for the SDL Window

	std::cout << ">>>>>> 4.1.2 - COMPLETE" << std::endl;
}
void Engine::LoadSDL(void) {
	std::cout << ">>>> 4.1 - Loading Library - SDL2" << std::endl;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << ">>>> ERROR!!!! - SDL_Init Error: " << SDL_GetError() << std::endl;
		std::cout << ">>>> 4.1 - FAILED" << std::endl;
		this->Close();
	}
	CreateSDLWindow();
	CreateSDLContext();

	std::cout << ">>>> 4.1 - COMPLETE" << std::endl;
}
void Engine::LoadGLEW(void) {
	std::cout << ">>>> 4.2 - Loading Library - GLEW" << std::endl;

	GLenum rev;
	glewExperimental = GL_TRUE;
	rev = glewInit();
	if (GLEW_OK != rev) {
		// If GLEW fails, close the program.
		std::cout << ">>>> ERROR!!!! - GLEW Error: " << glewGetErrorString(rev) << std::endl;
		std::cout << ">>>> 4.2 - FAILED" << std::endl;
		this->Close();
	}

	std::cout << ">>>> 4.2 - COMPLETE" << std::endl;
}
void Engine::LoadFreeType(void) {
	std::cout << ">>>> 4.3 - Loading Library - FreeType2" << std::endl;

	// Initialise the FreeType library
	if (FT_Init_FreeType(&freeTypeLibrary)) {
		std::cout << ">>>> ERROR!!!! - FreeType Failed to Initialise" << std::endl;
		std::cout << ">>>> 4.3 - FAILED" << std::endl;
		this->Close();
	}

	std::cout << ">>>> 4.3 - COMPLETE" << std::endl;
}
void Engine::LoadExternalLibraries(void) {
	std::cout << ">> 4 - Loading External Libraries" << std::endl;

	// Load the Libraries
	LoadSDL();
	LoadGLEW();
	LoadFreeType();

	// Setup Core OpenGL
	glViewport(0, 0, (int)windowDimensions.x, (int)windowDimensions.y);
	SDL_GL_SwapWindow(sdlWindow);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);

	std::cout << ">> 4 - COMPLETE" << std::endl;
}

// Loading Functions
void Engine::LoadShaders(void) {
	std::cout << ">> 5 - Loading Shaders" << std::endl;

	if (configFile->IsLoaded()) {
		size_t numberOfShaders = configFile->SizeOfObjectArray("engine.shaders");
		for (size_t i = 0; i < numberOfShaders; i++) {
			std::string nameOfShader = contentDirectory + "shaders/" + configFile->Get<std::string>("engine.shaders." + std::to_string(i) + ".shader.id");
			std::string vertexShaderName = contentDirectory + "shaders/" + configFile->Get<std::string>("engine.shaders." + std::to_string(i) + ".shader.vertex");
			std::string fragmentShaderName = contentDirectory + "shaders/" + configFile->Get<std::string>("engine.shaders." + std::to_string(i) + ".shader.fragment");

			// Create and Load the shader
			Shader* newShader = new Shader(nameOfShader, vertexShaderName, fragmentShaderName);
			if (newShader->IsLoaded()) {
				// Loaded successfully, storing it for use
				shaderRegister.push_back(newShader);
			}
			else {
				std::cout << ">>>> ERROR!!!! - Failed to load Shader " << nameOfShader << std::endl;
				std::cout << ">> 5 - FAILED" << std::endl;
				this->Close();
			}
		}
	}
	else {
		std::cout << ">>>> ERROR!!!! - Engine config wasn't loaded " << std::endl;
		std::cout << ">> 5 - FAILED" << std::endl;
		this->Close();
	}

	std::cout << ">> 5 - COMPLETE" << std::endl;
}
void Engine::LoadInputDevices(void) {
	std::cout << ">> 6 - Loading Input Devices" << std::endl;

	// Initialise the keyboard instance
	std::cout << ">>>> 6.1 - Keyboard Initialised..." << std::endl;
	deviceKeyboard = new Keyboard();


	// Search for any Keyboards.
	int numberOfConnectedControllers = SDL_NumJoysticks();
	if (numberOfConnectedControllers < 1) {
		//deviceGameController = nullptr;
		std::cout << ">>>> 6.2 - No Controllers were found..." << std::endl;
	}
	else {
		for (int i = 0; i < numberOfConnectedControllers; i++) {
			if (i < maxNumberOfControllers) {
				deviceGameControllerRegister.push_back(new GameController(SDL_GameControllerOpen(i)));
				if (deviceGameControllerRegister[i]->GetSDLHook() != NULL) {
					std::cout << ">>>> 6.2 - Game controller found: Controller has been opened for input" << std::endl;
				}
				else {
					std::cout << ">>>> ERROR!!!! - Unable to Open game controller for use! SDL Error: " << SDL_GetError() << std::endl;
				}
			}
		}

	}

	std::cout << ">> 6 - COMPLETE" << std::endl;
}
void Engine::LoadFonts(void) {
	std::cout << ">> 7 - Loading Fonts" << std::endl;

	if (configFile->IsLoaded()) {
		size_t numberOfFonts = configFile->SizeOfObjectArray("engine.fonts");
		for (size_t i = 0; i < numberOfFonts; i++) {
			std::string fontName = configFile->Get<std::string>("engine.fonts." + std::to_string(i) + ".font.id");
			std::string fontPath = contentDirectory + "fonts/" + configFile->Get<std::string>("engine.fonts." + std::to_string(i) + ".font.source");
			int fontSize = configFile->Get<int>("engine.fonts." + std::to_string(i) + ".font.size");
			Font* newFont = new Font(fontName, fontPath, fontSize);
			if (newFont->isLoaded) {
				fontRegister.push_back(newFont);
			}
			else {
				std::cout << ">>>> ERROR!!!! - Failed to load Font: " << fontPath << std::endl;
				std::cout << ">>>> 7 - FAILED" << std::endl;
				this->Close();
			}
		}
	}
	else {
		// Config failed to load.
		std::cout << ">>>> ERROR!!!! - Engine config wasn't loaded " << std::endl;
		std::cout << ">> 7 - FAILED" << std::endl;
		this->Close();
	}

	std::cout << ">> 7 - COMPLETE" << std::endl;
}
void Engine::LoadTextObjects(void) {
	std::cout << ">> 8 - Loading Text Objects" << std::endl;
	const int indexOfFont = GetIndexOfFont("Arial");
	textObjectRegister.push_back(new TextObject("UI: Health", "Health: 100%", fontRegister[indexOfFont], glm::vec3(20.0f, 20.0f, 0.02f), glm::vec3(255 / 255.0f, 255 / 255.0f, 255 / 255.0f), true));
	textObjectRegister.push_back(new TextObject("DEBUG: FPS Counter", "FPS: 0", fontRegister[indexOfFont], glm::vec3(20.0f, 30.0f, 0.02f), glm::vec3(255 / 255.0f, 255 / 255.0f, 255 / 255.0f), true));
	textObjectRegister.push_back(new TextObject("DEBUG: Grid Position", "GridPos: (0, 0)", fontRegister[indexOfFont], glm::vec3(20.0f, 40.0f, 0.02f), glm::vec3(255 / 255.0f, 255 / 255.0f, 255 / 255.0f), true));
	std::cout << ">> 8 - COMPLETE" << std::endl;
}
void Engine::LoadTextures(void) {
	std::cout << ">> 9 - Loading Textures" << std::endl;

	std::vector<std::string> listOfTextures = FileSystemUtilities::GetFileList(contentDirectory + "textures");
	const size_t textureFileListSize = listOfTextures.size();
	for (size_t i = 0; i < textureFileListSize; i++) {
		textureRegister.push_back(Texture(listOfTextures[i], true, true));
		if (!textureRegister.back().isLoaded) {
			std::cout << ">>>> ERROR!!!! - Failed to load texture" << listOfTextures[i] << std::endl;
			std::cout << ">>>> 9 - FAILED" << std::endl;
			this->Close();
		}
	}

	// Find the default texture for when textures are failed to be found.
	if ((indexOfDefaultTexture = GetIndexOfTexture(nameOfDefaultTexture)) == -1) {
		std::cout << ">>>> ERROR!!!! - Unable to find default texture" << std::endl;
		std::cout << ">>>> 9 - FAILED" << std::endl;
		this->Close();
	}

	std::cout << ">> 9 - COMPLETE" << std::endl;
}
void Engine::LoadModels(void) {
	std::cout << ">> 10 - Loading Models" << std::endl;

	std::vector<std::string> listOfModels = FileSystemUtilities::GetFileList(contentDirectory + "models");
	const size_t listOfModelSize = listOfModels.size();
	for (size_t i = 0; i < listOfModelSize; i++) {
		modelRegister.push_back(Model(listOfModels[i]));
	}

	// Find the default models
	if ((indexOfTileModel = GetIndexOfModel(nameOfDefaultTileModel)) == -1) {
		std::cout << ">>>> ERROR!!!! - Unable to find default model" << std::endl;
		std::cout << ">>>> 10 - FAILED" << std::endl;
		this->Close();
	}
	if ((indexOfSpriteModel = GetIndexOfModel(nameOfDefaultSpriteModel)) == -1) {
		indexOfSpriteModel = indexOfTileModel;
	}

	std::cout << ">> 10 - COMPLETE" << std::endl;
}
void Engine::LoadTilesets(void) {
	std::cout << ">> 11 - Loading Tilesets" << std::endl;

	std::vector<std::string> listOfTilesets = FileSystemUtilities::GetFileList(contentDirectory + "tilesets");
	const size_t listOfTilesetsSize = listOfTilesets.size();
	for (size_t i = 0; i < listOfTilesetsSize; i++) {
		Tileset* newTileSet = new Tileset(listOfTilesets[i]);
		if (newTileSet->IsLoaded()) {
			tilesetRegister.push_back(newTileSet);
		}
		else {
			std::cout << ">>>> ERROR!!!! - Failed to load Shader " << listOfTilesets[i] << std::endl;
			std::cout << ">> 11 - FAILED" << std::endl;
			this->Close();
		}
	}

	std::cout << ">> 11 - COMPLETE" << std::endl;
}
void Engine::LoadLevels(void) {
	std::cout << ">> 12 - Loading Levels" << std::endl;

	std::vector<std::string> listOfLevelFiles = FileSystemUtilities::GetFileList(contentDirectory + "levels");
	const size_t listOfLevelFilesSize = listOfLevelFiles.size();
	for (size_t i = 0; i < listOfLevelFilesSize; i++) {
		Level* newLevel = new Level(listOfLevelFiles[i]);
		if (newLevel->IsLoaded()) {
			levelRegister.push_back(newLevel);
		}
		else {
			std::cout << ">>>> ERROR!!!! - Failed to load Shader " << listOfLevelFiles[i] << std::endl;
			std::cout << ">> 12 - FAILED" << std::endl;
			this->Close();
		}
	}
	indexOfCurrentLevel = 0;

	std::cout << ">> 12 - COMPLETE" << std::endl;
}
void Engine::LoadItems(void) {
	std::cout << ">> 13 - Loading Items" << std::endl;
	std::cout << ">>>> NOT IMPLEMENTED" << std::endl;

	std::cout << ">> 13 - COMPLETE" << std::endl;
}
void Engine::LoadPlayer(void) {
	std::cout << ">> 14 - Loading Players" << std::endl;

	player = new Player(contentDirectory + "entities/player.json");

	std::cout << ">> 14 - COMPLETE" << std::endl;
}
void Engine::LoadEntities(void) {
	std::cout << ">> 15 - Loading Entities" << std::endl;
	std::cout << ">>>> NOT IMPLEMENTED" << std::endl;

	std::cout << ">> 15 - COMPLETE" << std::endl;
}
void Engine::LoadCameras(void) {
	std::cout << ">> 16 - Loading Cameras" << std::endl;

	mainCamera = new Camera(glm::vec3(0.0f, 0.0f, 1.0f));
	mainCameraFocus = player;

	std::cout << ">> 16 - COMPLETE" << std::endl;
}

// Master Load Function, calls all the other individual loads
void Engine::LoadEngine(void) {
	std::cout << "#### ARKENGINE LOAD BEGINNING" << std::endl;

	// Load Engine Core configs
	LoadEngineConfig();
	LoadKeyBindings();
	LoadEnginePointers();

	// Setup the Graphics Environment, Initialising OpenGL and loading our libraries
	LoadExternalLibraries();

	// Load the shaders
	LoadShaders();

	// Load the Input devices attached to the system
	LoadInputDevices();

	// Load Game Content
	LoadFonts();
	LoadTextObjects();
	LoadTextures();
	LoadModels();
	LoadTilesets();
	LoadLevels();
	LoadItems();
	LoadPlayer();
	LoadEntities();
	LoadCameras();

	// Load complete
	isRunning = true;	// Allows the game loop to run
	std::cout << "#### ARKENGINE LOAD COMPLETE" << std::endl;
}

// Events
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
	if (deviceKeyboard->GetKeyState(Keyboard::P)) {
		GetCurrentLevel()->Reload();
	}
	// Check the current level has been initialised
	if (GetCurrentLevel() != nullptr) {
		// Run the current Level's update function
		GetCurrentLevel()->Update(deltaTime);
	}

	// Check the player has been initialised
	if (player != nullptr) {
		// Run the player's update function
		player->Update(deltaTime);
	}

	// Update the text objects
	const size_t renderableTextRegisterSize = textObjectRegister.size();	// Grab size once on the update cycle, to prevent re-calculation every iteration
	for (size_t i = 0; i < renderableTextRegisterSize; i++) {
		// TODO: Add a check condition to see if the text is currently active
		textObjectRegister[i]->Update(deltaTime);
	}

	// Check the camera target has been initialised
	if (mainCameraFocus != nullptr) {
		// Run the camera's update function
		// TODO: Amend this to use a list of cameras?
		mainCamera->Update(deltaTime, *mainCameraFocus);
	}
}
void Engine::Render(void) {
	// Pre-render
	glViewport(0, 0, (int)windowDimensions.x, (int)windowDimensions.y);
	glClearColor(100 / 255.0f, 149 / 255.0f, 237 / 255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw the level
	if (GetCurrentLevel() != nullptr) {
		GetCurrentLevel()->Draw();
	}

	// Draw the player
	if (player != nullptr) {
		player->Draw();
	}

	// Render the text objects
	const size_t renderableTextRegisterSize = textObjectRegister.size();	// Grab size once on the update cycle, to prevent re-calculation every iteration
	for (size_t i = 0; i < renderableTextRegisterSize; i++) {
		// TODO: Add a check condition to see if the text is currently active
		textObjectRegister[i]->Draw();
	}

	// Post-Render
	SDL_GL_SwapWindow(sdlWindow);	// Gives the frame buffer to the display (swapBuffers).
}

// Game Loop
void Engine::Run(void) {

	// Loads all the configs, the game content and initialises everything needed by the engine to run.
	LoadEngine();

	std::cout << "\n## Game Runtime - Begun" << std::endl;

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
			std::cout << "capped deltaTime" << std::endl;
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
	std::cout << "## Game Runtime - Finished" << std::endl;
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
		if (textureRegister[i].name.find(textureName) != std::string::npos) {
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
	if (indexOfCurrentLevel != -1) {
		return levelRegister[indexOfCurrentLevel];
	}
	else {
		return nullptr;
	}
}
