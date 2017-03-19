#include "Entity.h"

Entity::Entity(const Model& model, const Texture& texture, const glm::vec3& position, const glm::vec2& sourceFrameSize) : GameObject(model, texture, position, sourceFrameSize) {
	movementDirection = Entity::Down;
}
Entity::~Entity() {

}

void Entity::Update(float deltaTime) {
	HandleMovement();
	HandleCollisions();

	// Calls the base class update.
	GameObject::Update(deltaTime);
}
