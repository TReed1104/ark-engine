#include "Tile.h"

Tile::Tile(const Model& model, const Texture& texture, const std::string& tileType, const glm::vec2& sourceFramePosition, const glm::vec3& position, const glm::vec2& sourceFrameSize) : GameObject(model, texture, position, sourceFrameSize) {
	this->sourceFramePosition = sourceFramePosition;
}
Tile::~Tile() {

}

void Tile::Update(float deltaTime) {

	velocity = glm::vec2(0.0f, 0.0f);
	// Class the base GameObject Update
	GameObject::Update(deltaTime);
}
