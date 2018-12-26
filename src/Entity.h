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
	Directions spriteDirection;
	enum AnimationState { IdleLeft, IdleRight, Fall, Jump, MoveLeft, MoveRight };
	AnimationState animationState;

	Entity(const std::string& scriptPath);
	~Entity(void);

	void Update(const float& deltaTime);
	virtual void HandleMovement(void) {}
	void HandleCollisions(float deltaTime);

private:

protected:
	void UpdateAnimationState(void);
	void UpdateAnimationIndex(void);
};

#endif