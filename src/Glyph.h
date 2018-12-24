#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Texture.h"
#include "Model.h"

class Glyph {
public:
	// Glyph metrics
	char character;
	glm::vec2 size;
	glm::vec2 advance;
	glm::vec2 horizontalBearing;
	glm::vec2 verticalBearing;

	// Rendering
	Model::Mesh mesh;
	Texture texture;
	glm::vec4 colour;

	// Due to the way SDL_TTF outputs its glyph metrics, the Glyph class has to use public fields and a blank constructor.
	Glyph(const char& character = NULL);
	~Glyph();

private:

};