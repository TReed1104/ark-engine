#include "Entity.h"
#include "Engine.h"

Entity::Entity(const std::string & scriptPath) : GameObject(scriptPath) {
	// Default values
	movementDirection = Directions::NotSet;
	spriteDirection = Directions::Down;
	if (script->isScriptLoaded) {
		// Texture Setup
		int indexOfTexture = -1;
		std::string textureName = script->Get<std::string>("entity.texture");
		// Find the Texture for the Player.
		for (size_t i = 0; i < Engine_Pointer->textureRegister.size(); i++) {
			if (Engine_Pointer->textureRegister[i].name.find(textureName) != std::string::npos) {
				indexOfTexture = i;
				texture = &Engine_Pointer->textureRegister[indexOfTexture];
			}
		}
		if (indexOfTexture == -1) {
			texture = &Engine_Pointer->textureRegister[Engine_Pointer->indexOfDefaultTexture];
		}
		sourceFrameSize = glm::vec2(script->Get<float>("entity.source_rectangle_dimensions.width"), script->Get<float>("entity.source_rectangle_dimensions.height"));

		position = glm::vec3(script->Get<float>("entity.position.x"), script->Get<float>("entity.position.y"), script->Get<float>("entity.position.z"));
		gridPosition = Engine_Pointer->ConvertToGridPosition(glm::vec2(this->position.x, this->position.y));
		drawOffset = glm::vec2(script->Get<float>("entity.draw_offset.x"), script->Get<float>("entity.draw_offset.y"));
		drawPosition = this->position + glm::vec3(this->drawOffset, 0);

		movementSpeed = script->Get<float>("entity.movement_speed");
		boundingBoxOffset = glm::vec2(script->Get<float>("entity.bounding_box_offset.x"), script->Get<float>("entity.bounding_box_offset.y"));
		glm::vec2 boundingBoxDimensions = glm::vec2(script->Get<float>("entity.bounding_box_dimensions.width"), script->Get<float>("entity.bounding_box_dimensions.height"));
		boundingBox = BoundingBox(glm::vec2(this->position.x, this->position.y) + boundingBoxOffset, boundingBoxDimensions);

		// Model Setup
		model = Engine_Pointer->modelRegister[Engine_Pointer->indexOfSpriteModel];
		model.SetMeshParents();
		model.Translate(drawPosition);
		model.Rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f));
		model.Scale(scale);
	}
}
Entity::~Entity(void) {

}

