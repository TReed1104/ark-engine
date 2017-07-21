#include "Glyph.h"

Glyph::Glyph(const char& character) {
	this->texture = Texture(std::to_string(character), false);
	this->character = character;
	this->minX = -1;
	this->maxX = -1;
	this->minY = -1;
	this->maxY = -1;
	this->advance = -1;
	this->width = -1;
	this->height = -1;
}
Glyph::~Glyph() {

}
