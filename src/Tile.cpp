#include "Tile.h"

Tile::Tile(const Engine& engine, const Model& model, const char* texturePath, const std::string& tileType, const glm::vec2& sourceFramePosition, const glm::vec3& position, const glm::vec2& sourceFrameSize) : GameObject(engine, model, position, texturePath, sourceFrameSize) {
	this->sourceFramePosition = sourceFramePosition;
}
Tile::~Tile() {

}

void Tile::Update(float deltaTime) {


	// Class the base GameObject Update
	GameObject::Update(deltaTime);
}
