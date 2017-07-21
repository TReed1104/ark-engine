#ifndef ARKENGINE_GLYPH_H_
#define ARKENGINE_GLYPH_H_

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Glyph.h"
#include "Texture.hpp"
#include "Model.h"

class Glyph {
public:
	// Glyph metrics
	char character;
	int minX;
	int maxX;
	int minY;
	int maxY;
	int advance;
	int width;
	int height;

	// Rendering
	Model model;
	Texture texture;
	glm::vec4 colour;

	// Due to the way SDL_TTF outputs its glyph metrics, the Glyph class has to use public fields and a blank constructor.
	Glyph(const char& character = NULL);
	~Glyph();

private:

};

#endif