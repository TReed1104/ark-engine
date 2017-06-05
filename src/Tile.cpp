#include "Tile.h"
#include "Engine.h"

Tile::Tile(const Texture& texture, const int& tileType, const glm::ivec2& sourceFramePosition, const glm::vec3& position) : GameObject() {
	this->type = (Type)tileType;
	this->texture = &texture;
	this->sourceFramePosition = sourceFramePosition;
	this->position = position;
	this->drawOffset = glm::vec2(0.0f);
	this->drawPosition = this->position + glm::vec3(this->drawOffset, 0);
	this->gridPosition = Engine_Pointer->ConvertToGridPosition(glm::vec2(this->position.x, this->position.y));
	this->boundingBoxOffset = glm::vec2(0.0f);
	this->boundingBox = BoundingBox(glm::vec2(this->position.x, this->position.y) + boundingBoxOffset);

	// Model Setup
	this->model = Engine_Pointer->modelRegister[Engine_Pointer->indexOfTileModel];
	this->model.SetMeshParents();
	this->model.Translate(drawPosition);
	this->model.Rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	this->model.Scale(scale);
}
Tile::~Tile(void) {

}

void Tile::Update(const float& deltaTime) {
	velocity = glm::vec2(0.0f, 0.0f);
	// Class the base GameObject Update
	GameObject::Update(deltaTime);
}
