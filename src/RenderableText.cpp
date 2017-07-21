#include "RenderableText.h"
#include "Engine.h"

Engine* RenderableText::Engine_Pointer;

RenderableText::RenderableText(const std::string& text, Font* font, const glm::vec3& position) {
	this->text = text;
	this->font = font;
	this->indexOfTextShader = 1;
	///TODO: Add code to find the text rendering shader dynamically.

	this->doTranslation = false;
	this->doRotation = false;
	this->doScalar = false;
	this->position = position;
	this->rotation = 0.0f;
	this->scale = glm::vec3(1.0f);
	LoadText();
}
RenderableText::~RenderableText() {

}

void RenderableText::Update(const float & deltaTime) {
	
}
void RenderableText::Draw(void) {

}
void RenderableText::LoadText(void) {
	const size_t lengthOfText = text.size();
	for (size_t i = 0; i < lengthOfText; i++) {
		char& currentChar = text[i];
		glyphs.push_back(font->GetGlyph(currentChar));

	}
}
