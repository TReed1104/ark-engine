#ifndef ARKENGINE_GAMEOBJECT_H_
#define ARKENGINE_GAMEOBJECT_H_

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL_image.h>
#include "Model.h"
#include "Texture.hpp"
#include "EngineConfig.h"

class Engine;

class GameObject {
public:
	Model model;
	int currentTextureIndex;
	std::vector<Texture> textureRegister;
	glm::vec2 textureDimensions;
	glm::vec2 sourceFrameSize;
	glm::vec2 sourceFramePosition;
	glm::vec3 position;
	glm::vec3 drawPosition;
	float rotation;
	glm::vec3 scale;
	glm::vec3 velocity;

	GameObject(const Engine& engine, const Model& model, const glm::vec3& position = glm::vec3(0.0f), const char* texturePath = "", const glm::vec2& sourceFrameSize = glm::vec2(TILE_SIZE_X, TILE_SIZE_Y));
	~GameObject();

	void Update(float deltaTime);
	void Draw();

private:
	const Engine* engine;

	void Translate(const int& indexOfMesh, const glm::vec3 &translation);
	void Rotate(const int& indexOfMesh, const float &rotationAngle, const glm::vec3 &rotationAxis);
	void Scale(const int& indexOfMesh, const glm::vec3 &scale);
	void LoadTexture(const char* texturePath);
};

#endif