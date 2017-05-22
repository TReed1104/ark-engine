#ifndef ARKENGINE_ENTITY_H_
#define ARKENGINE_ENTITY_H_
#include "GameObject.h"
#include "Utilities.hpp"

class Engine;

class Entity : public GameObject
{
public:
	enum Directions { NotSet, Up, Down, Left, Right, UpLeft, UpRight, DownLeft, DownRight};
	Directions movementDirection;

	Entity(const std::string& scriptPath);
	~Entity(void);

	void Update(const float& deltaTime);
	virtual void HandleMovement(void) {}
	void HandleCollisions(float deltaTime);

private:

};

#endif