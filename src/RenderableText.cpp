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
	// Apply transformations
	UpdatePosition();
	UpdateRotation();
	UpdateScale();
}
void RenderableText::Draw(void) {

}
void RenderableText::LoadText(void) {
	const size_t lengthOfText = text.size();
	for (size_t i = 0; i < lengthOfText; i++) {
		char& currentChar = text[i];
		Glyph& currentGlyph = font->GetGlyph(currentChar);

		glyphs.push_back(currentGlyph);
		model.meshes.push_back(currentGlyph.mesh);
	}

	// Setup the Model for handling its new Meshes.
	model.SetMeshParents();
	model.Translate(position);
	model.Rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	model.Scale(scale);
}
void RenderableText::UpdatePosition() {
	if (doTranslation) {
		model.Translate(position);
		doTranslation = false;
	}
}
void RenderableText::UpdateRotation() {
	if (doRotation) {
		model.Rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f));
		doRotation = false;
	}
}
void RenderableText::UpdateScale() {
	if (doScalar) {
		model.Scale(scale);
		doScalar = false;
	}
}
