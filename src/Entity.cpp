#include "Entity.h"
#include "Engine.h"

Entity::Entity(const std::string & scriptPath) : GameObject(scriptPath) {
	// Default values
	movementDirection = Directions::Right;
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

		baseMovementSpeed = script->Get<float>("entity.movement_speed");
		currentMovementSpeed = baseMovementSpeed;
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
	MovementController(deltaTime);

	// Calls the base class update.
	GameObject::Update(deltaTime);
}
void Entity::MovementController(const float & deltaTime) {
	glm::vec2 newVelocity = glm::vec2(0.0f);
	glm::vec2 newPosition = glm::vec2(position);
	glm::vec2 newGridPosition = Engine_Pointer->ConvertToGridPosition(newPosition);
	BoundingBox newBoundingBox = BoundingBox(newPosition + boundingBoxOffset, boundingBox.GetDimensions());

	Level* currentLevel = Engine_Pointer->levelRegister[Engine_Pointer->indexCurrentLevel];

	// Falling
	newVelocity.y = currentFallingSpeed * deltaTime;
	newPosition += newVelocity;
	newGridPosition = Engine_Pointer->ConvertToGridPosition(newPosition);
	newBoundingBox = BoundingBox(newPosition + boundingBoxOffset, boundingBox.GetDimensions());

	BoundingBox* bottomLeftOverlap = currentLevel->GetTileBoundingBox(newBoundingBox.BottomLeftGridPosition());
	BoundingBox* bottomRightOverlap = currentLevel->GetTileBoundingBox(newBoundingBox.BottomRightGridPosition());

	if (bottomLeftOverlap != nullptr && bottomRightOverlap != nullptr) {
		bool isBottomLeftIntersecting = newBoundingBox.Intersect(*bottomLeftOverlap);
		bool isBottomRightIntersecting = newBoundingBox.Intersect(*bottomRightOverlap);

		isFalling = (isBottomLeftIntersecting || isBottomRightIntersecting);
	}
	if (!isFalling) {
		velocity.y = newVelocity.y;
	}

}
void Entity::UpdateAnimationState(void) {
	switch (movementDirection) {
	case Directions::Up:
		animationState = AnimationState::AnimationJumping;
		break;
	case Directions::Down:
		animationState = AnimationState::AnimationFalling;
		break;
	case Directions::Left:
		(velocity != glm::vec2(0.0f, 0.0f)) ? animationState = AnimationState::AnimationMoveLeft : animationState = AnimationState::AnimationIdleLeft;
		break;
	case Directions::Right:
		(velocity != glm::vec2(0.0f, 0.0f)) ? animationState = AnimationState::AnimationMoveRight : animationState = AnimationState::AnimationIdleRight;
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
