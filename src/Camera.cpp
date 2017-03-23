#include "Camera.h"
#include "Tile.h"
#include "Engine.h"
#include "GameObject.h"

Engine* Camera::Engine_Pointer;

Camera::Camera(const glm::vec3& cameraPosition, const glm::vec3& cameraLookAt, const glm::vec3& upVector, const glm::mat4& projectionMatrix, const CameraMode& cameraMode) {
	// Initialise the Camera.
	this->position = cameraPosition;
	this->lookAt = cameraLookAt;
	this->upVector = upVector;
	this->viewMatrix = glm::lookAt(cameraPosition, cameraLookAt, upVector);
	this->projectionMatrix = projectionMatrix;
	this->controlMode = cameraMode;
}
Camera::~Camera() {

}

void Camera::Update(const float& deltaTime, const GameObject& object) {
	if (controlMode == CameraMode::Follow) {
		FollowObject(object);
	}
	else if (controlMode == CameraMode::Manual) {
		ManualControl();
	}
	ClampCameraToWorld();
	viewMatrix = glm::lookAt(position, lookAt, upVector);	// Update the ViewMatrix.
}
void Camera::FollowObject(const GameObject& object) {
	if (&object != nullptr) {

		glm::vec2 viewPort = Engine_Pointer->windowGridSize * Engine_Pointer->tileSize;
		glm::vec2 viewPortOffset = viewPort / 2.0f;
		BoundingBox boundingBoxOfObject = object.boundingBox;
		glm::vec2 objectCenterOffset = (boundingBoxOfObject.GetDimensions() / 2.0f);
		viewPortOffset -= objectCenterOffset;


		position = glm::vec3(boundingBoxOfObject.GetPosition().x - viewPortOffset.x, boundingBoxOfObject.GetPosition().y - viewPortOffset.y, position.z);
		lookAt = glm::vec3(position.x, position.y, lookAt.z);
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
	lookAt = glm::vec3(position.x, position.y, lookAt.z);
}
