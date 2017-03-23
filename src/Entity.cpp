#include "Entity.h"
#include "Engine.h"

Entity::Entity(const Model& model, const Texture& texture, const glm::vec3& position, const glm::vec2& sourceFrameSize) : GameObject(model, texture, position, sourceFrameSize) {
	movementDirection = Entity::NotSet;
	spriteDirection = Entity::Down;
	movementSpeed = 60.0f;
}
Entity::~Entity() {

}

void Entity::Update(const float& deltaTime) {
	HandleMovement();
	HandleCollisions(deltaTime);

	position += glm::vec3(velocity, 0.0f);
	position += glm::vec3(velocityForTileSnap, 0.0f);
	boundingBox.UpdatePosition(glm::vec2(position.x, position.y));
	drawPosition = (position + glm::vec3(drawOffset, 0.0f));

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
		BoundingBox newBoundingBox = BoundingBox(newPosition);

		// Check it is within the world bounds.
		if ((newPosition.x >= 0) && (newBoundingBox.TopRightPosition().x < Engine_Pointer->levelRegister[Engine_Pointer->indexCurrentLevel]->pixelGridSize.x) &&
			(newPosition.y >= 0) && (newBoundingBox.BottomRightPosition().y < Engine_Pointer->levelRegister[Engine_Pointer->indexCurrentLevel]->pixelGridSize.y)) {

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
						float offsetTopLeft = position.y - (topLeftOverlap.GetPosition().y + topLeftOverlap.GetDimensions().y);
						float offsetTopRight = position.y - (topRightOverlap.GetPosition().y + bottomLeftOverlap.GetDimensions().y);
						if (offsetTopLeft < offsetTopRight) { deltaY = -offsetTopLeft; }
						else { deltaY = -offsetTopRight; }
						break;
					}
					case Entity::Down:
					{
						// Calculate the Down snap
						float offsetBottomLeft = (position.y + boundingBox.GetDimensions().y) - bottomLeftOverlap.GetPosition().y;
						float offsetBottomRight = (position.y + boundingBox.GetDimensions().y) - bottomRightOverlap.GetPosition().y;
						if (offsetBottomLeft > offsetBottomRight) { deltaY = -offsetBottomLeft - 0.001f; }
						else { deltaY = -offsetBottomRight - 0.001f; }
						break;
					}
					case Entity::Left:
					{
						// Calculate the Left snap
						float offsetTopLeft = position.x - (topLeftOverlap.GetPosition().x + topLeftOverlap.GetDimensions().x);
						float offsetBottomLeft = position.x - (bottomLeftOverlap.GetPosition().x + bottomLeftOverlap.GetDimensions().x);
						if (offsetTopLeft < offsetBottomLeft) { deltaX = -offsetTopLeft; }
						else { deltaX = -offsetBottomLeft; }
						break;
					}
					case Entity::Right:
					{
						// Calculate the Right snap
						float offsetTopRight = (position.x + boundingBox.GetDimensions().x) - topRightOverlap.GetPosition().x;
						float offsetBottomRight = (position.x + boundingBox.GetDimensions().x) - bottomRightOverlap.GetPosition().x;
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
							float offsetTopLeft = position.x - (topLeftOverlap.GetPosition().x + topLeftOverlap.GetDimensions().x);
							float offsetBottomLeft = position.x - (bottomLeftOverlap.GetPosition().x + bottomLeftOverlap.GetDimensions().x);
							if (offsetTopLeft < offsetBottomLeft) { deltaX = -offsetTopLeft; }
							else { deltaX = -offsetBottomLeft; }
						}
						else {
							movementDirection = Entity::Left;
							HandleCollisions(deltaTime);
						}
						if (isCollidingUp) {
							// Calculate the Up Snap
							float offsetTopLeft = position.y - (topLeftOverlap.GetPosition().y + topLeftOverlap.GetDimensions().y);
							float offsetTopRight = position.y - (topRightOverlap.GetPosition().y + bottomLeftOverlap.GetDimensions().y);
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
							float offsetTopRight = (position.x + boundingBox.GetDimensions().x) - topRightOverlap.GetPosition().x;
							float offsetBottomRight = (position.x + boundingBox.GetDimensions().x) - bottomRightOverlap.GetPosition().x;
							if (offsetTopRight > offsetBottomRight) { deltaX = -offsetTopRight - 0.001f; }
							else { deltaX = -offsetBottomRight - 0.001f; }
						}
						else {
							movementDirection = Entity::Right;
							HandleCollisions(deltaTime);
						}
						if (isCollidingUp) {
							// Calculate the Up Snap
							float offsetTopLeft = position.y - (topLeftOverlap.GetPosition().y + topLeftOverlap.GetDimensions().y);
							float offsetTopRight = position.y - (topRightOverlap.GetPosition().y + bottomLeftOverlap.GetDimensions().y);
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
							float offsetTopLeft = position.x - (topLeftOverlap.GetPosition().x + topLeftOverlap.GetDimensions().x);
							float offsetBottomLeft = position.x - (bottomLeftOverlap.GetPosition().x + bottomLeftOverlap.GetDimensions().x);
							if (offsetTopLeft < offsetBottomLeft) { deltaX = -offsetTopLeft; }
							else { deltaX = -offsetBottomLeft; }
						}
						else {
							movementDirection = Entity::Left;
							HandleCollisions(deltaTime);
						}
						if (isCollidingDown) {
							// Calculate the Down snap
							float offsetBottomLeft = (position.y + boundingBox.GetDimensions().y) - bottomLeftOverlap.GetPosition().y;
							float offsetBottomRight = (position.y + boundingBox.GetDimensions().y) - bottomRightOverlap.GetPosition().y;
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
							float offsetTopRight = (position.x + boundingBox.GetDimensions().x) - topRightOverlap.GetPosition().x;
							float offsetBottomRight = (position.x + boundingBox.GetDimensions().x) - bottomRightOverlap.GetPosition().x;
							if (offsetTopRight > offsetBottomRight) { deltaX = -offsetTopRight - 0.001f; }
							else { deltaX = -offsetBottomRight - 0.001f; }
						}
						else {
							movementDirection = Entity::Right;
							HandleCollisions(deltaTime);
						}
						if (isCollidingDown) {
							// Calculate the Down snap
							float offsetBottomLeft = (position.y + boundingBox.GetDimensions().y) - bottomLeftOverlap.GetPosition().y;
							float offsetBottomRight = (position.y + boundingBox.GetDimensions().y) - bottomRightOverlap.GetPosition().y;
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
				velocityForTileSnap = glm::vec2(deltaX, deltaY);
			}
		}
	}
}
