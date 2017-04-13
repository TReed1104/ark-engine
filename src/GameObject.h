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
#include "LuaScript.h"

class Engine;

class GameObject {
public:
	static Engine* Engine_Pointer;


	GameObject(const std::string& scriptPath = "NO SCRIPT");
	~GameObject();
	// Model and texture variables
	LuaScript* script;
	Model model;
	const Texture* texture;
	glm::vec2 sourceFrameSize;
	glm::vec2 sourceFramePosition;
	
	// Physics
	glm::vec2 boundingBoxOffset;
	BoundingBox boundingBox;
	glm::vec2 velocity;
	glm::vec2 velocityForTileSnap;
	float movementSpeed;

	void Update(const float& deltaTime);
	void Draw();
	
	glm::vec2 GetPosition();
	void SetPosition(glm::vec2 newPosition);


private:
	void Translate(const int& indexOfMesh, const glm::vec3 &translation);
	void Rotate(const int& indexOfMesh, const float &rotationAngle, const glm::vec3 &rotationAxis);
	void Scale(const int& indexOfMesh, const glm::vec3 &scale);

protected:
	// Transformations
	glm::vec3 position;			// The world position of the object
	glm::vec2 drawOffset;
	glm::vec3 drawPosition;		// Position to draw to the screen, this takes into account the offset of the object to its texture
	glm::vec2 gridPosition;
	float rotation;
	glm::vec3 scale;
};

#endif