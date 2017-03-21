#include "BoundingBox.h"
#include "Engine.h"

Engine* BoundingBox::Engine_Pointer;

BoundingBox::BoundingBox(float x, float y, float width, float height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

BoundingBox::BoundingBox(glm::vec2 position, glm::vec2 dimensions) {
	this->x = position.x;
	this->y = position.y;
	this->width = dimensions.x;
	this->height = dimensions.y;
}

BoundingBox::~BoundingBox() {

}

bool BoundingBox::Intersect(BoundingBox otherBB) {
	bool isColliding = ((x < otherBB.x + otherBB.width) && (x + otherBB.width > otherBB.x) && (y < otherBB.y + otherBB.height) && (y + height > otherBB.y));
	return isColliding;
}