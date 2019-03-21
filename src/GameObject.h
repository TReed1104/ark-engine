#ifndef ARKENGINE_GAMEOBJECT_H_
#define ARKENGINE_GAMEOBJECT_H_

#include <string>
#include <vector>
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
#include "JsonParser.hpp"

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
	
	GameObject(const std::string& filePath = "NOT LOADED");
	~GameObject(void);

	virtual void Update(const float& deltaTime);
	void Draw(void);
	const std::string GetName(void);
	const bool IsLoaded(void);
	void Reposition(const glm::vec2& newPosition);
	void Reposition(const glm::vec3& newPosition);
	const glm::vec2 GetGridPosition(void);

private:
	void UpdateRotation(void);
	void UpdateScale(void);
	void TransformationController(void);
	void AnimationController(const float& deltaTime);

protected:
	JsonFile* configFile = nullptr;
	std::string name;
	bool isLoaded = false;

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
	bool isFalling = false;
	bool isAffectedByGravity = false;
	glm::vec2 velocity = glm::vec2(0.0f);
	float timeForMaxFall = 0.4f;
	float baseFallingSpeed = 75.0f;
	float maxFallingSpeed = 300.0f;
	float currentFallingSpeed = 0.0f;
	float fallTimer = 0.0f;

	virtual void UpdatePosition(void);
	virtual void PhysicsHandlerFalling(const float& deltaTime);
	virtual void PhysicsController(const float& deltaTime);
	virtual void LoadAnimations(void) = 0;
	virtual void AnimationStateHandler(void) = 0;
	virtual void AnimationIndexHandler(void) = 0;
};

#endif
