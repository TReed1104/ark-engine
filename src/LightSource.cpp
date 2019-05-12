#include "LightSource.h"
#include "Engine.h"

Engine* LightSource::Engine_Pointer;

// Constructors and deconstructors
LightSource::LightSource(const glm::vec3& lightPosition, const glm::vec3& lightColour) {
	this->position = lightPosition;
	this->colour = lightColour;
}
LightSource::~LightSource() {

}

// Gets
const glm::vec3 LightSource::GetPosition(void) {
	return this->position;
}
const glm::vec3 LightSource::GetColour(void) {
	return this->colour;
}
const float LightSource::GetAmbientStrength(void) {
	return this->ambientStrength;
}
const float LightSource::GetSpecularStrength(void) {
	return this->specularStrength;
}

// Sets
void LightSource::SetPosition(const glm::vec3& newPosition) {
	this->position = newPosition;
}
void LightSource::SetColour(const glm::vec3& newColour) {
	this->colour = newColour;
}
void LightSource::SetAmbientStrength(const float& newAmbientStrength) {
	this->ambientStrength = newAmbientStrength;
}
void LightSource::SetSpecularStrength(const float& newSpecularStrength) {
	this->specularStrength = newSpecularStrength;
}
