#include "Engine.h"

Engine::Engine(char* gameName) {
	exeName = gameName;
	windowSize = glm::vec2(MINIMUM_WINDOW_SIZE_WIDTH, MINIMUM_WINDOW_SIZE_HEIGHT);
	aspectRatio = (windowSize.x / windowSize.y);
	fieldOfView = glm::radians(45.0f);
	oldFrameTime = 0.0f;
	currentFrameTime = 0.0f;
	InitialiseWorldCamera();
}
Engine::~Engine() {
	delete player;

	for (int i = 0; i < tileRegister.size(); i++) {
		delete tileRegister[i];
	}

	for (int i = 0; i < entityRegister.size(); i++) {
		delete entityRegister[i];
	}
	std::cout << "Game Class Deconstructor Successful!" << std::endl;
}

void Engine::Run(void) {
	SetupEnvironment();				// Does all the setup function calls for OpenGL, SDL and glew.
	std::cout << ">> Game runtime started" << std::endl;

	while (isRunning) {
		currentFrameTime = SDL_GetTicks();
		float deltaTime = ((currentFrameTime - oldFrameTime) / 1000);

		EventHandler();
		Update(deltaTime);
		Renderer();

		oldFrameTime = currentFrameTime;
	}

	std::cout << ">> Game runtime finished" << std::endl;
	CleanupSDL();					// Cleans up after SDL.
	SDL_Quit();							// Quits the program.
}
void Engine::InitialiseSDL(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
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
	sdlWindow = SDL_CreateWindow(exeNameCStr, 100, 100, windowSize.x, windowSize.y, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

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
		shaderList.push_back(Shader("../Shaders/BaseVertex.vert", GL_VERTEX_SHADER));
		shaderList.push_back(Shader("../Shaders/BaseFragment.frag", GL_FRAGMENT_SHADER));
	}

	glProgram = CreateGLProgram(shaderList);	// Create the Program.
	if (glProgram == 0) {
		// If the program failed to be created, print the error and close the program.
		std::cout << "GLSL program creation error." << std::endl;
		SDL_Quit();
		exit(1);
	}
	else {
		// If the program was created successfully.
		std::cout << "GLSL program created Successfully! The GLUint is: " << glProgram << std::endl;
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
		shaderPointers.modelMatrixUniform == -1 || shaderPointers.viewMatrixUniform == -1 || shaderPointers.projectionMatrixUniform == -1 ) {
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
	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++) {
		glDeleteShader(shaderList[iLoop].shaderID);
	}
}
Model Engine::LoadModel(const std::string& modelPath) {
	// Load in a Model.
	if (modelPath != "") {
		Assimp::Importer importer;	// An importer for importing the model data.
		const aiScene* scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_GenNormals);		// Read the Model file.

		Model currentModel = Model(*this, modelPath);
		
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
		std::cout << "Model loaded: " << modelPath << std::endl;	// Outputs that the model has been loaded.
		importer.FreeScene();						// Cleans up the loader.
		return currentModel;
	}
}
void Engine::LoadLevels(void) {

}
void Engine::LoadContent(void) {
	// Load the Model Register
	modelRegister.push_back(LoadModel("../Content/Models/tile.obj"));

	// Loop through the model register and setup the Vertex Objects for every model for their default states.
	for (int i = 0; i < modelRegister.size(); i++) {
		modelRegister[i].SetVertexObjects();
	}

	// Load the Tile Register

	// Load the Level Register

	// Load the player
	player = new Player(*this, modelRegister[0], glm::vec3(16.0f, 32.0f, 0.0f), "../Content/Textures/placeholder.png");

	// Load the Entity Register

	// Load the Item Register

}
void Engine::SetupEnvironment(void) {
	std::cout << ">> Setting up Environment..." << std::endl;
	InitialiseSDL();
	CreateSDLWindow();
	CreateSDLContext();
	InitialiseGlew();
	glViewport(0, 0, windowSize.x, windowSize.y);
	SDL_GL_SwapWindow(sdlWindow);
	InitialiseProgram();
	std::cout << ">> Setup Complete" << std::endl;
	LoadContent();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
}
void Engine::CleanupSDL(void) {
	std::cout << ">> Starting Cleanup..." << std::endl;
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(sdlWindow);
	std::cout << ">> Cleanup Successful" << std::endl;
	std::cout << ">> Cleanup complete" << std::endl;
}
void Engine::InitialiseWorldCamera(void) {
	camera = Camera(*this, glm::vec3(0.0f, 0.0f, 0.1f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	camera.projectionMatrix = glm::ortho(0.0f, windowSize.x, windowSize.y, 0.0f);
}
void Engine::EventHandler(void) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type)	 {
		case SDL_WINDOWEVENT: 
			Event_Window(event.window);
			break;
		case SDL_QUIT:
			isRunning = false;
			break; 
		case SDL_KEYDOWN:			// Key down handling.
			Event_KeyDown(event.key);
			break;
		case SDL_KEYUP:				// Key up handling.
			Event_KeyUp(event.key);
			break;
		}
	}
}
void Engine::Event_Window(const SDL_WindowEvent& windowEvent) {
	switch (windowEvent.event) {
	case SDL_WINDOWEVENT_RESIZED:
		// Resize logic.
		windowSize = glm::vec2(windowEvent.data1, windowEvent.data2);
		break;
	default:
		break;
	}
}
void Engine::Event_KeyDown(const SDL_KeyboardEvent& keyboardEvent) {
	if (!keyboardEvent.repeat) {
		switch (keyboardEvent.keysym.sym) {
		case SDLK_ESCAPE:
			isRunning = false;
			break;
		default:
			break;
		}
	}
}
void Engine::Event_KeyUp(const SDL_KeyboardEvent& keyboardEvent) {
	if (!keyboardEvent.repeat) {
		switch (keyboardEvent.keysym.sym) {

		default:
			break;
		}
	}
}
void Engine::Update(float deltaTime) {
	if (player != nullptr) {
		player->Update(deltaTime);
	}
}
void Engine::Draw(void) {
	// Draw the level


	// Draw the player
	if (player != nullptr) {
		player->Draw();
	}

	// Draw the NPCs
}
void Engine::Renderer(void) {
	// Pre-render
	glViewport(0, 0, windowSize.x, windowSize.y);		// Sets view port.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);							// Sets the colour of the cleared buffer colour.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// Clear the color buffer and depth buffer.

	// Main Render
	glUseProgram(glProgram);
	glUniformMatrix4fv(shaderPointers.viewMatrixUniform, 1, GL_FALSE, glm::value_ptr(camera.viewMatrix));				// Pass the viewMatrix to the Shader.
	glUniformMatrix4fv(shaderPointers.projectionMatrixUniform, 1, GL_FALSE, glm::value_ptr(camera.projectionMatrix));	// Pass the projectionMatrix to the Shader.
	Draw();	// Does the actual drawing of the GameObjects, this is seperated to make it easier to read.
	glUseProgram(0);

	// Post-Render
	SDL_GL_SwapWindow(sdlWindow);	// Gives the frame buffer to the display (swapBuffers).
}