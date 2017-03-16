#include "Entity.h"

Entity::Entity(const Engine& engine, const Model& model, const Texture& texture, const glm::vec3& position, const glm::vec2& sourceFrameSize) : GameObject(engine, model, texture, position, sourceFrameSize) {

}
Entity::~Entity() {

}

void Entity::Update(float deltaTime) {


	// Calls the base class update.
	GameObject::Update(deltaTime);
}