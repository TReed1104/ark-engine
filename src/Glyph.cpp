#include "Glyph.h"

Glyph::Glyph(const char& character) {
	this->texture = Texture(std::to_string(character), false);
	this->character = character;
	this->size = glm::ivec2(0, 0);
	this->advance = glm::ivec2(0, 0);
	this->horizontalBearing = glm::ivec2(0, 0);
	this->verticalBearing = glm::ivec2(0, 0);
}
Glyph::~Glyph() {

}