void Entity::Update(const float& deltaTime) {
	HandleMovement();
	HandleCollisions(deltaTime);

	// Calls the base class update.
	GameObject::Update(deltaTime);
}
void Entity::HandleCollisions(float deltaTime) {
	glm::vec2 newVelocity = glm::vec2(0, 0);
	switch (movementDirection) {
		case Entity::NotSet:
			newVelocity = glm::vec2(0, 0);
			break;
		case Entity::Up:
			newVelocity = glm::vec2(0, -movementSpeed * deltaTime);
			break;
		case Entity::Down:
			newVelocity = glm::vec2(0, movementSpeed * deltaTime);
			break;
		case Entity::Left:
			newVelocity = glm::vec2(-movementSpeed * deltaTime, 0);
			break;
		case Entity::Right:
			newVelocity = glm::vec2(movementSpeed * deltaTime, 0);
			break;
		case Entity::UpLeft:
			newVelocity = glm::vec2(-movementSpeed * deltaTime, -movementSpeed * deltaTime);
			break;
		case Entity::UpRight:
			newVelocity = glm::vec2(movementSpeed * deltaTime, -movementSpeed * deltaTime);
			break;
		case Entity::DownLeft:
			newVelocity = glm::vec2(-movementSpeed * deltaTime, movementSpeed * deltaTime);
			break;
		case Entity::DownRight:
			newVelocity = glm::vec2(movementSpeed * deltaTime, movementSpeed * deltaTime);
			break;
		default:
			break;
	}
	if (movementDirection != Entity::NotSet) {
		// Calculate the new position and grid position
		glm::vec2 newPosition = glm::vec2(position.x, position.y) + newVelocity;
		glm::vec2 newGridPosition = Engine_Pointer->ConvertToGridPosition(glm::vec2(this->position.x, this->position.y));
		BoundingBox newBoundingBox = BoundingBox(newPosition + boundingBoxOffset, boundingBox.GetDimensions());

		// Check it is within the world bounds.
		if ((newBoundingBox.GetPosition().x >= 0) && (newBoundingBox.TopRightPosition().x < Engine_Pointer->levelRegister[Engine_Pointer->indexCurrentLevel]->pixelGridSize.x) &&
			(newBoundingBox.GetPosition().y >= 0) && (newBoundingBox.BottomRightPosition().y < Engine_Pointer->levelRegister[Engine_Pointer->indexCurrentLevel]->pixelGridSize.y)) {

			// Create a pointer to the current level
			Level* currentLevel = Engine_Pointer->levelRegister[Engine_Pointer->indexCurrentLevel];
			// Grab the bounding boxes of the locations of each of the four corners of the newBounding
			BoundingBox topLeftOverlap = currentLevel->GetTileBoundingBox(newBoundingBox.TopLeftGridPosition());
			BoundingBox topRightOverlap = currentLevel->GetTileBoundingBox(newBoundingBox.TopRightGridPosition());
			BoundingBox bottomLeftOverlap = currentLevel->GetTileBoundingBox(newBoundingBox.BottomLeftGridPosition());
			BoundingBox bottomRightOverlap = currentLevel->GetTileBoundingBox(newBoundingBox.BottomRightGridPosition());
			// Check if the new bounding box is intersecting with the four bounding boxes
			bool isTopLeftIntersecting = newBoundingBox.Intersect(topLeftOverlap);
			bool isTopRightIntersecting = newBoundingBox.Intersect(topRightOverlap);
			bool isBottomLeftIntersecting = newBoundingBox.Intersect(bottomLeftOverlap);
			bool isBottomRightIntersecting = newBoundingBox.Intersect(bottomRightOverlap);
			// See if any of the intersect checks were true.
			bool isColliding = (isTopLeftIntersecting || isTopRightIntersecting || isBottomLeftIntersecting || isBottomRightIntersecting);

			if (!isColliding) {
				velocity = newVelocity;
			}
			else {
				// If a collision was detected, we will snap the entity to the next available free position
				float deltaX = 0.0f;
				float deltaY = 0.0f;

				switch (movementDirection) {
					case Entity::Up:
					{
						// Calculate the Up Snap
						float offsetTopLeft = boundingBox.GetPosition().y - (topLeftOverlap.GetPosition().y + topLeftOverlap.GetDimensions().y);
						float offsetTopRight = boundingBox.GetPosition().y - (topRightOverlap.GetPosition().y + bottomLeftOverlap.GetDimensions().y);
						if (offsetTopLeft < offsetTopRight) { deltaY = -offsetTopLeft; }
						else { deltaY = -offsetTopRight; }
						break;
					}
					case Entity::Down:
					{
						// Calculate the Down snap
						float offsetBottomLeft = (boundingBox.GetPosition().y + boundingBox.GetDimensions().y) - bottomLeftOverlap.GetPosition().y;
						float offsetBottomRight = (boundingBox.GetPosition().y + boundingBox.GetDimensions().y) - bottomRightOverlap.GetPosition().y;
						if (offsetBottomLeft > offsetBottomRight) { deltaY = -offsetBottomLeft - 0.001f; }
						else { deltaY = -offsetBottomRight - 0.001f; }
						break;
					}
					case Entity::Left:
					{
						// Calculate the Left snap
						float offsetTopLeft = boundingBox.GetPosition().x - (topLeftOverlap.GetPosition().x + topLeftOverlap.GetDimensions().x);
						float offsetBottomLeft = boundingBox.GetPosition().x - (bottomLeftOverlap.GetPosition().x + bottomLeftOverlap.GetDimensions().x);
						if (offsetTopLeft < offsetBottomLeft) { deltaX = -offsetTopLeft; }
						else { deltaX = -offsetBottomLeft; }
						break;
					}
					case Entity::Right:
					{
						// Calculate the Right snap
						float offsetTopRight = (boundingBox.GetPosition().x + boundingBox.GetDimensions().x) - topRightOverlap.GetPosition().x;
						float offsetBottomRight = (boundingBox.GetPosition().x + boundingBox.GetDimensions().x) - bottomRightOverlap.GetPosition().x;
						if (offsetTopRight > offsetBottomRight) { deltaX = -offsetTopRight - 0.001f; }
						else { deltaX = -offsetBottomRight - 0.001f; }
						break;
					}
					case Entity::UpLeft:
					{
						bool isCollidingLeft = isTopLeftIntersecting && isBottomLeftIntersecting;
						bool isCollidingUp = isTopLeftIntersecting && isTopRightIntersecting;

						if (isCollidingLeft) {
							// Calculate the Left snap
							float offsetTopLeft = boundingBox.GetPosition().x - (topLeftOverlap.GetPosition().x + topLeftOverlap.GetDimensions().x);
							float offsetBottomLeft = boundingBox.GetPosition().x - (bottomLeftOverlap.GetPosition().x + bottomLeftOverlap.GetDimensions().x);
							if (offsetTopLeft < offsetBottomLeft) { deltaX = -offsetTopLeft; }
							else { deltaX = -offsetBottomLeft; }
						}
						else {
							movementDirection = Entity::Left;
							HandleCollisions(deltaTime);
						}
						if (isCollidingUp) {
							// Calculate the Up Snap
							float offsetTopLeft = boundingBox.GetPosition().y - (topLeftOverlap.GetPosition().y + topLeftOverlap.GetDimensions().y);
							float offsetTopRight = boundingBox.GetPosition().y - (topRightOverlap.GetPosition().y + bottomLeftOverlap.GetDimensions().y);
							if (offsetTopLeft < offsetTopRight) { deltaY = -offsetTopLeft; }
							else { deltaY = -offsetTopRight; }
						}
						else {
							movementDirection = Entity::Up;
							HandleCollisions(deltaTime);
						}
						break;
					}
					case Entity::UpRight:
					{
						bool isCollidingRight = isTopRightIntersecting && isBottomRightIntersecting;
						bool isCollidingUp = isTopLeftIntersecting && isTopRightIntersecting;

						if (isCollidingRight) {
							// Calculate the Right snap
							float offsetTopRight = (boundingBox.GetPosition().x + boundingBox.GetDimensions().x) - topRightOverlap.GetPosition().x;
							float offsetBottomRight = (boundingBox.GetPosition().x + boundingBox.GetDimensions().x) - bottomRightOverlap.GetPosition().x;
							if (offsetTopRight > offsetBottomRight) { deltaX = -offsetTopRight - 0.001f; }
							else { deltaX = -offsetBottomRight - 0.001f; }
						}
						else {
							movementDirection = Entity::Right;
							HandleCollisions(deltaTime);
						}
						if (isCollidingUp) {
							// Calculate the Up Snap
							float offsetTopLeft = boundingBox.GetPosition().y - (topLeftOverlap.GetPosition().y + topLeftOverlap.GetDimensions().y);
							float offsetTopRight = boundingBox.GetPosition().y - (topRightOverlap.GetPosition().y + bottomLeftOverlap.GetDimensions().y);
							if (offsetTopLeft < offsetTopRight) { deltaY = -offsetTopLeft; }
							else { deltaY = -offsetTopRight; }
						}
						else {
							movementDirection = Entity::Up;
							HandleCollisions(deltaTime);
						}
						break;
					}
					case Entity::DownLeft:
					{
						bool isCollidingLeft = isTopLeftIntersecting && isBottomLeftIntersecting;
						bool isCollidingDown = isBottomLeftIntersecting && isBottomRightIntersecting;

						if (isCollidingLeft) {
							// Calculate the Left snap
							float offsetTopLeft = boundingBox.GetPosition().x - (topLeftOverlap.GetPosition().x + topLeftOverlap.GetDimensions().x);
							float offsetBottomLeft = boundingBox.GetPosition().x - (bottomLeftOverlap.GetPosition().x + bottomLeftOverlap.GetDimensions().x);
							if (offsetTopLeft < offsetBottomLeft) { deltaX = -offsetTopLeft; }
							else { deltaX = -offsetBottomLeft; }
						}
						else {
							movementDirection = Entity::Left;
							HandleCollisions(deltaTime);
						}
						if (isCollidingDown) {
							// Calculate the Down snap
							float offsetBottomLeft = (boundingBox.GetPosition().y + boundingBox.GetDimensions().y) - bottomLeftOverlap.GetPosition().y;
							float offsetBottomRight = (boundingBox.GetPosition().y + boundingBox.GetDimensions().y) - bottomRightOverlap.GetPosition().y;
							if (offsetBottomLeft > offsetBottomRight) { deltaY = -offsetBottomLeft - 0.001f; }
							else { deltaY = -offsetBottomRight - 0.001f; }
						}
						else {
							movementDirection = Entity::Down;
							HandleCollisions(deltaTime);
						}
						break;
					}
					case Entity::DownRight:
					{
						bool isCollidingRight = isTopRightIntersecting && isBottomRightIntersecting;
						bool isCollidingDown = isBottomLeftIntersecting && isBottomRightIntersecting;

						if (isCollidingRight) {
							// Calculate the Right snap
							float offsetTopRight = (boundingBox.GetPosition().x + boundingBox.GetDimensions().x) - topRightOverlap.GetPosition().x;
							float offsetBottomRight = (boundingBox.GetPosition().x + boundingBox.GetDimensions().x) - bottomRightOverlap.GetPosition().x;
							if (offsetTopRight > offsetBottomRight) { deltaX = -offsetTopRight - 0.001f; }
							else { deltaX = -offsetBottomRight - 0.001f; }
						}
						else {
							movementDirection = Entity::Right;
							HandleCollisions(deltaTime);
						}
						if (isCollidingDown) {
							// Calculate the Down snap
							float offsetBottomLeft = (boundingBox.GetPosition().y + boundingBox.GetDimensions().y) - bottomLeftOverlap.GetPosition().y;
							float offsetBottomRight = (boundingBox.GetPosition().y + boundingBox.GetDimensions().y) - bottomRightOverlap.GetPosition().y;
							if (offsetBottomLeft > offsetBottomRight) { deltaY = -offsetBottomLeft - 0.001f; }
							else { deltaY = -offsetBottomRight - 0.001f; }
						}
						else {
							movementDirection = Entity::Down;
							HandleCollisions(deltaTime);
						}
						break;
					}
					default:
					{
						break;
					}
				}
				velocitySnap = glm::vec2(deltaX, deltaY);
			}
		}
	}
}
void Entity::AnimationHandler(void) {
	switch (spriteDirection) {
	case Directions::Up:
		(velocity != glm::vec2(0.0f, 0.0f)) ? animationState = AnimationState::MoveUp : animationState = AnimationState::IdleUp;
		break;
	case Directions::Down:
		(velocity != glm::vec2(0.0f, 0.0f)) ? animationState = AnimationState::MoveDown : animationState = AnimationState::IdleDown;
		break;
	case Directions::Left:
		(velocity != glm::vec2(0.0f, 0.0f)) ? animationState = AnimationState::MoveLeft : animationState = AnimationState::IdleLeft;
		break;
	case Directions::Right:
		(velocity != glm::vec2(0.0f, 0.0f)) ? animationState = AnimationState::MoveRight : animationState = AnimationState::IdleRight;
		break;
	default:
		break;
	}
}
