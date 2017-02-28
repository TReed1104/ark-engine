#include "Tile.h"

Tile::Tile(const Engine& engine, const Model& model, const glm::vec3& position, const char* texturePath, const glm::vec2& sourceFrameSize) : GameObject(engine, model, position, texturePath, sourceFrameSize) {

}
Tile::~Tile() {

}

void Tile::Update(float deltaTime) {


	// Class the base GameObject Update
	GameObject::Update(deltaTime);
}
