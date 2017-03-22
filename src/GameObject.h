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
#include "BoundingBox.h"

class Engine;

class GameObject {
public:
	static Engine* Engine_Pointer;

	// Model and texture variables
	Model model;
	const Texture* texture;
	glm::vec2 sourceFrameSize;
	glm::vec2 sourceFramePosition;
	// Transformations
	glm::vec3 position;			// The world position of the object
	glm::vec2 drawOffset;
	glm::vec3 drawPosition;		// Position to draw to the screen, this takes into account the offset of the object to its texture
	glm::vec2 gridPosition;
	float rotation;
	glm::vec3 scale;
	// Physics
	BoundingBox boundingBox;
	glm::vec2 velocity;
	float movementSpeed;

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