#include "Camera.h"
#include "Tile.h"
#include "Engine.h"
#include "GameObject.h"
#include <iomanip>

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
	// Depending on the Camera mode, call the right function
	(controlMode == CameraMode::Follow) ? FollowObject(deltaTime, object) : ManualControl();

	// Clamp the position to the world bounds
	position.x = glm::clamp(position.x, 0.0f, (Engine_Pointer->levelRegister[Engine_Pointer->indexCurrentLevel]->pixelGridSize.x - (Engine_Pointer->windowGridSize * Engine_Pointer->tileSize).x));
	position.y = glm::clamp(position.y, 0.0f, (Engine_Pointer->levelRegister[Engine_Pointer->indexCurrentLevel]->pixelGridSize.y - (Engine_Pointer->windowGridSize * Engine_Pointer->tileSize).y));

	// Create the new View matrix from the updated values
	viewMatrix = glm::lookAt(position, glm::vec3(position.x, position.y, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}
void Camera::FollowObject(const float& deltaTime, GameObject& object) {
	if (&object != nullptr) {
		centerPoint = object.position + glm::vec3((object.boundingBox.GetDimensions() / 2.0f), position.z);
		glm::vec2 viewPortOffset = (viewPort / 2.0f);
		position = centerPoint - glm::vec3(viewPortOffset, 0.0f);

		//glm::vec3 targetPosition = centerPoint - glm::vec3(viewPortOffset, 0.0f);
		//targetPosition.x = glm::clamp(targetPosition.x, 0.0f, (Engine_Pointer->levelRegister[Engine_Pointer->indexCurrentLevel]->pixelGridSize.x - (Engine_Pointer->windowGridSize * Engine_Pointer->tileSize).x));
		//targetPosition.y = glm::clamp(targetPosition.y, 0.0f, (Engine_Pointer->levelRegister[Engine_Pointer->indexCurrentLevel]->pixelGridSize.y - (Engine_Pointer->windowGridSize * Engine_Pointer->tileSize).y));
		//position += (targetPosition - position) * (1.0f * deltaTime);
		//std::cout << std::setprecision(15) << position.x << ", " << position.y << std::endl;
	}
}
void Camera::ManualControl(void) {

}
void Camera::SetControlMode(const CameraMode& newMode) {
	this->controlMode = newMode;
}
