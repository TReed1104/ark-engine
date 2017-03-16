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

class Engine;

class GameObject {
public:
	static Engine* engine;
	Model model;
	const Texture* texture;
	glm::vec2 sourceFrameSize;
	glm::vec2 sourceFramePosition;
	glm::vec3 position;
	glm::vec3 drawPosition;
	float rotation;
	glm::vec3 scale;
	glm::vec3 velocity;

	GameObject(const Model& model, const Texture& texture = Texture("Empty"), const glm::vec3& position = glm::vec3(0.0f), const glm::vec2& sourceFrameSize = glm::vec2(16, 16));
	~GameObject();

	void Update(float deltaTime);
	void Draw();

private:

	void Translate(const int& indexOfMesh, const glm::vec3 &translation);
	void Rotate(const int& indexOfMesh, const float &rotationAngle, const glm::vec3 &rotationAxis);
	void Scale(const int& indexOfMesh, const glm::vec3 &scale);
};

#endif