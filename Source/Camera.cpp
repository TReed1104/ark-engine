// Header file Includes.
#include "Camera.h"
#include "Tile.h"
#include "Engine.h"


Camera::Camera(Engine &engine, glm::vec3 &cameraPosition, glm::vec3 &cameraLookAt, glm::vec3 &upVector) {
	// Initialise the Camera.
	this->engine = &engine;
	this->position = cameraPosition;
	this->lookAt = cameraLookAt;
	this->upVector = upVector;
	this->viewMatrix = glm::lookAt(cameraPosition, cameraLookAt, upVector);
}
Camera::~Camera() {

}

void Camera::Update(float deltaTime) {
	if (engine->player != nullptr) {
		position = glm::vec3(engine->player->position.x, engine->player->position.y, position.z);
		lookAt = glm::vec3(position.x, position.y, lookAt.z);
	}
	// Calculate the position of the AABB in the TileMap grid, the math is floored to get grid position.
	cameraGridPosition = glm::floor(glm::vec2(position.x, position.y) / glm::vec2(TILE_SIZE_X, TILE_SIZE_Y));
	viewMatrix = glm::lookAt(position, lookAt, upVector);	// Update the ViewMatrix.
}