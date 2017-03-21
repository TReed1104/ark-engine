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