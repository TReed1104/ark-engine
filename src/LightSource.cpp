#include "LightSource.h"
#include "Engine.h"

Engine* LightSource::Engine_Pointer;

// Constructors and deconstructors
LightSource::LightSource(const std::string& lightName, const glm::vec3& lightPosition, const glm::vec3& lightColour, const float& lightAmbientStrength, const float& lightSpecularStrength) {
	this->name = lightName;
	this->position = lightPosition;
	this->colour = lightColour;
	this->ambientStrength = lightAmbientStrength;
	this->specularStrength = lightSpecularStrength;
}
LightSource::~LightSource() {

}

// Gets
const std::string LightSource::GetName(void) {
	return name;
}
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
