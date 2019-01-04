#include "Entity.h"
#include "Engine.h"

Entity::Entity(const std::string & scriptPath) : GameObject(scriptPath) {
	// Default values
	movementDirection = Directions::NotSet;
	spriteDirection = Directions::Right;
	isAffectedByGravity = true;
	isCrouching = false;
	isJumping = false;
	currentMovementSpeed = baseMovementSpeed;
	currentJumpingSpeed = 0.0f;
	jumpingTimer = 0.0f;

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

		standingBoundingBoxOffset = glm::vec2(script->Get<int>("entity.bounding_box_offset.x"), script->Get<int>("entity.bounding_box_offset.y"));
		glm::vec2 standingBoundingBoxDimensions = glm::vec2(script->Get<int>("entity.bounding_box_dimensions.width"), script->Get<int>("entity.bounding_box_dimensions.height"));
		standingBoundingBox = BoundingBox(glm::vec2(this->position.x, this->position.y) + standingBoundingBoxOffset, standingBoundingBoxDimensions);

		crouchingBoundingBoxOffset = glm::vec2(3, 24);
		glm::vec2 crouchingBoundingBoxDimensions = glm::vec2(10, 16);
		crouchingBoundingBox = BoundingBox(glm::vec2(this->position.x, this->position.y) + crouchingBoundingBoxOffset, crouchingBoundingBoxDimensions);

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
	GameObject::Update(deltaTime);
}
void Entity::UpdatePosition(void) {
	GameObject::UpdatePosition();
	standingBoundingBox.UpdatePosition(glm::vec2(position.x, position.y) + standingBoundingBoxOffset);
	crouchingBoundingBox.UpdatePosition(glm::vec2(position.x, position.y) + crouchingBoundingBoxOffset);

}
void Entity::PhysicsHandlerCrouching(const float& deltaTime) {
	if (isCrouching) {
		// Amend Bounding Box size and positions
		boundingBox = crouchingBoundingBox;
		boundingBoxOffset = crouchingBoundingBoxOffset;
	}
	else {
		// Check the position above the entity, if the position is a collision set crouching to true
		boundingBox = standingBoundingBox;
		boundingBoxOffset = standingBoundingBoxOffset;
	}
}
void Entity::PhysicsHandlerJumping(const float& deltaTime) {
	// If the entity is not crouched or falling, try and jump
	if (!isFalling && isJumping) {
		// Declare the variables used for the calculations
		glm::vec2 newVelocity = glm::vec2(0.0f, currentJumpingSpeed * deltaTime);
		glm::vec2 newPosition = glm::vec2(position) + newVelocity;
		BoundingBox newBoundingBox = BoundingBox(newPosition + boundingBoxOffset, boundingBox.GetDimensions());
		bool isColliding = false;

		// Calculate the difference in grid cells between the top left of the AABB and the top right, giving us the AABB width in cells
		int deltaGridX = abs(newBoundingBox.TopLeftGridPosition().x - newBoundingBox.TopRightGridPosition().x);
		// For every delta, do another check until we run out of checks or find a collision
		for (size_t i = 0; i <= deltaGridX; i++) {
			glm::ivec2 gridPositionToCheck = newBoundingBox.TopLeftGridPosition() + glm::ivec2(i, 0);					// Position of the overlap
			BoundingBox* overlapToCheck = Engine_Pointer->GetCurrentLevel()->GetTileBoundingBox(gridPositionToCheck);	// The AABB of the grid cell
			if (overlapToCheck != nullptr) {
				isColliding = newBoundingBox.Intersect(*overlapToCheck);													// Do the actual intersection check
				if (isColliding) {
					// If we have found a collision, break out the loop because no more checks are needed
					break;
				}
			}
			else {
				isColliding = true;
				break;
			}
		}

		// If there is no collision, Fall
		if (!isColliding) {
			// Apply physics
			velocity.y = newVelocity.y;

			// Work out the objects Jumping speed for the next update cycle
			jumpingTimer += deltaTime;
			if (jumpingTimer >= Engine_Pointer->physicsInterval) {
				float forceIncrement = (timeForMaxJump / Engine_Pointer->physicsInterval);	// Increment the Jumping force is applied at
				currentJumpingSpeed -= (baseJumpingSpeed / forceIncrement);
				jumpingTimer = 0.0f;
			}

			// The jumping upwards force has reached 0, allow a fall to begin
			if (currentJumpingSpeed >= 0) {
				currentJumpingSpeed = baseJumpingSpeed;
				isJumping = false;
				jumpingTimer = 0.0f;
			}
		}
		else {
			// If there was a collision, reset the jump variables
			currentJumpingSpeed = baseJumpingSpeed;
			isJumping = false;
			jumpingTimer = 0.0f;
		}
	}
}
void Entity::PhysicsHandlerFalling(const float& deltaTime) {
	if (!isJumping) {
		GameObject::PhysicsHandlerFalling(deltaTime);
	}
}
void Entity::PhysicsHandlerMovement(const float& deltaTime) {
	// Declare the variables used for the calculations
	glm::vec2 newVelocity = glm::vec2(0.0f);
	glm::vec2 newPosition = glm::vec2(position);
	BoundingBox newBoundingBox = BoundingBox(newPosition + boundingBoxOffset, boundingBox.GetDimensions());

	// Left & Right
	if (movementDirection == Directions::Left) {
		// Set the values used for the calculations
		newVelocity.x = -currentMovementSpeed * deltaTime;
		newPosition.x += newVelocity.x;
		newBoundingBox = BoundingBox(newPosition + boundingBoxOffset, boundingBox.GetDimensions());
		bool isColliding = false;

		// Calculate the difference in grid cells between the top of the AABB and the bottom
		int deltaGridY = abs(newBoundingBox.TopLeftGridPosition().y - newBoundingBox.BottomLeftGridPosition().y);
		// For every delta, do another check until we run out of checks or find a collision
		for (size_t i = 0; i <= deltaGridY; i++) {
			glm::ivec2 gridPositionToCheck = newBoundingBox.TopLeftGridPosition() + glm::ivec2(0, i);					// Position of the overlap
			BoundingBox* overlapToCheck = Engine_Pointer->GetCurrentLevel()->GetTileBoundingBox(gridPositionToCheck);	// The AABB of the position
			if (overlapToCheck != nullptr) {
				isColliding = newBoundingBox.Intersect(*overlapToCheck);	// Do the actual intersection check
				if (isColliding) {
					// If we have found a collision, break out the loop because no more checks are needed
					break;
				}
			}
			else {
				isColliding = true;
				break;
			}
		}

		// If there is no collision, Move Left
		if (!isColliding) {
			velocity.x = newVelocity.x;
		}
	}
	else if (movementDirection == Directions::Right) {
		// Set the values used for the calculations
		newVelocity.x = currentMovementSpeed * deltaTime;
		newPosition.x += newVelocity.x;
		newBoundingBox = BoundingBox(newPosition + boundingBoxOffset, boundingBox.GetDimensions());
		bool isColliding = false;

		// Calculate the difference in grid cells between the top of the AABB and the bottom
		int deltaGridY = abs(newBoundingBox.TopRightGridPosition().y - newBoundingBox.BottomRightGridPosition().y);
		// For every delta, do another check until we run out of checks or find a collision
		for (size_t i = 0; i <= deltaGridY; i++) {
			glm::ivec2 gridPositionToCheck = newBoundingBox.TopRightGridPosition() + glm::ivec2(0, i);					// Position of the overlap
			BoundingBox* overlapToCheck = Engine_Pointer->GetCurrentLevel()->GetTileBoundingBox(gridPositionToCheck);	// The AABB of the position
			if (overlapToCheck != nullptr) {
				isColliding = newBoundingBox.Intersect(*overlapToCheck);	// Do the actual intersection check
				if (isColliding) {
					// If we have found a collision, break out the loop because no more checks are needed
					break;
				}
			}
			else {
				isColliding = true;
				break;
			}
		}

		// If there is no collision, Move Right
		if (!isColliding) {
			velocity.x = newVelocity.x;
		}
	}
}
void Entity::PhysicsController(const float& deltaTime) {
	PhysicsHandlerCrouching(deltaTime);
	PhysicsHandlerJumping(deltaTime);
	PhysicsHandlerFalling(deltaTime);
	PhysicsHandlerMovement(deltaTime);
}
void Entity::AnimationStateHandler(void) {
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
void Entity::AnimationIndexHandler(void) {
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
