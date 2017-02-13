#ifndef Engine_h_included
#define Engine_h_included

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <vector>
#include "GL\glew.h"
#include "SDL.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <assimp/mesh.h>
#include <assimp/material.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "ShaderComponents.h"
#include "Shader.h"
#include "Camera.h"
#include "Tile.h"
#include "GameObject.h"

// Useful constants to the Engine.
const float PI = 3.14f;
const int MINIMUM_WINDOW_SIZE_WIDTH = 300;
const int MINIMUM_WINDOW_SIZE_HEIGHT = 300;


class Engine {
public:
	Camera camera;
	// Texture Register
	// Level Register
	std::vector<Model> modelRegister;
	std::vector<GameObject*> tileRegister;
	GameObject* player;
	std::vector<GameObject*> agentRegister;
	std::vector<GameObject*> itemRegister;
	SDL_Window *sdlWindow;

	Engine(char* gameName);
	~Engine(void);

	void Run(void);

private:
	std::string exeName;
	bool isRunning = true;
	SDL_GLContext glContext;
	GLuint glProgram;
	ShaderComponents shaderComponent;
	float aspectRatio;
	glm::vec2 windowSize;
	float fieldOfView;
	float oldFrameTime;
	float currentFrameTime;

	// Functions
	void InitialiseSDL(void);
	void CreateSDLWindow(void);
	void CreateSDLContext(void);
	void InitialiseGlew(void);
	GLuint CreateGLProgram(const std::vector<Shader> &shaderList);
	void InitialiseProgram(void);
	Model LoadModel(std::string modelPath);
	void LoadContent(void);
	void SetupEnvironment(void);
	void CleanupSDL(void);
	void InitialiseWorldCamera(void);
	void EventHandling(void);
	void Update(float deltaTime);
	void Draw(void);
	void Renderer(void);

};


#endif