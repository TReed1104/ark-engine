#ifndef ARKENGINE_GAMEOBJECT_H_
#define ARKENGINE_GAMEOBJECT_H_

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL_image.h>
#include "Model.h"
#include "Texture.hpp"
#include "Animation.h"
#include "BoundingBox.h"
#include "LuaScript.h"

class Engine;

class GameObject {
public:
	static Engine* Engine_Pointer;

	// Animation Enum
	enum AnimationState { IdleDown, IdleUp, IdleLeft, IdleRight, MoveDown, MoveUp, MoveLeft, MoveRight, AttackDown, AttackUp, AttackLeft, AttackRight, };

	// Shader indexer
	int indexOfCurrentShader;

	// Model and texture variables
	LuaScript* script;
	Model model;
	const Texture* texture;
	glm::vec2 sourceFrameSize;
	glm::vec2 sourceFramePosition;
	std::vector<Animation> animations;

	// Positions
	glm::vec3 position;			// The world position of the object
	glm::vec2 gridPosition;		// The current grid cell the object is counted as being within.
	glm::vec2 drawOffset;		// The offset the sprite is to be drawn in relation to the world position.
	glm::vec3 drawPosition;		// Position to draw to the screen, this takes into account the offset of the object to its texture

	// Bounding Boxes
	BoundingBox boundingBox;
	glm::vec2 boundingBoxOffset;

	// Movement Physics
	glm::vec2 velocity;
	glm::vec2 velocitySnap;
	float movementSpeed;
	
	// Transformations
	float rotation;
	glm::vec3 scale;
	bool doRotation;		// Flag controlling if the Object needs rotating
	bool doScalar;			// Flag controlling if the Object needs scaling
	
	GameObject(const std::string& scriptPath = "NO SCRIPT");
	~GameObject(void);

	void Update(const float& deltaTime);
	void Draw(void);
	void Reposition(const glm::vec2& newPosition);

private:
	void UpdatePosition(bool doTransform = false);
	void UpdateRotation();
	void UpdateScale();
	void LoadAnimations();
	void AnimationHandler(const float& deltaTime);

protected:

};

#endif
