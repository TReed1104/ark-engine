#include "Tile.h"
#include "Engine.h"

Tile::Tile(const Texture& texture, const int& tileType, const glm::ivec2& sourceFramePosition, const glm::vec3& position, const BoundingBox& boundingBox, const glm::ivec2& boundingBoxOffset, const bool& isSlope, const glm::ivec2& slopeOffset) : GameObject() {
	this->name = "Tile";
	this->type = (Type)tileType;
	// Set the tile name
	if (type == Type::Solid) {
		this->name += " - Solid";
	}
	else {
		this->name += " - Walkable";
	}

	this->isSlope = isSlope;
	if (this->isSlope) {
		this->name += " - Sloped";
		this->slopeOffset = slopeOffset;
		this->slopeAngle = CalculateSlope(this->slopeOffset);
	}
	else {
		this->slopeOffset = glm::ivec2(0);
		this->slopeAngle = 0.0f;
	}

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
float Tile::CalculateSlope(const glm::ivec2& slopeOffset) {
	glm::ivec2 left = glm::ivec2(position) + glm::ivec2(0, slopeOffset.x);
	glm::ivec2 right = glm::ivec2(position.x + Engine_Pointer->tileSize.x, position.y) + glm::ivec2(0, slopeOffset.y);

	// Delta of each side where the slope is
	int deltaX = right.x - left.x;
	int deltaY = right.y - left.y;
	float slopeAngle = std::atan2(deltaY, deltaX) * 180 / 3.14159f;	// Calculate the slope

	return slopeAngle;
}
