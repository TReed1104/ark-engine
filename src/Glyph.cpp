#include "Glyph.h"

Glyph::Glyph(const char& character) {
	this->texture = Texture(std::to_string(character));
	this->character = character;
	this->size = glm::vec2(0, 0);
	this->advance = glm::vec2(0, 0);
	this->horizontalBearing = glm::vec2(0, 0);
	this->verticalBearing = glm::vec2(0, 0);
}
Glyph::~Glyph() {

}