#ifndef ARKENGINE_LIGHTSOURCE_H_
#define ARKENGINE_LIGHTSOURCE_H_
#include "glm/glm.hpp"
#include <string>

class Engine;

class LightSource {
public:
	static Engine* Engine_Pointer;

	// Constructors and deconstructors
	LightSource(const std::string& lightName, const glm::vec3& lightPosition = glm::vec3(0.0f));
	~LightSource();

	// Gets
	const std::string GetName(void);
	const glm::vec3 GetPosition(void);
	const glm::vec3 GetDirection(void);
	const glm::vec3 GetAmbientIntensity(void);
	const glm::vec3 GetDiffuseColour(void);
	const glm::vec3 GetSpecularIntensity(void);
	const float GetSpotlightCutOff(void);
	const float GetSpotlightCutOffOuter(void);
	const float GetAttenuationConstant(void);
	const float GetAttenuationLinear(void);
	const float GetAttenuationQuadratic(void);

	// Sets
	void SetPosition(const glm::vec3& newPosition);
	void SetDirection(const glm::vec3& newDirection);
	void SetAmbientIntensity(const glm::vec3& newAmbientIntensity);
	void SetDiffuseColour(const glm::vec3& newDiffuseColour);
	void SetSpecularIntensity(const glm::vec3& newSpecularIntensity);
	void SetSpotlightCutOff(const float& newCutOff);
	void SetSpotlightCutOffOuter(const float& newCutOffOuter);
	void SetAttenuationConstant(const float& newAttenuationConstant);
	void SetAttenuationLinear(const float& newAttenuationLinear);
	void SetAttenuationQuadratic(const float& newAttenuationQuadratic);

private:
	// Attributes of the light source
	std::string name;
	glm::vec3 position;
	glm::vec3 direction;

	// Colours and intensities
	glm::vec3 ambietyIntensity;
	glm::vec3 diffuseColour;
	glm::vec3 specularIntensity;

	// Spotlighting
	float spotlightCutOff;
	float spotlightCutOffOuter;

	// Attenuation
	float attenuationConstant;
	float attenuationLinear;
	float attenuationQuadratic;

};

#endif