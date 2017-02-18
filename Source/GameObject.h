#ifndef ARKENGINE_GAMEOBJECT_H_
#define ARKENGINE_GAMEOBJECT_H_

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
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

	virtual void Update(float deltaTime);
	virtual void Draw();

private:
	const Engine* engine;
	glm::mat4 translationMatrix;
	glm::mat4 rotationMatrix;
	glm::mat4 scaleMatrix;

	void Translate(const glm::vec3 &translation);
	void Rotate(const float &rotationAngle, const glm::vec3 &rotationAxis);
	void Scale(const glm::vec3 &scale);
	glm::mat4 GetModelMatrix();
	void LoadTexture(const char* texturePath);
};

#endif