#ifndef ARKENGINE_LIGHTSOURCE_H_
#define ARKENGINE_LIGHTSOURCE_H_
#include "glm/glm.hpp"
#include <string>

class Engine;

class LightSource {
public:
	static Engine* Engine_Pointer;

	// Constructors and deconstructors
	LightSource(const std::string& lightName, const glm::vec3& lightPosition = glm::vec3(0.0f), const glm::vec3& lightColour = glm::vec3(1.0f));
	~LightSource();

	// Gets
	const std::string GetName(void);
	const glm::vec3 GetPosition(void);
	const glm::vec3 GetColour(void);
	const float GetAmbientStrength(void);
	const float GetSpecularStrength(void);

	// Sets
	void SetPosition(const glm::vec3& newPosition);
	void SetColour(const glm::vec3& newColour);
	void SetAmbientStrength(const float& newAmbientStrength);
	void SetSpecularStrength(const float& newSpecularStrength);

private:
	// Attributes of the light source
	std::string name;
	glm::vec3 position;					// Position of the light in world space
	glm::vec3 colour;					// Colour of the light
	float ambientStrength = 0.1f;		// How strong is the light ambience in the world?
	float specularStrength = 0.5f;		// How strong is the specular spot on a surface?
};

#endif