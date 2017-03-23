#include "Engine.h"

Engine::Engine(char* gameName) {
	exeName = gameName;
	oldFrameTime = 0.0f;
	currentFrameTime = 0.0f;

	contentDirectory = "content/";
	thumbStickDeadZone = 8000;
	triggerDeadZone = 8000;
	pressedStateFlag = 1;
	indexCurrentLevel = -1;
	indexOfDefaultTexture = -1;
}
Engine::~Engine() {
	delete deviceKeyboard;
	delete deviceGameController;
	delete player;

	// Delete all the entries in the registers.
	for (int i = 0; i < itemRegister.size(); i++) {
		delete itemRegister[i];
	}

	for (int i = 0; i < entityRegister.size(); i++) {
		delete entityRegister[i];
	}

	for (int i = 0; i < levelRegister.size(); i++) {
		delete levelRegister[i];
	}
	std::cout << "Game Class Deconstructor Successful!" << std::endl;
}

// Engine config related functions
void Engine::LoadEngineConfig() {
	LuaScript configScript = LuaScript(contentDirectory + "config/engine_config.lua");
	if (configScript.isScriptLoaded) {
		tileSize = glm::vec2(configScript.Get<int>("config.tile_size.x"), configScript.Get<int>("config.tile_size.y"));
		windowGridSize = glm::vec2(configScript.Get<int>("config.window_grid.x"), configScript.Get<int>("config.window_grid.y"));
		windowScaler = glm::vec2(configScript.Get<int>("config.window_scale.x"), configScript.Get<int>("config.window_scale.y"));
		windowDimensions = (tileSize * windowGridSize) * windowScaler;
	}
	else {
		// Config failed to load.
		SDL_Quit();
		exit(1);
	}
}
void Engine::LoadKeyBindings() {
	LuaScript keybindScript = LuaScript(contentDirectory + "config/key_bindings.lua");
	if (keybindScript.isScriptLoaded) {
		keybindMovementUp = (Keyboard::Keys)keybindScript.Get<int>("keybindings.movement_up");
		keybindMovementDown = (Keyboard::Keys)keybindScript.Get<int>("keybindings.movement_down");
		keybindMovementLeft = (Keyboard::Keys)keybindScript.Get<int>("keybindings.movement_left");
		keybindMovementRight = (Keyboard::Keys)keybindScript.Get<int>("keybindings.movement_right");
	}
	else {
		// Config failed to load.
		SDL_Quit();
		exit(1);
	}
}
void Engine::SetEnginePointers(void) {
	std::cout << ">> Setting static pointers - Begun" << std::endl;
	Model::Engine_Pointer = this;
	Keyboard::Engine_Pointer = this;
	GameController::Engine_Pointer = this;
	BoundingBox::Engine_Pointer = this;
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
	SDL_GL_SetSwapInterval(-1);
	std::cout << ">> SDL initialised Successfully!" << std::endl;
}
void Engine::CreateSDLWindow(void) {
	int beginIdxWindows = exeName.rfind("\\");		// Find last occurrence of a backslash
	int beginIdxLinux = exeName.rfind("/");			// Find last occurrence of a forward slash
	int beginIdx = std::max(beginIdxWindows, beginIdxLinux);
	std::string exeNameEnd = exeName.substr(beginIdx + 1);
	const char *exeNameCStr = exeNameEnd.c_str();

	// Create window
	sdlWindow = SDL_CreateWindow(exeNameCStr, 500, 500, windowDimensions.x, windowDimensions.y, SDL_WINDOW_OPENGL);

	// Error handling for the SDL Window.
	if (sdlWindow == nullptr) {
		std::cout << ">> SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		exit(1);
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
		SDL_Quit();
		exit(1);
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
	if (SDL_NumJoysticks() < 1) {
		deviceGameController = nullptr;
		std::cout << ">> No Controllers were found..." << std::endl;
	}
	else {
		// Active the first game controller.
		deviceGameController = new GameController(SDL_GameControllerOpen(0));
		if (deviceGameController->GetSDLHook() != NULL) {
			std::cout << ">> Game controller found: Controller 0 has been opened for input" << std::endl;
		}
		else {
			std::cout << ">> ERROR: Unable to Open game controller for use! SDL Error: " << SDL_GetError() << std::endl;
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
		SDL_Quit();
		exit(1);
	}
	else {
		std::cout << ">> GLEW Initialisation Successfully!" << std::endl;
	}
}
GLuint Engine::CreateGLProgram(const std::vector<Shader>& shaderList) {
	// Create the OpenGL program and attach the shaders.
	GLuint program = glCreateProgram();

	// Loop through the Shader List and attach each one to the program.
	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++) {
		glAttachShader(program, shaderList[iLoop].shaderID);
	}

	glLinkProgram(program);	// Link the program object to the specific program.

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);

	if (status == GL_FALSE) {
		// If the program or shaders fail, print the errors.
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	// Loop through the Shader list and detach each one.
	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++) {
		glDetachShader(program, shaderList[iLoop].shaderID);
	}

	return program;
}
void Engine::InitialiseProgram(void) {
	std::vector<Shader> shaderList;
	int openGLMajorVersion;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &openGLMajorVersion);
	if (openGLMajorVersion == 3) {
		// Need to convert this later to use a script to dynamically load shaders.
		shaderList.push_back(Shader("content/shaders/default.vert", GL_VERTEX_SHADER));
		shaderList.push_back(Shader("content/shaders/default.frag", GL_FRAGMENT_SHADER));
	}

	glProgram = CreateGLProgram(shaderList);	// Create the Program.
	if (glProgram == 0) {
		// If the program failed to be created, print the error and close the program.
		std::cout << ">> GLSL program creation error." << std::endl;
		SDL_Quit();
		exit(1);
	}
	else {
		// If the program was created successfully.
		std::cout << ">> GLSL program created Successfully! The GLUint is: " << glProgram << std::endl;
	}

	// Get the Vertex Shader Pointers
	shaderPointers.vertexPositionAttrib = glGetAttribLocation(glProgram, "vertexPosition");
	shaderPointers.colourAttrib = glGetAttribLocation(glProgram, "vertexColor");
	shaderPointers.uvAttrib = glGetAttribLocation(glProgram, "vertexUV");
	shaderPointers.modelMatrixUniform = glGetUniformLocation(glProgram, "modelMatrix");
	shaderPointers.viewMatrixUniform = glGetUniformLocation(glProgram, "viewMatrix");
	shaderPointers.projectionMatrixUniform = glGetUniformLocation(glProgram, "projectionMatrix");
	// Get the Fragment Shader Pointers
	shaderPointers.textureSamplerUniform = glGetUniformLocation(glProgram, "textureSampler");
	shaderPointers.hasTextureUniform = glGetUniformLocation(glProgram, "hasTexture");
	shaderPointers.textureDimensionsUniform = glGetUniformLocation(glProgram, "textureDimensions");
	shaderPointers.sourceFrameSizeUniform = glGetUniformLocation(glProgram, "sourceFrameSize");
	shaderPointers.sourceFramePositionUniform = glGetUniformLocation(glProgram, "sourceFramePosition");


	// Check if any of the Vertex Shader Pointers were not set.
	if (shaderPointers.vertexPositionAttrib == -1 || shaderPointers.colourAttrib == -1 || shaderPointers.uvAttrib == -1 ||
		shaderPointers.modelMatrixUniform == -1 || shaderPointers.viewMatrixUniform == -1 || shaderPointers.projectionMatrixUniform == -1) {
		std::cout << "Error assigning Vertex Shader Pointers" << std::endl;
		std::cout << "vertexPositionAttrib: " << shaderPointers.vertexPositionAttrib << std::endl;
		std::cout << "colourAttrib: " << shaderPointers.colourAttrib << std::endl;
		std::cout << "uvLocation: " << shaderPointers.uvAttrib << std::endl;
		std::cout << "modelMatrixLocation: " << shaderPointers.modelMatrixUniform << std::endl;
		std::cout << "viewMatrixLocation: " << shaderPointers.viewMatrixUniform << std::endl;
	}

	// Check if any of the Fragment Shader Pointers were not set.
	if (shaderPointers.textureSamplerUniform == -1 || shaderPointers.hasTextureUniform == -1 || shaderPointers.textureDimensionsUniform == -1 ||
		shaderPointers.sourceFrameSizeUniform == -1 || shaderPointers.sourceFramePositionUniform == -1) {
		std::cout << "Error assigning Fragment Shader Pointers" << std::endl;
		std::cout << "textureSamplerLocation: " << shaderPointers.textureSamplerUniform << std::endl;
		std::cout << "hasTextureUniform: " << shaderPointers.hasTextureUniform << std::endl;
		std::cout << "textureDimensions: " << shaderPointers.textureDimensionsUniform << std::endl;
		std::cout << "sourceFrameSize: " << shaderPointers.sourceFrameSizeUniform << std::endl;
		std::cout << "sourceFramePosition: " << shaderPointers.sourceFramePositionUniform << std::endl;
	}

	// Clean up shaders, they aren't needed anymore as they are loaded into the program.
	for (size_t i = 0; i < shaderList.size(); i++) {
		glDeleteShader(shaderList[i].shaderID);
	}
}
void Engine::SetupEnvironment(void) {
	std::cout << ">> Environment Setup - Begun" << std::endl;
	// SDL setup
	InitialiseSDL();
	CreateSDLWindow();
	CreateSDLContext();
	CheckForInputDevices();		// Finds all the input devices attached to the machine

	// OpenGL setup
	InitialiseGlew();
	glViewport(0, 0, windowDimensions.x, windowDimensions.y);
	SDL_GL_SwapWindow(sdlWindow);
	InitialiseProgram();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
	std::cout << ">> Environment Setup - Complete" << std::endl;
}
void Engine::CleanUp(void) {
	std::cout << "Cleanup - Begun" << std::endl;
	if (SDL_NumJoysticks() > 0)	{
		SDL_GameControllerClose(deviceGameController->GetSDLHook());	// Close the controller.
	}
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(sdlWindow);
	std::cout << "Cleanup - Complete" << std::endl;
}
// Content loading related functions
void Engine::ImportTexture(const char * texturePath) {
	// This function loads a texture into memory to be used with a source rectangle to depict what part of it to render.
	if (texturePath != "EMPTY") {
		//int init = IMG_Init(IMG_INIT_PNG);
		SDL_Surface* image = IMG_Load(texturePath);	// Try and load the texture.
		if (image == NULL) {
			// If the texture was not loaded correctly, quit the program and show a error message on the console.
			std::cout << ">> The loading of Spritesheet: " << texturePath << " failed." << std::endl;
			SDL_Quit();
			exit(1);
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
		if (textureRegister[i].name.find("default.png") != std::string::npos) {
			indexOfDefaultTexture = i;
		}
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
	// Loop through the model register and setup the Vertex Objects for every model for their default states.
	for (int i = 0; i < modelRegister.size(); i++) {
		modelRegister[i].SetVertexObjects();
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
	LuaScript playerScript = LuaScript(contentDirectory + "scripts/entities/entity_player.lua");
	if (playerScript.isScriptLoaded) {
		glm::vec3 playerPosition = glm::vec3(playerScript.Get<float>("player.position.x"), playerScript.Get<float>("player.position.y"), playerScript.Get<float>("player.position.z"));

		// Find the Texture for the Player.
		int indexOfPlayerTexture = -1;
		std::string playerTextureName = playerScript.Get<std::string>("player.texture");
		for (size_t i = 0; i < textureRegister.size(); i++) {
			if (textureRegister[i].name.find(playerTextureName) != std::string::npos) {
				indexOfPlayerTexture = i;
			}
		}

		// Initialise the Player and use the appropriate texture depending on if the declared texture was found.
		if (indexOfPlayerTexture != -1) {
			player = new Player(modelRegister[0], textureRegister[indexOfPlayerTexture], playerPosition);
		}
		else {
			player = new Player(modelRegister[0], textureRegister[indexOfDefaultTexture], playerPosition);
		}
	}
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
	camera = Camera(*this, glm::vec3(0.0f, 0.0f, 0.1f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::ortho(0.0f, windowDimensions.x, windowDimensions.y, 0.0f));
	std::cout << ">> Loading Camera - Complete" << std::endl;
}
void Engine::Load(void) {
	std::cout << "Engine Loading - Begun" << std::endl;
	LoadEngineConfig();			// Loads the main config file for the engine
	LoadKeyBindings();			// Loads the Keybinding config file
	SetEnginePointers();		// Sets the Engine_Pointer static of each of the Engines classes.
	SetupEnvironment();			// Setup the Engine environment (E.g. OpenGL, SDL, etc.)

	// Load all the game Content
	LoadTextures();
	LoadModels();
	LoadTilesets();
	LoadLevels();
	LoadItems();
	LoadPlayer();
	LoadEntities();
	LoadCameras();

	isRunning = true;	// Successfully loaded the engine, allow the game loop to run
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
				deviceGameController->UpdateThumbSticks();
				break;
			case SDL_CONTROLLERBUTTONDOWN:
				deviceGameController->UpdateButtonStates();
				break;
			case SDL_CONTROLLERBUTTONUP:
				deviceGameController->UpdateButtonStates();
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
	}
}
void Engine::Draw(void) {
	// Draw the level
	if (levelRegister[indexCurrentLevel] != nullptr) {
		levelRegister[indexCurrentLevel]->Draw();
	}
	// Draw the player
	if (player != nullptr) {
		player->Draw();
	}
}
void Engine::Renderer(void) {
	// Pre-render
	glViewport(0, 0, windowDimensions.x, windowDimensions.y);
	glClearColor(100 / 255.0f, 149 / 255.0f, 237 / 255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Main Render
	glUseProgram(glProgram);
	glUniformMatrix4fv(shaderPointers.viewMatrixUniform, 1, GL_FALSE, glm::value_ptr(camera.viewMatrix));				// Pass the view matrix and projection matrix to the Shader.
	glUniformMatrix4fv(shaderPointers.projectionMatrixUniform, 1, GL_FALSE, glm::value_ptr(camera.projectionMatrix));
	Draw();	// Does the actual drawing of the GameObjects, this is seperated to make it easier to read.
	glUseProgram(0);

	// Post-Render
	SDL_GL_SwapWindow(sdlWindow);	// Gives the frame buffer to the display (swapBuffers).
}
// Core Engine function
void Engine::Run(void) {
	Load();		// Loads all the configs, the game content and initialises everything needed by the engine to run.
	std::cout << "Game Runtime - Begun" << std::endl;
	while (isRunning) {
		currentFrameTime = SDL_GetTicks();
		float deltaTime = ((currentFrameTime - oldFrameTime) / 1000);

		// Main Game loop
		EventHandler();			// Handle any events
		Update(deltaTime);		// Update the game
		Renderer();				// Render the game

		oldFrameTime = currentFrameTime;
	}
	std::cout << "Game Runtime - Finished" << std::endl;
	CleanUp();					// Cleans up after SDL
	SDL_Quit();						// Quits the program
}
// Engine Utilities
glm::vec2 Engine::ConvertToGridPosition(const glm::vec2 & position) {
	return glm::vec2((int)(position.x / this->tileSize.x), (int)(position.y / this->tileSize.y));
}
