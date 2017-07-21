#ifndef ARKENGINE_RENDERABLETEXT_H_
#define ARKENGINE_RENDERABLETEXT_H_

#include <string>
#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Font.h"
#include "Glyph.h"

class Engine;

class RenderableText {
public:
	static Engine* Engine_Pointer;

	std::string text;
	std::vector<Glyph> glyphs;

	// Positions
	glm::vec3 position;			// The world position of the object
	glm::vec3 drawPosition;		// Position to draw to the screen, this takes into account the offset of the object to its texture

	// Transformations
	float rotation;
	glm::vec3 scale;
	bool doTranslation;		// Flag controlling if the Text needs translating
	bool doRotation;		// Flag controlling if the Text needs rotating
	bool doScalar;			// Flag controlling if the Text needs scaling

	RenderableText(const std::string& text = "", const Font* font = nullptr);
	~RenderableText();

	void Update(const float& deltaTime);
	void Draw(void);
	void Reposition(const glm::vec2& newPosition);

private:

};

#endif
