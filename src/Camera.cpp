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
		bool isCameraCenter = true;
		if (isCameraCenter) {
			// Camera is centered on the object.
			// Update the center point of the camera to be the center of the objects sprite.
			centerPoint = object.position + glm::vec3((object.boundingBox.GetDimensions() / 2.0f), position.z);
			// Update the camera position.
			position = centerPoint - glm::vec3(viewPort / 2.0f, 0.0f);
		}
		else {
			// Camera will pan towards the center of the object at the given speed.
			// Update the center point of the camera to be the center of the objects sprite.
			centerPoint = object.position + glm::vec3((object.boundingBox.GetDimensions() / 2.0f), position.z);
			// Calculate the position we want to move the camera to
			glm::vec3 targetPosition = centerPoint - glm::vec3(viewPort / 2.0f, 0.0f);
			// Clamp the targetPosition to the bounds of the world as the camera's position already is.
			targetPosition.x = glm::clamp(targetPosition.x, 0.0f, (Engine_Pointer->levelRegister[Engine_Pointer->indexCurrentLevel]->pixelGridSize.x - (Engine_Pointer->windowGridSize * Engine_Pointer->tileSize).x));
			targetPosition.y = glm::clamp(targetPosition.y, 0.0f, (Engine_Pointer->levelRegister[Engine_Pointer->indexCurrentLevel]->pixelGridSize.y - (Engine_Pointer->windowGridSize * Engine_Pointer->tileSize).y));
			// Calculate the difference between the targetposition and where the camera currently is
			glm::vec3 cameraDelta = (targetPosition - position);
			// Set the Speed the camera is to move at
			float speed = 1.0f;
			// Update the camera position.
			position += cameraDelta * (speed * deltaTime);
		}
	}
}
void Camera::ManualControl(void) {

}
void Camera::SetControlMode(const CameraMode& newMode) {
	this->controlMode = newMode;
}
