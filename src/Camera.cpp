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
	this->isCameraCenter = true;
}
Camera::~Camera(void) {

}

void Camera::Update(const float& deltaTime, GameObject& object) {
	// Depending on the Camera mode, call the right function
	(controlMode == CameraMode::Follow) ? FollowObject(deltaTime, object) : ManualControl();

	// Clamp the position to the world bounds
	position.x = glm::clamp(position.x, 0.0f, (Engine_Pointer->levelRegister[Engine_Pointer->indexOfCurrentLevel]->pixelGridSize.x - (Engine_Pointer->windowGridSize * Engine_Pointer->tileSize).x));
	position.y = glm::clamp(position.y, 0.0f, (Engine_Pointer->levelRegister[Engine_Pointer->indexOfCurrentLevel]->pixelGridSize.y - (Engine_Pointer->windowGridSize * Engine_Pointer->tileSize).y));


	// Create the new View matrix from the updated values
	viewMatrix = glm::lookAt(position, glm::vec3(position.x, position.y, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}
void Camera::FollowObject(const float& deltaTime, GameObject& object) {
	if (&object != nullptr) {
		if (isCameraCenter) {
			// Camera is centered on the object.
			// Update the center point of the camera to be the center of the objects sprite.
			centerPoint = object.drawPosition + glm::vec3(glm::vec2(object.sourceFrameSize / 2), position.z);
			// Update the camera position.
			position = glm::vec3(glm::vec2(centerPoint.x, centerPoint.y) - (viewPort / 2.0f), position.z);
		}
		else {
			// Camera will pan towards the center of the object at the given speed.
			// Update the center point of the camera to be the center of the objects sprite.
			centerPoint = object.drawPosition + glm::vec3(glm::vec2(object.sourceFrameSize / 2), position.z);
			// Calculate the position we want to move the camera to
			glm::vec3 targetPosition = glm::vec3(glm::vec2(centerPoint.x, centerPoint.y) - (viewPort / 2.0f), position.z);
			// Clamp the targetPosition to the bounds of the world as the camera's position already is.
			targetPosition.x = glm::clamp(targetPosition.x, 0.0f, (Engine_Pointer->levelRegister[Engine_Pointer->indexOfCurrentLevel]->pixelGridSize.x - (Engine_Pointer->windowGridSize * Engine_Pointer->tileSize).x));
			targetPosition.y = glm::clamp(targetPosition.y, 0.0f, (Engine_Pointer->levelRegister[Engine_Pointer->indexOfCurrentLevel]->pixelGridSize.y - (Engine_Pointer->windowGridSize * Engine_Pointer->tileSize).y));
			// Calculate the difference between the targetposition and where the camera currently is
			glm::vec3 cameraDelta = (targetPosition - position);
			// Set the Speed the camera is to move at
			float speed = 3.0f;
			
			// Update the camera position.
			glm::vec3 movementSize = cameraDelta * (speed * deltaTime);
			position += movementSize;

			// Snap math to make sure the movements don't get too small
			//std::cout << "Camera Delta X: " << cameraDelta.x << ", Y: " << cameraDelta.y << std::endl;
			//std::cout << "Size of the momvement X: " << movementSize.x << ", Y: " << movementSize.y << std::endl;
			glm::vec3 absDelta = glm::abs(cameraDelta);	// Absolute the delta to ignore the x/y direction of it
			if (absDelta.x < 0.5f) {
				position.x = targetPosition.x;
			}
			if (absDelta.y < 0.5f) {
				position.y = targetPosition.y;
			}
		}
	}
}
void Camera::ManualControl(void) {

}
void Camera::SetControlMode(const CameraMode& newMode) {
	this->controlMode = newMode;
}
