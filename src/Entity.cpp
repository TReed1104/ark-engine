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
