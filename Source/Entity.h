#ifndef ARKENGINE_ENTITY_H_
#define ARKENGINE_ENTITY_H_
#include "GameObject.h"

class Entity : public GameObject
{
public:
	Entity(const Engine& engine, const Model& model, const glm::vec3& position = glm::vec3(0.0f), const char* texturePath = "", const glm::vec2& sourceFrameSize = glm::vec2(TILE_SIZE_X, TILE_SIZE_Y));
	~Entity();

	void Update(float deltaTime);

private:

};

#endif