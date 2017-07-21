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
}
RenderableText::~RenderableText() {

}

void RenderableText::Update(const float & deltaTime) {
	UpdatePosition();
	UpdateRotation();
	UpdateScale();
}
void RenderableText::Draw(void) {

}
void RenderableText::Reposition(const glm::vec2 & newPosition) {
	position = glm::vec3(newPosition.x, newPosition.y, position.z);
	const size_t sizeOfGlyphList = glyphs.size();
	for (size_t i = 0; i < sizeOfGlyphList; i++) {
		glyphs[i].model.Translate(position);
	}
}
void RenderableText::UpdatePosition() {
	if (doTranslation) {
		const size_t sizeOfGlyphList = glyphs.size();
		for (size_t i = 0; i < sizeOfGlyphList; i++) {
			glyphs[i].model.Translate(position);
		}
		doTranslation = false;
	}
}
void RenderableText::UpdateRotation() {
	if (doRotation) {
		const size_t sizeOfGlyphList = glyphs.size();
		for (size_t i = 0; i < sizeOfGlyphList; i++) {
			glyphs[i].model.Rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f));
		}
		doRotation = false;
	}
}
void RenderableText::UpdateScale() {
	if (doScalar) {
		const size_t sizeOfGlyphList = glyphs.size();
		for (size_t i = 0; i < sizeOfGlyphList; i++) {
			glyphs[i].model.Scale(scale);
		}
		doScalar = false;
	}
}
