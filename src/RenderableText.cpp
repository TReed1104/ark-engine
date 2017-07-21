#include "RenderableText.h"

Engine* RenderableText::Engine_Pointer;


RenderableText::RenderableText(const std::string& text, const Font* font) {

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
