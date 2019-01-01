#include "Entity.h"
#include "Engine.h"

Entity::Entity(const std::string & scriptPath) : GameObject(scriptPath) {
	// Default values
	movementDirection = Directions::NotSet;
	spriteDirection = Directions::Right;
	isCrouching = false;
	canFall = true;
	canJump = true;

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

	// Calls the base class update.
	GameObject::Update(deltaTime);
}
void Entity::PhysicsHandlerCrouching(const float& deltaTime) {
	if (isCrouching) {
		// Amend Bounding Box size and positions

	}
	else {
		// Check the position above the entity, if the position is a collision set crouching to true
	}
}
void Entity::PhysicsHandlerMovement(const float& deltaTime) {
	// Declare the variables used for the calculations
	glm::vec2 newVelocity = glm::vec2(0.0f);
	glm::vec2 newPosition = glm::vec2(position);
	BoundingBox newBoundingBox = BoundingBox(newPosition + boundingBoxOffset, boundingBox.GetDimensions());
	BoundingBox* topLeftOverlap = nullptr;
	BoundingBox* topRightOverlap = nullptr;
	BoundingBox* bottomLeftOverlap = nullptr;
	BoundingBox* bottomRightOverlap = nullptr;

	// Left & Right
	if (movementDirection == Directions::Left) {
		// Set the values used for the calculations
		newVelocity.x = -currentMovementSpeed * deltaTime;
		newPosition.x += newVelocity.x;
		newBoundingBox = BoundingBox(newPosition + boundingBoxOffset, boundingBox.GetDimensions());
		topLeftOverlap = Engine_Pointer->GetCurrentLevel()->GetTileBoundingBox(newBoundingBox.TopLeftGridPosition());
		bottomLeftOverlap = Engine_Pointer->GetCurrentLevel()->GetTileBoundingBox(newBoundingBox.BottomLeftGridPosition());

		// Check for a collision
		bool isColliding = true;
		if (topLeftOverlap != nullptr && bottomLeftOverlap != nullptr) {
			bool isTopLeftIntersecting = newBoundingBox.Intersect(*topLeftOverlap);
			bool isBottomLeftIntersecting = newBoundingBox.Intersect(*bottomLeftOverlap);
			isColliding = (isTopLeftIntersecting || isBottomLeftIntersecting);
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
		topRightOverlap = Engine_Pointer->GetCurrentLevel()->GetTileBoundingBox(newBoundingBox.TopRightGridPosition());
		bottomRightOverlap = Engine_Pointer->GetCurrentLevel()->GetTileBoundingBox(newBoundingBox.BottomRightGridPosition());

		// Check for a collision
		bool isColliding = true;
		if (topRightOverlap != nullptr && bottomRightOverlap != nullptr) {
			bool isTopRightIntersecting = newBoundingBox.Intersect(*topRightOverlap);
			bool isBottomRightIntersecting = newBoundingBox.Intersect(*bottomRightOverlap);
			isColliding = (isTopRightIntersecting || isBottomRightIntersecting);
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
