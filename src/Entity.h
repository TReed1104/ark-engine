#ifndef ARKENGINE_ENTITY_H_
#define ARKENGINE_ENTITY_H_
#include "GameObject.h"
#include "Utilities.hpp"

class Engine;

class Entity : public GameObject
{
public:
	enum Directions { Left, Right, Up, Down };
	Directions movementDirection;
	enum AnimationState { AnimationIdleLeft, AnimationIdleRight, AnimationFalling, AnimationJumping, AnimationMoveLeft, AnimationMoveRight };
	AnimationState animationState;

	Entity(const std::string& scriptPath);
	~Entity(void);

	void Update(const float& deltaTime);
	virtual void EntityController(void) {}	// The AI or Player Controller, gets overriden by the derived classes

private:

protected:
	bool isJumping;
	bool isFalling;

	void MovementController(const float& deltaTime);	// Executes the movements specified by EntityController
	void UpdateAnimationState(void);
	void UpdateAnimationIndex(void);
};

#endif