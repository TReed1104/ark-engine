#include "Entity.h"
#include "Engine.h"

Entity::Entity(const std::string & scriptPath) : GameObject(scriptPath) {
	// Default values
	movementDirection = Directions::NotSet;
	spriteDirection = Directions::Right;
	isCrouching = false;
	isJumping = false;
	isFalling = false;

	if (script->isScriptLoaded) {
		animationState = AnimationState::AnimationIdleRight;

		// Texture Setup
		int indexOfTexture = -1;
		std::string textureName = script->Get<std::string>("entity.texture");
		// Find the Texture for the Player.
		if ((indexOfTexture = Engine_Pointer->GetIndexOfTexture(textureName)) != -1) {
			// Desired text was found, set the texture.
			texture = &Engine_Pointer->textureRegister[indexOfTexture];
		}
		else {
			// Texture could not be found, use the default.
			texture = &Engine_Pointer->textureRegister[Engine_Pointer->indexOfDefaultTexture];
		}

		position = glm::vec3(script->Get<float>("entity.position.x"), script->Get<float>("entity.position.y"), script->Get<float>("entity.position.z"));
		gridPosition = Engine_Pointer->ConvertToGridPosition(glm::vec2(this->position.x, this->position.y));
		drawOffset = glm::ivec2(script->Get<int>("entity.draw_offset.x"), script->Get<int>("entity.draw_offset.y"));
		drawPosition = this->position + glm::vec3(this->drawOffset, 0);

		boundingBoxOffset = glm::vec2(script->Get<int>("entity.bounding_box_offset.x"), script->Get<int>("entity.bounding_box_offset.y"));
		glm::vec2 boundingBoxDimensions = glm::vec2(script->Get<int>("entity.bounding_box_dimensions.width"), script->Get<int>("entity.bounding_box_dimensions.height"));
		boundingBox = BoundingBox(glm::vec2(this->position.x, this->position.y) + boundingBoxOffset, boundingBoxDimensions);

		// Model Setup
		model = Engine_Pointer->modelRegister[Engine_Pointer->indexOfSpriteModel];
		model.SetMeshParents();
		model.Translate(drawPosition);
		model.Rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f));
		model.Scale(scale);

		sourceFrameSize = Engine_Pointer->entityTextureFrameSize;
	}
}
Entity::~Entity(void) {

}

