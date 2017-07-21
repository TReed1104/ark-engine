#ifndef ARKENGINE_RENDERABLETEXT_H_
#define ARKENGINE_RENDERABLETEXT_H_

#include <string>
#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Model.h"
#include "Font.h"
#include "Glyph.h"

class Engine;

class RenderableText {
public:
	static Engine* Engine_Pointer;

	std::string text;			// String the class will represent
	Font* font;
	std::vector<Glyph> glyphs;	// The glyphs making up the text.
	Model model;

	// Indexers
	int indexOfTextShader;

	// Positions
	glm::vec3 position;

	// Transformations and flags.
	float rotation;
	glm::vec3 scale;
	bool doTranslation;
	bool doRotation;
	bool doScalar;

	RenderableText(const std::string& text, Font* font, const glm::vec3& position = glm::vec3(0.0f));
	~RenderableText();

	void Update(const float& deltaTime);
	void Draw(void);

private:
	void LoadText(void);
};

#endif
