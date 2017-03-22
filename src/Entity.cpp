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

	glm::vec2 newPosition = glm::vec2(position.x, position.y) + newVelocity;
	glm::vec2 newGridPosition = EngineUtilities::ConvertToGridPosition(Engine_Pointer->tileSize, glm::vec2(position.x, position.y));
	BoundingBox newBoundingBox = BoundingBox(newPosition);

	velocity = newVelocity;
}
