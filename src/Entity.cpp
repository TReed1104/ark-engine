#include "Entity.h"

Entity::Entity(const Model& model, const Texture& texture, const glm::vec3& position, const glm::vec2& sourceFrameSize) : GameObject(model, texture, position, sourceFrameSize) {
	movementDirection = Entity::NotSet;
	spriteDirection = Entity::Down;
	movementSpeed = 60.0f;
}
Entity::~Entity() {

}

void Entity::Update(float deltaTime) {
	HandleMovement();
	HandleCollisions();

	drawPosition += glm::vec3(velocity * deltaTime, 0.0f);

	// Calls the base class update.
	GameObject::Update(deltaTime);
}

void Entity::HandleCollisions(void) {

	glm::vec2 newVelocity = glm::vec2(0, 0);
	switch (movementDirection) {
		case Entity::NotSet:
			newVelocity = glm::vec2(0, 0);
			break;
		case Entity::Up:
			newVelocity = glm::vec2(0, -movementSpeed);
			break;
		case Entity::Down:
			newVelocity = glm::vec2(0, movementSpeed);
			break;
		case Entity::Left:
			newVelocity = glm::vec2(-movementSpeed, 0);
			break;
		case Entity::Right:
			newVelocity = glm::vec2(movementSpeed, 0);
			break;
		case Entity::UpLeft:
			newVelocity = glm::vec2(-movementSpeed, -movementSpeed);
			break;
		case Entity::UpRight:
			newVelocity = glm::vec2(movementSpeed, -movementSpeed);
			break;
		case Entity::DownLeft:
			newVelocity = glm::vec2(-movementSpeed, movementSpeed);
			break;
		case Entity::DownRight:
			newVelocity = glm::vec2(movementSpeed, movementSpeed);
			break;
		default:
			break;
	}

	velocity = newVelocity;
}
