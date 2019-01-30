#ifndef ARKENGINE_ENTITY_H_
#define ARKENGINE_ENTITY_H_
#include "GameObject.h"
#include "Utilities.hpp"

class Engine;

class Entity : public GameObject
{
public:
	enum Directions { NotSet, Left, Right, Up, Down };
	enum AnimationState { AnimationIdleStandingLeft, AnimationIdleStandingRight, AnimationMovingStandingLeft, AnimationMovingStandingRight, 
							AnimationIdleCrawlingLeft, AnimationIdleCrawlingRight, AnimationMovingCrawlingLeft, AnimationMovingCrawlingRight, 
								AnimationJumpingLeft, AnimationJumpingRight, AnimationFallingLeft, AnimationFallingRight };
	
	Directions movementDirection = Directions::NotSet;
	Directions spriteDirection = Directions::Right;
	AnimationState animationState = AnimationState::AnimationIdleStandingRight;;

	Entity(const std::string& filePath);
	~Entity(void);

	void Update(const float& deltaTime);

private:

protected:
	bool canCrawl = false;
	bool isTryingToCrawl = false;
	bool isCrawling = false;
	bool isJumping = false;
	float baseMovementSpeed = 90.0f;
	float maxMovementSpeed = 120.0f;
	float currentMovementSpeed = 90.0f;
	float timeForMaxJump = 0.4f;
	float baseJumpingSpeed = -220.0f;
	float currentJumpingSpeed = 0.0f;
	float jumpingTimer = 0.0f;

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