void Entity::Update(const float& deltaTime) {
	EntityController();
	ActionController(deltaTime);

	// Calls the base class update.
	GameObject::Update(deltaTime);
}
void Entity::ActionHandlerCrouching(const float& deltaTime) {
	if (isCrouching) {
		// Amend Bounding Box size and positions

	}
	else {
		// Check the position above the entity, if the position is a collision set crouching to true
	}
}
void Entity::ActionHandlerJumping(const float& deltaTime) {
	// If the entity is now crouched or falling, try and jump
	if (!isCrouching && !isFalling && isJumping ) {
		glm::vec2 newVelocity = glm::vec2(0.0f);
		glm::vec2 newPosition = glm::vec2(position);
		glm::vec2 newGridPosition = Engine_Pointer->ConvertToGridPosition(newPosition);
		BoundingBox newBoundingBox = BoundingBox(newPosition + boundingBoxOffset, boundingBox.GetDimensions());

		BoundingBox* topLeftOverlap = nullptr;
		BoundingBox* topRightOverlap = nullptr;
		BoundingBox* bottomLeftOverlap = nullptr;
		BoundingBox* bottomRightOverlap = nullptr;

		Level* currentLevel = Engine_Pointer->levelRegister[Engine_Pointer->indexCurrentLevel];

		newVelocity.y = currentJumpingSpeed * deltaTime;

		newPosition.y += newVelocity.y;
		newGridPosition = Engine_Pointer->ConvertToGridPosition(newPosition);
		newBoundingBox = BoundingBox(newPosition + boundingBoxOffset, boundingBox.GetDimensions());

		topLeftOverlap = currentLevel->GetTileBoundingBox(newBoundingBox.TopLeftGridPosition());
		topRightOverlap = currentLevel->GetTileBoundingBox(newBoundingBox.TopRightGridPosition());

		// Check above the entity, top left + top right
		bool isCollding = true;
		if (topLeftOverlap != nullptr && topRightOverlap != nullptr) {
			bool isTopLeftIntersecting = newBoundingBox.Intersect(*topLeftOverlap);
			bool isTopRightIntersecting = newBoundingBox.Intersect(*topRightOverlap);

			isCollding = (isTopLeftIntersecting || isTopRightIntersecting);
		}
		else {
			// If neither of the level position could be found, stop the entity falling due to them going outside world bounds.
			isJumping = false;
			newVelocity = glm::vec2(0.0f);
		}

		// If empty, initialise the jump, start with a high velocity upwards and decrease til 0
		if (!isCollding) {
			
			velocity.y = newVelocity.y;
			currentJumpingSpeed +=2;
			if (currentJumpingSpeed >= 0) {
				isJumping = false;
				currentJumpingSpeed = 0.0f;
			}
		}
		else {
			// If the entity is not falling, reset the current falling speed to nothing
			currentJumpingSpeed = 0.0f;
			isJumping = false;
		}
	}
}
void Entity::ActionHandlerFalling(const float& deltaTime) {
	if (!isJumping) {
		glm::vec2 newVelocity = glm::vec2(0.0f);
		glm::vec2 newPosition = glm::vec2(position);
		glm::vec2 newGridPosition = Engine_Pointer->ConvertToGridPosition(newPosition);
		BoundingBox newBoundingBox = BoundingBox(newPosition + boundingBoxOffset, boundingBox.GetDimensions());

		BoundingBox* topLeftOverlap = nullptr;
		BoundingBox* topRightOverlap = nullptr;
		BoundingBox* bottomLeftOverlap = nullptr;
		BoundingBox* bottomRightOverlap = nullptr;

		Level* currentLevel = Engine_Pointer->levelRegister[Engine_Pointer->indexCurrentLevel];

		// Falling
		if (currentFallingSpeed < baseFallingSpeed) {
			newVelocity.y = baseFallingSpeed * deltaTime;
		}
		else {
			newVelocity.y = currentFallingSpeed * deltaTime;
		}
		newPosition.y += newVelocity.y;
		newGridPosition = Engine_Pointer->ConvertToGridPosition(newPosition);
		newBoundingBox = BoundingBox(newPosition + boundingBoxOffset, boundingBox.GetDimensions());

		bottomLeftOverlap = currentLevel->GetTileBoundingBox(newBoundingBox.BottomLeftGridPosition());
		bottomRightOverlap = currentLevel->GetTileBoundingBox(newBoundingBox.BottomRightGridPosition());

		if (bottomLeftOverlap != nullptr && bottomRightOverlap != nullptr) {
			bool isBottomLeftIntersecting = newBoundingBox.Intersect(*bottomLeftOverlap);
			bool isBottomRightIntersecting = newBoundingBox.Intersect(*bottomRightOverlap);

			isFalling = !(isBottomLeftIntersecting || isBottomRightIntersecting);
		}
		else {
			// If neither of the level position could be found, stop the entity falling due to them going outside world bounds.
			isFalling = false;
			newVelocity = glm::vec2(0.0f);
		}

		// If the entity is falling
		if (isFalling) {
			// Set the velocity, to the new calculated velocity
			velocity.y = newVelocity.y;

			// If the currentFallingSpeed hasn't yet been set to the base value
			if (currentFallingSpeed < baseFallingSpeed) {
				currentFallingSpeed = baseFallingSpeed;
			}

			// Increment the falling speed
			currentFallingSpeed +=2;

			// If the falling speed has gone past the max falling speed, set it back to max
			if (currentFallingSpeed > maxFallingSpeed) {
				currentFallingSpeed = maxFallingSpeed;
			}
		}
		else {
			// If the entity is not falling, reset the current falling speed to nothing
			currentFallingSpeed = 0.0f;
		}
	}
}
void Entity::ActionHandlerWalking(const float& deltaTime) {
	glm::vec2 newVelocity = glm::vec2(0.0f);
	glm::vec2 newPosition = glm::vec2(position);
	glm::vec2 newGridPosition = Engine_Pointer->ConvertToGridPosition(newPosition);
	BoundingBox newBoundingBox = BoundingBox(newPosition + boundingBoxOffset, boundingBox.GetDimensions());

	BoundingBox* topLeftOverlap = nullptr;
	BoundingBox* topRightOverlap = nullptr;
	BoundingBox* bottomLeftOverlap = nullptr;
	BoundingBox* bottomRightOverlap = nullptr;

	Level* currentLevel = Engine_Pointer->levelRegister[Engine_Pointer->indexCurrentLevel];

	// Left & Right
	if (movementDirection == Directions::Left) {
		newVelocity.x = -currentMovementSpeed * deltaTime;
		newPosition.x += newVelocity.x;
		newGridPosition = Engine_Pointer->ConvertToGridPosition(newPosition);
		newBoundingBox = BoundingBox(newPosition + boundingBoxOffset, boundingBox.GetDimensions());

		topLeftOverlap = currentLevel->GetTileBoundingBox(newBoundingBox.TopLeftGridPosition());
		bottomLeftOverlap = currentLevel->GetTileBoundingBox(newBoundingBox.BottomLeftGridPosition());
		bool isColliding = true;

		if (topLeftOverlap != nullptr && bottomLeftOverlap != nullptr) {
			bool isTopLeftIntersecting = newBoundingBox.Intersect(*topLeftOverlap);
			bool isBottomLeftIntersecting = newBoundingBox.Intersect(*bottomLeftOverlap);
			isColliding = (isTopLeftIntersecting || isBottomLeftIntersecting);
		}

		if (!isColliding) {
			velocity.x = newVelocity.x;
			newVelocity = glm::vec2(0.0f);
		}
	}
	else if (movementDirection == Directions::Right) {
		newVelocity.x = currentMovementSpeed * deltaTime;
		newPosition.x += newVelocity.x;
		newGridPosition = Engine_Pointer->ConvertToGridPosition(newPosition);
		newBoundingBox = BoundingBox(newPosition + boundingBoxOffset, boundingBox.GetDimensions());

		topRightOverlap = currentLevel->GetTileBoundingBox(newBoundingBox.TopRightGridPosition());
		bottomRightOverlap = currentLevel->GetTileBoundingBox(newBoundingBox.BottomRightGridPosition());
		bool isColliding = true;

		if (topRightOverlap != nullptr && bottomRightOverlap != nullptr) {
			bool isTopRightIntersecting = newBoundingBox.Intersect(*topRightOverlap);
			bool isBottomRightIntersecting = newBoundingBox.Intersect(*bottomRightOverlap);
			isColliding = (isTopRightIntersecting || isBottomRightIntersecting);
		}

		if (!isColliding) {
			velocity.x = newVelocity.x;
			newVelocity = glm::vec2(0.0f);
		}
	}
}
void Entity::ActionController(const float& deltaTime) {
	ActionHandlerCrouching(deltaTime);
	ActionHandlerJumping(deltaTime);
	ActionHandlerFalling(deltaTime);
	ActionHandlerWalking(deltaTime);

}
void Entity::UpdateAnimationState(void) {
	switch (spriteDirection) {
	case Directions::Up:
		animationState = AnimationState::AnimationJumping;
		break;
	case Directions::Down:
		animationState = AnimationState::AnimationFalling;
		break;
	case Directions::Left:
		(velocity.x != 0.0f) ? animationState = AnimationState::AnimationMoveLeft : animationState = AnimationState::AnimationIdleLeft;
		break;
	case Directions::Right:
		(velocity.x != 0.0f) ? animationState = AnimationState::AnimationMoveRight : animationState = AnimationState::AnimationIdleRight;
		break;
	default:
		break;
	}
}
void Entity::UpdateAnimationIndex(void) {
	// Using the current animaiton state, work out which animation in the list to use.
	switch (animationState) {
	case AnimationState::AnimationIdleLeft:
		animationIndex = 0;
		break;
	case AnimationState::AnimationIdleRight:
		animationIndex = 1;
		break;
	case AnimationState::AnimationMoveLeft:
		animationIndex = 2;
		break;
	case AnimationState::AnimationMoveRight:
		animationIndex = 3;
		break;
	default:
		break;
	}
}
