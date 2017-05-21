#include "Camera.h"
#include "Tile.h"
#include "Engine.h"
#include "GameObject.h"

Engine* Camera::Engine_Pointer;

Camera::Camera(const glm::vec3& cameraPosition, const CameraMode& cameraMode) {
	// Initialise the Camera.
	this->viewPort = (Engine_Pointer->windowGridSize * Engine_Pointer->tileSize);
	this->position = cameraPosition;
	this->projectionMatrix = glm::ortho(0.0f, viewPort.x, viewPort.y, 0.0f, 0.0f, 2.0f);
	this->controlMode = cameraMode;
}
Camera::~Camera(void) {

}

void Camera::Update(const float& deltaTime, GameObject& object) {
	if (controlMode == CameraMode::Follow) {
		FollowObject(deltaTime, object);
	}
	else if (controlMode == CameraMode::Manual) {
		ManualControl();
	}
	ClampCameraToWorld();
	viewMatrix = glm::lookAt(position, glm::vec3(position.x, position.y, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}
void Camera::FollowObject(const float& deltaTime, GameObject& object) {
	if (&object != nullptr) {
		centerPoint = glm::vec3((object.boundingBox.GetPosition() + (object.boundingBox.GetDimensions() / 2.0f)), position.z);
		glm::vec2 viewPortOffset = (viewPort / 2.0f);
		position = centerPoint - glm::vec3(viewPortOffset, 0.0f);
	}
}
void Camera::ManualControl(void) {

}
void Camera::SetControlMode(const CameraMode& newMode) {
	this->controlMode = newMode;
}
void Camera::ClampCameraToWorld(void) {
	if (position.x < 0) { 
		position.x = 0; 
	}
	if (position.y < 0) { 
		position.y = 0; 
	}
	if (position.x > (Engine_Pointer->levelRegister[Engine_Pointer->indexCurrentLevel]->pixelGridSize.x - (Engine_Pointer->windowGridSize * Engine_Pointer->tileSize).x)) {
		position.x = (Engine_Pointer->levelRegister[Engine_Pointer->indexCurrentLevel]->pixelGridSize.x - (Engine_Pointer->windowGridSize * Engine_Pointer->tileSize).x);
	}
	if (position.y >(Engine_Pointer->levelRegister[Engine_Pointer->indexCurrentLevel]->pixelGridSize.y - (Engine_Pointer->windowGridSize * Engine_Pointer->tileSize).y)) {
		position.y = (Engine_Pointer->levelRegister[Engine_Pointer->indexCurrentLevel]->pixelGridSize.y - (Engine_Pointer->windowGridSize * Engine_Pointer->tileSize).y);
	}
}
