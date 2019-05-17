#include "LightSource.h"
#include "Engine.h"

Engine* LightSource::Engine_Pointer;

LightSource::LightSource(const std::string& name, const LightingType& type, const glm::vec3& position, const glm::vec3& direction, const glm::vec3& ambientColour, const glm::vec3& diffuseColour, const glm::vec3& specularColour, const float& spotlightCutOff, const float& spotlightCutOffOuter, const float& attenuationConstant, const float& attenuationLinear, const float& attentuationQuadratic) {
	this->name = name;
	this->type = type;
	this->position = position;
	this->direction = direction;
	this->ambientColour = ambientColour;
	this->diffuseColour = diffuseColour;
	this->specularColour = specularColour;
	this->spotlightCutOff = spotlightCutOff;
	this->spotlightCutOffOuter = spotlightCutOffOuter;
	this->attenuationConstant = attenuationConstant;
	this->attenuationLinear = attenuationLinear;
	this->attenuationQuadratic = attentuationQuadratic;

	switch (this->type) {
		case LightSource::Directional:
			this->position = glm::vec3(0.0f);
			break;
		case LightSource::Point:
			this->spotlightCutOff = 0.0f;
			this->spotlightCutOff = 0.0f;
			break;
		default:
			break;
	}
}
LightSource::~LightSource() {
}

const std::string LightSource::GetName(void) {
	return this->name;
}
const LightSource::LightingType LightSource::GetType(void) {
	return this->type;
}
const glm::vec3 LightSource::GetPosition(void) {
	return this->position;
}
const glm::vec3 LightSource::GetDirection(void) {
	return this->direction;
}
const glm::vec3 LightSource::GetAmbientColour(void) {
	return this->ambientColour;
}
const glm::vec3 LightSource::GetDiffuseColour(void) {
	return this->diffuseColour;
}
const glm::vec3 LightSource::GetSpecularColour(void) {
	return this->specularColour;
}
const float LightSource::GetSpotlightCutOff(void) {
	return this->spotlightCutOff;
}
const float LightSource::GetSpotlightCutOffOuter(void) {
	return spotlightCutOffOuter;
}
const float LightSource::GetAttenuationConstant(void) {
	return this->attenuationConstant;
}
const float LightSource::GetAttenuationLinear(void) {
	return this->attenuationLinear;
}
const float LightSource::GetAttenuationQuadratic(void) {
	return this->attenuationQuadratic;
}
void LightSource::SetPosition(const glm::vec3& newPosition) {
	this->position = newPosition;
}
void LightSource::SetDirection(const glm::vec3& newDirection) {
	this->direction = newDirection;
}
void LightSource::SetLightingColours(const glm::vec3& newAmbientColour, const glm::vec3& newDiffuseColour, const glm::vec3& newSpecularColour) {
	this->ambientColour = newAmbientColour;
	this->diffuseColour = newDiffuseColour;
	this->specularColour = newSpecularColour;
}
void LightSource::SetAmbientColour(const glm::vec3& newAmbientColour) {
	this->ambientColour = newAmbientColour;
}
void LightSource::SetDiffuseColour(const glm::vec3& newDiffuseColour) {
	this->diffuseColour = newDiffuseColour;
}
void LightSource::SetSpecularColour(const glm::vec3& newSpecularColour) {
	this->specularColour = newSpecularColour;
}
void LightSource::SetSpotlightValues(const float& newCutOff, const float& newCutOffOuter) {
	this->spotlightCutOff = newCutOff;
	this->spotlightCutOffOuter = newCutOffOuter;
}
void LightSource::SetAttenuation(const float& newConstant, const float& newLinear, const float & newQuadratic) {
	this->attenuationConstant = newConstant;
	this->attenuationLinear = newLinear;
	this->attenuationQuadratic = newQuadratic;
}
