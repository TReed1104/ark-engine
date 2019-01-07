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
	glm::ivec2 drawOffset;
	
	// Bounding Box variables
	BoundingBox boundingBox;
	glm::vec2 boundingBoxOffset;
	
	GameObject(const std::string& scriptPath = "NO SCRIPT");
	~GameObject(void);

	virtual void Update(const float& deltaTime);
	void Draw(void);
	void Reposition(const glm::vec2& newPosition);

private:
	void UpdateRotation(void);
	void UpdateScale(void);
	void TransformationController(void);
	void LoadAnimations(void);
	void AnimationController(const float& deltaTime);

protected:
	LuaScript* script;

	// Animations
	int animationIndex;
	std::vector<Animation> animations;

	// Shader indexer
	int indexOfCurrentShader;

	// Transformations
	glm::vec3 position;
	glm::ivec2 gridPosition;
	Model model;
	float rotation;
	glm::vec3 scale;

	// Movement Physics
	bool isFalling;
	bool isAffectedByGravity;
	glm::vec2 velocity;
	const float timeForMaxFall = 0.7f;
	const float baseFallingSpeed = 5.0f;
	const float maxFallingSpeed = 250.0f;
	float currentFallingSpeed;
	float fallTimer;

	virtual void UpdatePosition(void);
	virtual void PhysicsHandlerFalling(const float& deltaTime);
	virtual void PhysicsController(const float& deltaTime);
	virtual void AnimationStateHandler(void);
	virtual void AnimationIndexHandler(void);
};

#endif
