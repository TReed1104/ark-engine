#include "Tile.h"
#include "Engine.h"

Tile::Tile(const Tile& otherTile) : GameObject() {
	// copy constructor
	this->name = otherTile.name;
	this->type = otherTile.type;
	this->isSlope = otherTile.isSlope;
	this->slopeOffset = otherTile.slopeOffset;
	this->slopeAngle = otherTile.slopeAngle;
	this->texture = otherTile.texture;
	this->sourceFramePosition = otherTile.sourceFramePosition;
	this->position = otherTile.position;
	this->gridPosition = otherTile.gridPosition;
	this->drawOffset = otherTile.drawOffset;
	this->drawPosition = otherTile.drawPosition;
	this->boundingBoxOffset = otherTile.boundingBoxOffset;
	this->boundingBox = otherTile.boundingBox;
	this->model = otherTile.model;
	this->model.SetMeshParents();	// Configure the mesh
	this->isLoaded = otherTile.isLoaded;
}
Tile::Tile(Texture* texture, const int& tileType, const glm::ivec2& sourceFramePosition, const glm::vec3& position, const BoundingBox& boundingBox, const glm::ivec2& boundingBoxOffset, const bool& isSlope, const glm::ivec2& slopeOffset) : GameObject() {
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
	this->texture = texture;
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
	isLoaded = true;
}
Tile::~Tile(void) {

}

void Tile::Update(const float& deltaTime) {
	GameObject::Update(deltaTime);
}
std::map<std::string, std::string> Tile::ExportDataForBinding(void) {
	std::map<std::string, std::string> exportData;	// Create the "map" (its a dictionary lets be honest)

	exportData["position"] = std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(position.z);
	exportData["draw_position"] = std::to_string(drawPosition.x) + ", " + std::to_string(drawPosition.y) + ", " + std::to_string(drawPosition.z);
	exportData["grid_position"] = std::to_string(gridPosition.x) + ", " + std::to_string(gridPosition.y);
	if (indexOfShader != -1) {
		exportData["shader"] = Engine_Pointer->shaderRegister[indexOfShader]->GetName();
	}
	else {
		exportData["shader"] = "Not Set";
	}
	exportData["texture"] = texture->GetName();
	exportData["model"] = model.GetName();
	if (animationIndex != -1) {
		exportData["current_animation"] = animations[animationIndex].GetName();
	}
	else {
		exportData["current_animation"] = "Not Set";
	}

	return exportData;
}
float Tile::CalculateSlope(const glm::ivec2& slopeOffset) {
	glm::ivec2 left = glm::ivec2(position) + glm::ivec2(0, slopeOffset.x);
	glm::ivec2 right = glm::ivec2(position.x + Engine_Pointer->tileSize.x, position.y) + glm::ivec2(0, slopeOffset.y);

	// Delta of each side where the slope is
	int deltaX = right.x - left.x;
	int deltaY = right.y - left.y;
	float slopeAngle = (float)(std::atan2(deltaY, deltaX)) * 180.0f / 3.14159f;		// Calculate the slope

	return slopeAngle;
}
void Tile::LoadAnimations(void) {
}
void Tile::AnimationStateHandler(void) {
}
void Tile::AnimationIndexHandler(void) {
}
