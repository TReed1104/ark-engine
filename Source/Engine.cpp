#include "Engine.h"

Engine::Engine(char* gameName, glm::vec2 initialWindowSize) {
	exeName = gameName;
	windowSize = initialWindowSize;
	aspectRatio = (windowSize.x / windowSize.y);
	fieldOfView = glm::radians(45.0f);
	oldFrameTime = 0.0f;
	currentFrameTime = 0.0f;
}
Engine::~Engine() {

}

void Engine::Run(void) {
	SetupEnvironment();				// Does all the setup function calls for OpenGL, SDL and glew.
	std::cout << ">> Game runtime started" << std::endl;

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

	// Get the location pointer for the attributes.
	// Vertex Shader locations.
	vertexPositionAttrib = glGetAttribLocation(glProgram, "vertexPosition");
	colourAttrib = glGetAttribLocation(glProgram, "vertexColor");
	uvAttrib = glGetAttribLocation(glProgram, "vertexUV");
	// Fragment Shader locations.
	modelMatrixUniform = glGetUniformLocation(glProgram, "modelMatrix");
	viewMatrixUniform = glGetUniformLocation(glProgram, "viewMatrix");
	projectionMatrixUniform = glGetUniformLocation(glProgram, "projectionMatrix");
	textureSamplerUniform = glGetUniformLocation(glProgram, "textureSampler");
	hasTextureUniform = glGetUniformLocation(glProgram, "hasTexture");

	// If any of the shaderlocations have failed to be found, print the value of each for debugging.
	if (vertexPositionAttrib == -1 || colourAttrib == -1 || uvAttrib == -1 || modelMatrixUniform == -1 || viewMatrixUniform == -1 ||
		projectionMatrixUniform == -1 ||textureSamplerUniform == -1 || hasTextureUniform == -1) {
		std::cout << "Error assigning program locations" << std::endl;
		std::cout << "vertexPositionAttrib: " << vertexPositionAttrib << std::endl;
		std::cout << "colourAttrib: " << colourAttrib << std::endl;
		std::cout << "uvLocation: " << uvAttrib << std::endl;
		std::cout << "modelMatrixLocation: " << modelMatrixUniform << std::endl;
		std::cout << "viewMatrixLocation: " << viewMatrixUniform << std::endl;
		std::cout << "textureSamplerLocation: " << textureSamplerUniform << std::endl;
	}

	// Clean up shaders, they aren't needed anymore as they are loaded into the program.
	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++) {
		glDeleteShader(shaderList[iLoop].shaderID);
	}
}
void Engine::LoadContect(void) {

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
	LoadContect();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}
void Engine::CleanupSDL(void) {
	std::cout << ">> Starting Cleanup..." << std::endl;
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(sdlWindow);
	std::cout << ">> Cleanup Successful" << std::endl;
	std::cout << ">> Cleanup complete" << std::endl;
}