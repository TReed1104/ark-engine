#ifndef ARKENGINE_ENTITY_H_
#define ARKENGINE_ENTITY_H_
#include "GameObject.h"
#include "Utilities.hpp"

class Engine;

class Entity : public GameObject
{
public:
	enum Directions { NotSet, Left, Right, Up, Down };
	Directions movementDirection;
	Directions spriteDirection;
	enum AnimationState { AnimationIdleStandingLeft, AnimationIdleStandingRight, AnimationMovingStandingLeft, AnimationMovingStandingRight, 
							AnimationIdleCrawlingLeft, AnimationIdleCrawlingRight, AnimationMovingCrawlingLeft, AnimationMovingCrawlingRight, 
								AnimationJumpingLeft, AnimationJumpingRight, AnimationFallingLeft, AnimationFallingRight };
	AnimationState animationState;

	Entity(const std::string& scriptPath);
	~Entity(void);

	void Update(const float& deltaTime);

private:

protected:
	bool canCrawl;
	bool isTryingToCrawl;
	bool isCrawling;
	bool isJumping;
	const float baseMovementSpeed = 80.0f;
	const float maxMovementSpeed = 120.0f;
	float currentMovementSpeed;
	const float timeForMaxJump = 0.6f;
	const float baseJumpingSpeed = -150.0f;
	float currentJumpingSpeed;
	float jumpingTimer;

	BoundingBox standingBoundingBox;
	glm::vec2 standingBoundingBoxOffset;
	BoundingBox crawlingBoundingBox;
	glm::vec2 crawlingBoundingBoxOffset;

	virtual void EntityController(void) {}	// The AI or Player Controller, gets overriden by the derived classes
	void UpdatePosition(void);
	void PhysicsHandlerCrawling(const float& deltaTime);
	void PhysicsHandlerJumping(const float& deltaTime);
	void PhysicsHandlerFalling(const float & deltaTime);
	void PhysicsHandlerMovement(const float& deltaTime);
	void PhysicsController(const float& deltaTime);
	void AnimationStateHandler(void);
	void AnimationIndexHandler(void);
};

#endif