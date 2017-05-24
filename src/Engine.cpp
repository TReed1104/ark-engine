#include "Engine.h"

Engine::Engine(char* gameName) {
	windowTitle = gameName;
	oldFrameTime = 0.0f;
	currentFrameTime = 0.0f;

	contentDirectory = "content/";

	indexOfDefaultShader = 0;
	indexCurrentLevel = -1;

	nameOfTileModel = "";
	nameOfSpriteModel = "";
	nameOfDefaultTexture = "";
	indexOfTileModel = -1;
	indexOfDefaultTexture = -1;
	indexOfSpriteModel = -1;

	camera = nullptr;
	player = nullptr;
	deviceKeyboard = nullptr;
}
Engine::~Engine(void) {

}

// Engine config related functions
void Engine::LoadEngineConfig(void) {
	// Loads the main config file for the engine
	LuaScript configScript = LuaScript(contentDirectory + "config/engine_config.lua");
	if (configScript.isScriptLoaded) {
		// Core setup
		windowTitle = configScript.Get<std::string>("config.window.title");
		tileSize = glm::vec2(configScript.Get<int>("config.window.tile_size.x"), configScript.Get<int>("config.window.tile_size.y"));
		windowGridSize = glm::vec2(configScript.Get<int>("config.window.grid_size.x"), configScript.Get<int>("config.window.grid_size.y"));
		windowScaler = glm::vec2(configScript.Get<int>("config.window.scale.x"), configScript.Get<int>("config.window.scale.y"));
		windowDimensions = (tileSize * windowGridSize) * windowScaler;

		// Default content setup
		nameOfTileModel = configScript.Get<std::string>("config.default_content.tile");
		nameOfSpriteModel = configScript.Get<std::string>("config.default_content.sprite");
		nameOfDefaultTexture = configScript.Get<std::string>("config.default_content.texture");

		// Controller setup
		maxNumberOfControllers = configScript.Get<int>("config.game_controller.max_number_of_controllers");
		indexOfPlayerController = configScript.Get<int>("config.game_controller.index_of_player_controller");
		thumbStickDeadZone = configScript.Get<int>("config.game_controller.thumb_stick_dead_zone");
		triggerDeadZone = configScript.Get<int>("config.game_controller.trigger_dead_zone");
		pressedStateFlag = configScript.Get<int>("config.game_controller.press_state_flag");
	}
	else {
		// Config failed to load.
		this->Close();
	}
}
void Engine::LoadKeyBindings(void) {
	// Loads the Keybinding config file
	LuaScript keybindScript = LuaScript(contentDirectory + "config/key_bindings.lua");
	if (keybindScript.isScriptLoaded) {
		keybindMovementUp = (Keyboard::Keys)keybindScript.Get<int>("keybindings.movement_up");
		keybindMovementDown = (Keyboard::Keys)keybindScript.Get<int>("keybindings.movement_down");
		keybindMovementLeft = (Keyboard::Keys)keybindScript.Get<int>("keybindings.movement_left");
		keybindMovementRight = (Keyboard::Keys)keybindScript.Get<int>("keybindings.movement_right");
	}
	else {
		// Config failed to load.
		this->Close();
	}
}
void Engine::LoadEnginePointers(void) {
	// Sets the Engine_Pointer static of each of the Engines classes.
	std::cout << ">> Setting static pointers - Begun" << std::endl;
	Model::Engine_Pointer = this;
	Keyboard::Engine_Pointer = this;
	GameController::Engine_Pointer = this;
	Camera::Engine_Pointer = this;
	BoundingBox::Engine_Pointer = this;
	Animation::Engine_Pointer = this;
	Animation::Frame::Engine_Pointer = this;
	GameObject::Engine_Pointer = this;
	Tileset::Engine_Pointer = this;
	Level::Engine_Pointer = this;
	std::cout << ">> Setting static pointers - Complete" << std::endl;
}
// OpenGL and SDL related functions
void Engine::InitialiseSDL(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << ">> SDL_Init Error: " << SDL_GetError() << std::endl;
		exit(1);
	}
	std::cout << ">> SDL initialised Successfully!" << std::endl;
}
void Engine::CreateSDLWindow(void) {
	// Create window
	sdlWindow = SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowDimensions.x, windowDimensions.y, SDL_WINDOW_OPENGL);

	// Error handling for the SDL Window.
	if (sdlWindow == nullptr) {
		std::cout << ">> SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		this->Close();
	}
	std::cout << ">> SDL Window Created Successfully!" << std::endl;
}
void Engine::CreateSDLContext(void) {

	// Sets up the OpenGL context for OpenGL version 3.3 - This is for 32bit Windows.
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);		// Use the Core profile.
	glContext = SDL_GL_CreateContext(sdlWindow);		// Try and Setup the SDL Context for OpenGL 3.3, if the platform does not support OpenGL 3.3 the context will be a null pointer.

	if (glContext == nullptr) {
		// If setup for OpenGL 3.3 and OpenGL 2.0 both failed, The program will display an error and close.
		SDL_DestroyWindow(sdlWindow);
		std::cout << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;	// Print the error.
		this->Close();
	}

	// Print the OpenGL version the program is using (3.3 or 2.0).
	int tempMajor;
	int tempMinor;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &tempMajor);
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &tempMinor);

	std::cout << ">> OpenGL Context Created Successfully!" << std::endl;
	std::cout << ">> GL Context Version: " << tempMajor << "." << tempMinor << std::endl;
}
void Engine::CheckForInputDevices(void) {
	std::cout << ">> Checking for Input Devices - Begun" << std::endl;
	// Initialise the keyboard instance
	deviceKeyboard = new Keyboard();

	// Search for any Keyboards.
	int numberOfConnectedControllers = SDL_NumJoysticks();
	if (numberOfConnectedControllers < 1) {
		//deviceGameController = nullptr;
		std::cout << ">> No Controllers were found..." << std::endl;
	}
	else {
		for (int i = 0; i < numberOfConnectedControllers; i++) {
			if (i < maxNumberOfControllers) {
				deviceGameControllerRegister.push_back(new GameController(SDL_GameControllerOpen(i)));
				if (deviceGameControllerRegister[i]->GetSDLHook() != NULL) {
					std::cout << ">> Game controller found: Controller 0 has been opened for input" << std::endl;
				}
				else {
					std::cout << ">> ERROR: Unable to Open game controller for use! SDL Error: " << SDL_GetError() << std::endl;
				}
			}
		}

	}
	std::cout << ">> Checking for Input Devices - Complete" << std::endl;
}
void Engine::InitialiseGlew(void) {
	GLenum rev;
	glewExperimental = GL_TRUE;
	rev = glewInit();
	if (GLEW_OK != rev) {
		// If GLEW fails, close the program.
		std::cout << ">> GLEW Error: " << glewGetErrorString(rev) << std::endl;
		this->Close();
	}
	else {
		std::cout << ">> GLEW Initialisation Successfully!" << std::endl;
	}
}
void Engine::LoadShaders(void) {
	// Load the Shaders
	std::cout << ">> Loading Shaders - Begun" << std::endl;
	std::vector<std::string> listOfShaders = FileSystemUtilities::GetFileList(contentDirectory + "config/shaders");
	for (size_t i = 0; i < listOfShaders.size(); i++) {
		LuaScript currentShaderConfig = LuaScript(listOfShaders[i]);
		if (currentShaderConfig.isScriptLoaded) {
			// Grab the variables from the config script
			std::string vertexShaderName = contentDirectory + "shaders/" + currentShaderConfig.Get<std::string>("shader_config.vertex");
			std::string fragmentShaderName = contentDirectory + "shaders/" + currentShaderConfig.Get<std::string>("shader_config.fragment");
			std::string shaderName = currentShaderConfig.Get<std::string>("shader_config.name");

			// Create and Load the shader
			Shader* newShader = new Shader(shaderName, vertexShaderName, fragmentShaderName);
			if (newShader->Load()) {
				// Loaded successfully, storing it for use
				shaderRegister.push_back(newShader);
			}
			else {
				std::cout << ">> Shader " << shaderName << " failed to load." << std::endl;
				this->Close();
			}
		}
	}
	std::cout << "\n>> Loading Shaders - Complete" << std::endl;
}
void Engine::LoadGraphicsEnvironment(void) {
	// Setup the Engine environment (E.g. OpenGL, SDL, etc.)
	std::cout << ">> Environment Setup - Begun" << std::endl;
	// SDL setup
	InitialiseSDL();
	CreateSDLWindow();
	CreateSDLContext();
	CheckForInputDevices();		// Finds all the input devices attached to the machine

	// OpenGL setup
	InitialiseGlew();
	LoadShaders();
	glViewport(0, 0, windowDimensions.x, windowDimensions.y);
	SDL_GL_SwapWindow(sdlWindow);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
	std::cout << ">> Environment Setup - Complete" << std::endl;
}
void Engine::CleanUp(void) {
	std::cout << "Cleanup - Begun" << std::endl;

	// Delete all the levels.
	int levelRegisterSize = levelRegister.size();
	for (int i = 0; i < levelRegisterSize; i++) {
		delete levelRegister[i];
	}

	// Delete all the items loaded.
	int itemRegisterSize = itemRegister.size();
	for (int i = 0; i < itemRegisterSize; i++) {
		delete itemRegister[i];
	}

	// Delete the camera
	if (camera != nullptr) {
		delete camera;
	}

	// Delete all the Entities.
	if (player != nullptr) {
		delete player;
	}
	int entityRegisterSize = entityRegister.size();
	for (int i = 0; i < entityRegisterSize; i++) {
		delete entityRegister[i];
	}

	// Delete all the Input Devices.
	if (deviceKeyboard != nullptr) {
		delete deviceKeyboard;
	}
	int gameControllerRegisterSize = deviceGameControllerRegister.size();
	for (int i = 0; i < gameControllerRegisterSize; i++) {
		delete deviceGameControllerRegister[i];
	}

	// Delete the loaded Shaders.
	int shaderRegisterSize = shaderRegister.size();
	for (int i = 0; i < shaderRegisterSize; i++) {
		delete shaderRegister[i];
	}

	if (SDL_NumJoysticks() > 0) {
		for (int i = 0; i < deviceGameControllerRegister.size(); i++) {
			SDL_GameControllerClose(deviceGameControllerRegister[i]->GetSDLHook());	// Close the controller.
		}
	}
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
// Content loading related functions
void Engine::ImportTexture(const char* texturePath) {
	// This function loads a texture into memory to be used with a source rectangle to depict what part of it to render.
	if (texturePath != "EMPTY") {
		//int init = IMG_Init(IMG_INIT_PNG);
		SDL_Surface* image = IMG_Load(texturePath);	// Try and load the texture.
		if (image == NULL) {
			// If the texture was not loaded correctly, quit the program and show a error message on the console.
			std::cout << ">> The loading of Spritesheet: " << texturePath << " failed." << std::endl;
			this->Close();
		}
		else {
			std::cout << ">> The loading of Spritesheet: " << texturePath << " was successful." << std::endl;
		}

		Texture tempTexture = Texture(texturePath);
		glGenTextures(1, &tempTexture.id);				// Generate a texture ID and store it
		glBindTexture(GL_TEXTURE_2D, tempTexture.id);

		// Set the texturing variables for this texture.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

		tempTexture.dimensions = glm::vec2(image->w, image->h);
		// Setup the texture.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		textureRegister.push_back(tempTexture);
		SDL_FreeSurface(image);
	}
}
void Engine::LoadTextures(void) {
	std::cout << ">> Loading Textures - Begun" << std::endl;
	std::vector<std::string> listOfTextures = FileSystemUtilities::GetFileList(contentDirectory + "textures");
	for (size_t i = 0; i < listOfTextures.size(); i++) {
		ImportTexture(listOfTextures[i].c_str());
	}

	// Find the default texture for when textures are failed to be found.
	for (size_t i = 0; i < textureRegister.size(); i++) {
		if (textureRegister[i].name.find(nameOfDefaultTexture) != std::string::npos) {
			indexOfDefaultTexture = i;
		}
	}
	if (indexOfDefaultTexture == -1) {
		this->Close();
	}

	std::cout << ">> Loading Textures - Complete" << std::endl;
}
Model Engine::LoadModel(const std::string& modelPath) {
	// Load in a Model.
	if (modelPath != "") {
		Assimp::Importer importer;	// An importer for importing the model data.
		const aiScene* scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_GenNormals);		// Read the Model file.

		Model currentModel = Model(modelPath);

		// Loop through each mesh in the loaded model.
		for (int i = 0; i < scene->mNumMeshes; i++) {
			Model::Mesh currentMesh = Model::Mesh();

			for (int j = 0; j < scene->mMeshes[i]->mNumVertices; j++) {
				// Get the vertices from assimp.
				currentMesh.vertexPositions.push_back(glm::vec3(scene->mMeshes[i]->mVertices[j].x, scene->mMeshes[i]->mVertices[j].y, scene->mMeshes[i]->mVertices[j].z));

				// If the mesh is setup for texturing, get all the textures from assimp.
				if (scene->mMeshes[i]->mTextureCoords[0] != NULL) {
					currentMesh.isSetupForTextures = true;
					currentMesh.uvs.push_back(glm::vec2(scene->mMeshes[i]->mTextureCoords[0][j].x, scene->mMeshes[i]->mTextureCoords[0][j].y));
				}

				// Populate the colour buffer with a default value.
				currentMesh.colourData.push_back(glm::vec3(1.0f, 1.0f, 1.0f));

				// Check if the current mesh has surface normals setup ready for lighting.
				if (scene->mMeshes[i]->mNormals != NULL) {
					currentMesh.surfaceNormals.push_back(glm::vec3(scene->mMeshes[i]->mNormals[j].x, scene->mMeshes[i]->mNormals[j].y, scene->mMeshes[i]->mNormals[j].z));
				}
			}
			// Loops through the array of indices, pushing each of them to the index vector individually, this is because they are used individually whilst drawing instead of in 3s.
			for (int j = 0; j < scene->mMeshes[i]->mNumFaces; j++) {
				currentMesh.indices.push_back(scene->mMeshes[i]->mFaces[j].mIndices[0]);
				currentMesh.indices.push_back(scene->mMeshes[i]->mFaces[j].mIndices[1]);
				currentMesh.indices.push_back(scene->mMeshes[i]->mFaces[j].mIndices[2]);
			}
			currentMesh.BindBuffers();
			currentModel.meshes.push_back(currentMesh);
		}
		std::cout << ">> Model loaded: " << modelPath << std::endl;	// Outputs that the model has been loaded.
		importer.FreeScene();						// Cleans up the loader.
		return currentModel;
	}
}
void Engine::LoadModels(void) {
	std::cout << ">> Loading Models - Begun" << std::endl;
	std::vector<std::string> listOfModels = FileSystemUtilities::GetFileList(contentDirectory + "models");
	for (size_t i = 0; i < listOfModels.size(); i++) {
		modelRegister.push_back(LoadModel(listOfModels[i]));
	}

	// Find the default model
	for (int i = 0; i < modelRegister.size(); i++) {
		if (modelRegister[i].name.find(nameOfTileModel) != std::string::npos) {
			indexOfTileModel = i;
		}

		if (modelRegister[i].name.find(nameOfSpriteModel) != std::string::npos) {
			indexOfSpriteModel = i;
		}
	}
	if (indexOfTileModel == -1) {
		this->Close();
	}
	if (indexOfSpriteModel == -1) {
		indexOfSpriteModel = indexOfTileModel;
	}

	std::cout << ">> Loading Models - Complete" << std::endl;
}
void Engine::LoadTilesets(void) {
	// Load the Tilesets
	std::cout << ">> Loading Tilesets - Begun" << std::endl;
	std::vector<std::string> listOfTilesets = FileSystemUtilities::GetFileList(contentDirectory + "tilesets");
	for (size_t i = 0; i < listOfTilesets.size(); i++) {
		Tileset tileSet = Tileset();
		tileSet.Load(listOfTilesets[i]);
		tilesetRegister.push_back(tileSet);
	}
	std::cout << ">> Loading Tilesets - Complete" << std::endl;
}
void Engine::LoadLevels(void) {
	// Load each of the Level scripts.
	std::cout << ">> Loading Levels - Begun" << std::endl;
	std::vector<std::string> listOfLevelFiles = FileSystemUtilities::GetFileList(contentDirectory + "levels");
	for (size_t i = 0; i < listOfLevelFiles.size(); i++) {
		levelRegister.push_back(new Level(listOfLevelFiles[i]));
	}
	indexCurrentLevel = 0;
	std::cout << ">> Loading Levels - Complete" << std::endl;
}
void Engine::LoadItems(void) {
	std::cout << ">> Loading Items - Begun" << std::endl;

	std::cout << ">> Loading Items - Complete" << std::endl;
}
void Engine::LoadPlayer(void) {
	// Load the Player
	std::cout << ">> Loading Player - Begun" << std::endl;
	player = new Player(contentDirectory + "scripts/entities/entity_player.lua");
	std::cout << ">> Loading Player - Complete" << std::endl;
}
void Engine::LoadEntities(void) {
	// Load the Entities
	std::cout << ">> Loading Entities - Begun" << std::endl;
	std::vector<std::string> listOfEntityFiles = FileSystemUtilities::GetFileList(contentDirectory + "scripts/entities");
	for (size_t i = 0; i < listOfEntityFiles.size(); i++) {

	}
	std::cout << ">> Loading Entities - Complete" << std::endl;
}
void Engine::LoadCameras(void) {
	std::cout << ">> Loading Camera - Begun" << std::endl;
	camera = new Camera(glm::vec3(0.0f, 0.0f, 1.0f));
	std::cout << ">> Loading Camera - Complete" << std::endl;
}
void Engine::Load(void) {
	std::cout << "Engine Loading - Begun" << std::endl;
	// Load Engine Core
	LoadEngineConfig();
	LoadKeyBindings();
	LoadEnginePointers();
	LoadGraphicsEnvironment();

	// Load Game Content
	LoadModels();
	LoadTextures();
	LoadTilesets();
	LoadLevels();
	LoadItems();
	LoadPlayer();
	LoadEntities();
	LoadCameras();

	// Load complete
	isRunning = true;	// Allows the game loop to run
	std::cout << "Engine Loading - Complete" << std::endl;
}
// Game loop related functions
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
	if (levelRegister[indexCurrentLevel] != nullptr) {
		levelRegister[indexCurrentLevel]->Update(deltaTime);
	}
	if (player != nullptr) {
		player->Update(deltaTime);
		camera->Update(deltaTime, *player);
	}

	if (deviceKeyboard->GetKeyState(Keyboard::y)) {
		windowScaler = glm::vec2(1.0f);
		windowDimensions = (tileSize * windowGridSize) * windowScaler;
		SDL_SetWindowSize(sdlWindow, windowDimensions.x, windowDimensions.y);
	}
	if (deviceKeyboard->GetKeyState(Keyboard::u)) {
		windowScaler = glm::vec2(2.0f);
		windowDimensions = (tileSize * windowGridSize) * windowScaler;
		SDL_SetWindowSize(sdlWindow, windowDimensions.x, windowDimensions.y);
	}
	if (deviceKeyboard->GetKeyState(Keyboard::i)) {
		windowScaler = glm::vec2(3.0f);
		windowDimensions = (tileSize * windowGridSize) * windowScaler;
		SDL_SetWindowSize(sdlWindow, windowDimensions.x, windowDimensions.y);
	}
}
void Engine::Render(void) {
	// Pre-render
	glViewport(0, 0, windowDimensions.x, windowDimensions.y);
	glClearColor(100 / 255.0f, 149 / 255.0f, 237 / 255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw the level
	if (levelRegister[indexCurrentLevel] != nullptr) {
		levelRegister[indexCurrentLevel]->Draw();
	}
	// Draw the player
	if (player != nullptr) {
		player->Draw();
	}

	// Post-Render
	SDL_GL_SwapWindow(sdlWindow);	// Gives the frame buffer to the display (swapBuffers).
}
// Core Engine function
void Engine::Run(void) {
	Load();		// Loads all the configs, the game content and initialises everything needed by the engine to run.
	std::cout << "Game Runtime - Begun" << std::endl;

	// FPS variables
	int fpsCounter = 0;
	float secondCounter = 0;

	while (isRunning) {
		// DeltaTime math
		currentFrameTime = SDL_GetTicks();
		float deltaTime = ((currentFrameTime - oldFrameTime) / 1000);

		// Main Game loop
		EventHandler();			// Handle any events
		Update(deltaTime);		// Update the game
		Render();				// Render the game

		// FPS math, called after the engine has finished its render function.
		fpsCounter++;					// Counts the number of frames as they've been rendered.
		secondCounter += deltaTime;		// Counts up to the next second
		if (secondCounter >= 1) {
			// If it has been a second since the last FPS count, reset the counter and print.
			std::string fpsDisplay = windowTitle + " - FPS: " + std::to_string(fpsCounter);
			SDL_SetWindowTitle(sdlWindow, fpsDisplay.c_str());
			fpsCounter = 0;
			secondCounter = 0;
		}

		oldFrameTime = currentFrameTime;
	}
	std::cout << "Game Runtime - Finished" << std::endl;
	this->Close(true);
}
// Engine Utilities
glm::vec2 Engine::ConvertToGridPosition(const glm::vec2 & position) {
	return glm::vec2((int)(position.x / this->tileSize.x), (int)(position.y / this->tileSize.y));
}
