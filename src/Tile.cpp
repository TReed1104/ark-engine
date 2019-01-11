#include "Tile.h"
#include "Engine.h"

Tile::Tile(const Texture& texture, const int& tileType, const glm::ivec2& sourceFramePosition, const glm::vec3& position, 
			const BoundingBox& boundingBox, const glm::ivec2& boundingBoxOffset, 
				const bool& isSlope, const glm::ivec2& slopeOffset) : GameObject() {

	this->type = (Type)tileType;
	this->isSlope = isSlope;
	this->slopeOffset = slopeOffset;

	// Texturess
	this->texture = &texture;
	this->sourceFramePosition = sourceFramePosition;

	// Transformation override
	this->position = position;
	this->gridPosition = Engine_Pointer->ConvertToGridPosition(glm::vec2(this->position.x, this->position.y));
	this->drawPosition = this->position + glm::vec3(this->drawOffset, 0);

	// Collisions
	this->boundingBox = boundingBox;
	this->boundingBoxOffset = boundingBoxOffset;
	
	// Model Setup
	model = Engine_Pointer->modelRegister[Engine_Pointer->indexOfTileModel];
	model.SetMeshParents();
	model.Translate(drawPosition);
	model.Rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	model.Scale(scale);
}
Tile::~Tile(void) {

}

void Tile::Update(const float& deltaTime) {
	GameObject::Update(deltaTime);
}
