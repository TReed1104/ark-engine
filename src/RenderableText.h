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

class Engine;

class RenderableText {
public:
	static Engine* Engine_Pointer;

	std::string name;			// id of the text
	std::string text;			// String the class will represent
	glm::vec3 colour;
	std::vector<Glyph> glyphs;	// The glyphs making up the text
	Model model;

	// Indexers
	int indexOfTextShader;

	// Positions
	glm::vec3 position;
	bool useCamera;

	// Transformations
	float rotation;
	glm::vec3 scale;

	RenderableText(const std::string& name, const std::string& text, Font* font, const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& colour = glm::vec3(1.0f), const bool& useCamera = false);
	~RenderableText();

	void Update(const float& deltaTime);
	void Draw(void);

	void Enable(void);
	void Disable(void);

private:
	void LoadText(Font* font);
	void UpdatePosition(void);
	void UpdateRotation(void);
	void UpdateScale(void);
};

#endif