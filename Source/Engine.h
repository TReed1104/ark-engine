#ifndef ARKENGINE_ENGINE_H_
#define ARKENGINE_ENGINE_H_

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
#include "EngineConfig.h"
#include "ShaderPointers.hpp"
#include "Shader.h"
#include "Camera.h"
#include "Tile.h"
#include "GameObject.h"
#include "Player.h"

class Engine {
public:
	Camera camera;
	// Texture Register
	// Level Register
	std::vector<Model> modelRegister;
	std::vector<Tile*> tileRegister;
	Player* player;
	std::vector<GameObject*> entityRegister;
	std::vector<GameObject*> itemRegister;
	SDL_Window* sdlWindow;
	ShaderPointers shaderPointers;
	SDL_GLContext glContext;
	GLuint glProgram;

	Engine(char* gameName);
	~Engine(void);

	void Run(void);

private:
	std::string exeName;
	bool isRunning = true;
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
	GLuint CreateGLProgram(const std::vector<Shader>& shaderList);
	void InitialiseProgram(void);
	Model LoadModel(const std::string& modelPath);
	void LoadContent(void);
	void SetupEnvironment(void);
	void CleanupSDL(void);
	void InitialiseWorldCamera(void);
	void EventHandler(void);
	void Event_Window(const SDL_WindowEvent& windowEvent);
	void Event_KeyDown(const SDL_KeyboardEvent& keyboardEvent);
	void Event_KeyUp(const SDL_KeyboardEvent& keyboardEvent);
	void Update(float deltaTime);
	void Draw(void);
	void Renderer(void);

};


#endif