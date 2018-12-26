#ifndef ARKENGINE_ENTITY_H_
#define ARKENGINE_ENTITY_H_
#include "GameObject.h"
#include "Utilities.hpp"

class Engine;

class Entity : public GameObject
{
public:
	enum Directions { MovementNotSet, MovementLeft, MovementRight, MovementFalling, MovementJumping };
	Directions movementDirection;
	Directions spriteDirection;
	enum AnimationState { AnimationIdleLeft, AnimationIdleRight, AnimationFalling, AnimationJumping, AnimationMoveLeft, AnimationMoveRight };
	AnimationState animationState;

	Entity(const std::string& scriptPath);
	~Entity(void);

	void Update(const float& deltaTime);
	virtual void MovementController(void) {}

private:

protected:
	bool isJumping;
	bool isFalling;

	void Move(const float& deltaTime);
	void UpdateAnimationState(void);
	void UpdateAnimationIndex(void);
};

#endif