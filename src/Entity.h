#ifndef ARKENGINE_ENTITY_H_
#define ARKENGINE_ENTITY_H_
#include "GameObject.h"

class Entity : public GameObject
{
public:
	enum MovementDirection {Up, Down, Left, Right, UpLeft, UpRight, DownLeft, DownRight};
	MovementDirection movementDirection;

	Entity(const Model& model, const Texture& texture, const glm::vec3& position = glm::vec3(0.0f), const glm::vec2& sourceFrameSize = glm::vec2(16, 16));
	~Entity();

	void Update(float deltaTime);
	virtual void HandleMovement(void) {}
	virtual void HandleCollisions(void) {}

private:

};

#endif