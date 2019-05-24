#include "LightSource.h"
#include "Engine.h"

Engine* LightSource::Engine_Pointer;

LightSource::LightSource(const std::string& name, const glm::vec3& position, const glm::vec3& colour, const float& radius) {
	this->name = name;
	this->position = position;
	this->colour = colour;
	this->radius = radius;
}
LightSource::~LightSource() {

}

void LightSource::Update(float deltaTime) {
	// Logic for the light
}
const std::string LightSource::GetName(void) {
	return name;
}
const glm::vec3 LightSource::GetPosition(void) {
	return position;
}
const glm::vec3 LightSource::GetColour(void) {
	return colour;
}
const float LightSource::GetRadius(void) {
	return radius;
}
void LightSource::SetPosition(const glm::vec3& newPosition) {
	this->position = newPosition;
}
void LightSource::SetColour(const glm::vec3& newColour) {
	this->colour = newColour;
}
void LightSource::SetRadius(const float& newRadius) {
	this->radius = newRadius;
}
