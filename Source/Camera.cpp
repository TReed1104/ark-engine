// Header file Includes.
#include "Camera.h"
#include "Tile.h"
#include "Engine.h"
#include "GameObject.h"

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

void Camera::Update(float deltaTime, GameObject &object) {
	if (controlMode == CameraMode::FreeFollow || controlMode == CameraMode::LockedFollow) {
		FollowObject(object);
	}
	else if (controlMode == CameraMode::Manual) {
		ManualControl();
	}
	viewMatrix = glm::lookAt(position, lookAt, upVector);	// Update the ViewMatrix.
}
void Camera::FollowObject(GameObject &object) {
	if (engine->player != nullptr) {
		position = glm::vec3(engine->player->position.x, engine->player->position.y, position.z);
		lookAt = glm::vec3(position.x, position.y, lookAt.z);
	}
}
void Camera::ManualControl(void) {

}
void Camera::SetControlMode(CameraMode newMode) {

}