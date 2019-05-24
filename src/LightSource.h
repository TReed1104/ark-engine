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
	LightSource(const std::string& name, const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& colour = glm::vec3(1.0f), const float& radius = 128.0f);
	~LightSource();

	void Update(float deltaTime);

	// Get Attributes
	const std::string GetName(void);
	const glm::vec3 GetPosition(void);
	const glm::vec3 GetColour(void);
	const float GetRadius(void);

	// Set Attributes
	void SetPosition(const glm::vec3& newPosition);
	void SetColour(const glm::vec3& newColour);
	void SetRadius(const float& newRadius);

private:
	// Attributes of the light source
	std::string name;
	glm::vec3 position;
	glm::vec3 colour;
	float radius;

};

#endif