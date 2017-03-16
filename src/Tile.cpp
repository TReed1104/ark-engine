#include "Tile.h"

Tile::Tile(const Engine& engine, const Model& model, const Texture& texture, const std::string& tileType, const glm::vec2& sourceFramePosition, const glm::vec3& position, const glm::vec2& sourceFrameSize) : GameObject(engine, model, texture, position, sourceFrameSize) {
	this->sourceFramePosition = sourceFramePosition;
}
Tile::~Tile() {

}

void Tile::Update(float deltaTime) {


	// Class the base GameObject Update
	GameObject::Update(deltaTime);
}
