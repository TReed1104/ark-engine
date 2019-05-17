#ifndef ARKENGINE_LIGHTSOURCE_H_
#define ARKENGINE_LIGHTSOURCE_H_
#include "glm/glm.hpp"
#include <string>

class Engine;

class LightSource {
public:
	static Engine* Engine_Pointer;

	enum LightingType { Directional, Point, Spotlight };

	// Constructors and deconstructors
	LightSource(const std::string& name, const LightingType& type, const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& direction = glm::vec3(0.0f, 0.0f, -1.0f), 
				const glm::vec3& ambientColour = glm::vec3(0.2f), const glm::vec3& diffuseColour = glm::vec3(1.0f), const glm::vec3& specularColour = glm::vec3(1.0f),
				const float& spotlightCutOff = 12.5f, const float& spotlightCutOffOuter = 17.5f, const float& attenuationConstant = 1.0f, const float& attenuationLinear = 0.0014f, const float& attentuationQuadratic = 0.000007f);
	~LightSource();

	// Get Attributes
	const std::string GetName(void);
	const LightingType GetType(void);
	const glm::vec3 GetPosition(void);
	const glm::vec3 GetDirection(void);

	// Get Light colours and intensities
	const glm::vec3 GetAmbientColour(void);
	const glm::vec3 GetDiffuseColour(void);
	const glm::vec3 GetSpecularColour(void);

	// Get Spotlighting values
	const float GetSpotlightCutOff(void);
	const float GetSpotlightCutOffOuter(void);

	// Get Attenuation values
	const float GetAttenuationConstant(void);
	const float GetAttenuationLinear(void);
	const float GetAttenuationQuadratic(void);

	// Set Attributes
	void SetPosition(const glm::vec3& newPosition);
	void SetDirection(const glm::vec3& newDirection);

	// Get Light colours and intensities
	void SetLightingColours(const glm::vec3& newAmbientColour, const glm::vec3& newDiffuseColour, const glm::vec3& newSpecularColour);
	void SetAmbientColour(const glm::vec3& newAmbientColour);
	void SetDiffuseColour(const glm::vec3& newDiffuseColour);
	void SetSpecularColour(const glm::vec3& newSpecularColour);

	// Get Spotlighting values
	void SetSpotlightValues(const float& newCutOff, const float& newCutOffOuter);

	// Get Attenuation values
	void SetAttenuation(const float& newConstant, const float& newLinear, const float& newQuadratic);

private:
	// Attributes of the light source
	std::string name;
	LightingType type;
	glm::vec3 position;
	glm::vec3 direction;

	// Colours and intensities
	glm::vec3 ambientColour;
	glm::vec3 diffuseColour;
	glm::vec3 specularColour;

	// Spotlighting
	float spotlightCutOff;
	float spotlightCutOffOuter;

	// Attenuation
	float attenuationConstant;
	float attenuationLinear;
	float attenuationQuadratic;

};

#endif