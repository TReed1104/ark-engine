#ifndef ARKENGINE_GAMEOBJECT_H_
#define ARKENGINE_GAMEOBJECT_H_

#include <string>
#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL_image.h>
#include "Model.h"
#include "Texture.h"
#include "Animation.h"
#include "BoundingBox.h"
#include "LuaScript.h"

class Engine;

class GameObject {
public:
	static Engine* Engine_Pointer;

	// Rendering
	const Texture* texture;
	glm::ivec2 sourceFrameSize;
	glm::ivec2 sourceFramePosition;
	glm::vec3 drawPosition;
	glm::ivec2 drawOffset;		// The offset the sprite is to be drawn in relation to the world position.
	
	// Bounding Box variables
	BoundingBox boundingBox;
	glm::vec2 boundingBoxOffset;
	
	GameObject(const std::string& scriptPath = "NO SCRIPT");
	~GameObject(void);

	virtual void Update(const float& deltaTime);
	void Draw(void);
	void Reposition(const glm::vec2& newPosition);

private:
	void UpdatePosition(void);
	void UpdateRotation(void);
	void UpdateScale(void);
	void TransformationController(void);
	void LoadAnimations(void);
	void AnimationController(const float& deltaTime);
	virtual void PhysicsController(const float& deltaTime);
	virtual void AnimationStateHandler(void);
	virtual void AnimationIndexHandler(void);

protected:
	LuaScript* script;	// The script attached to the object

	// Animations
	std::vector<Animation> animations;
	int animationIndex;

	// Shader indexer
	int indexOfCurrentShader;

	// Model variables
	Model model;

	// Transformations
	glm::vec3 position;			// The world position of the object
	glm::ivec2 gridPosition;	// The current grid cell the top-left of the object is within
	float rotation;				// Rotation of the object
	glm::vec3 scale;			// Scale of the object


	// Movement Physics
	bool isFalling;
	bool isAffectedByGravity;
	glm::vec2 velocity;
	const float baseMovementSpeed = 80.0f;
	const float maxMovementSpeed = 120.0f;
	float currentMovementSpeed;
	const float timeForMaxFall = 1.5f;
	const float baseFallingSpeed = 2.0f;
	const float maxFallingSpeed = 200.0f;
	float currentFallingSpeed;
	float fallTimer;
	const float timeForMaxJump = 0.8f;
	const float baseJumpingSpeed = -120.0f;
	float currentJumpingSpeed;
	float jumpingTimer;

	void PhysicsHandlerFalling(const float& deltaTime);
};

#endif
