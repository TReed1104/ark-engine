#include "BoundingBox.h"
#include "Engine.h"

Engine* BoundingBox::Engine_Pointer;

BoundingBox::BoundingBox(float x, float y, float width, float height) {
	this->x = roundf(x);
	this->y = roundf(y);
	this->width = width;
	this->height = height;
}
BoundingBox::BoundingBox(glm::vec2 position) {
	this->x = roundf(position.x);
	this->y = roundf(position.y);
	this->width = Engine_Pointer->tileSize.x;
	this->height = Engine_Pointer->tileSize.y;
}
BoundingBox::BoundingBox(glm::vec2 position, glm::vec2 dimensions) {
	this->x = roundf(position.x);
	this->y = roundf(position.y);
	this->width = dimensions.x;
	this->height = dimensions.y;
}
BoundingBox::~BoundingBox(void) {

}

bool BoundingBox::Intersect(BoundingBox otherBB) {
	if (!Engine_Pointer->levelRegister[Engine_Pointer->indexCurrentLevel]->IsTileSolid(otherBB.GetGridPosition())) {
		return false;
	}
	else {
		return ((x < otherBB.x + otherBB.width) && (x + width > otherBB.x) && (y < otherBB.y + otherBB.height) && (y + height > otherBB.y));
	}
}
void BoundingBox::UpdatePosition(glm::vec2 newPosition) {
	this->x = roundf(newPosition.x);
	this->y = roundf(newPosition.y);
}
glm::vec2 BoundingBox::GetPosition(void) {
	return glm::vec2(x, y);
}
glm::vec2 BoundingBox::GetGridPosition(void) {
	return Engine_Pointer->ConvertToGridPosition(glm::vec2(x, y));
}
glm::vec2 BoundingBox::GetDimensions(void) {
	return glm::vec2(width, height);
}
glm::vec2 BoundingBox::TopLeftPosition(void) {
	return glm::vec2(x, y);
}
glm::vec2 BoundingBox::TopRightPosition(void) {
	return (glm::vec2(x, y) + glm::vec2(width - 1, 0));
}
glm::vec2 BoundingBox::BottomLeftPosition(void) {
	return (glm::vec2(x, y) + glm::vec2(0, height - 1));
}
glm::vec2 BoundingBox::BottomRightPosition(void) {
	return (glm::vec2(x, y) + glm::vec2(width - 1, height - 1));
}
glm::vec2 BoundingBox::TopLeftGridPosition(void) {
	return Engine_Pointer->ConvertToGridPosition(glm::vec2(x, y));
}
glm::vec2 BoundingBox::TopRightGridPosition(void) {
	return Engine_Pointer->ConvertToGridPosition((glm::vec2(x, y) + glm::vec2(width - 1, 0)));
}
glm::vec2 BoundingBox::BottomLeftGridPosition(void) {
	return Engine_Pointer->ConvertToGridPosition((glm::vec2(x, y) + glm::vec2(0, height - 1)));
}
glm::vec2 BoundingBox::BottomRightGridPosition(void) {
	return Engine_Pointer->ConvertToGridPosition((glm::vec2(x, y) + glm::vec2(width - 1, height - 1)));
}
