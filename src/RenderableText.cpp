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
	glEnable(GL_BLEND);
	
	const size_t sizeOfGlyphList = glyphs.size();
	for (size_t i = 0; i < sizeOfGlyphList; i++) {
		Glyph &currentGlyph = glyphs[i];
		const size_t numberOfMeshes = currentGlyph.model.meshes.size();
		for (size_t j = 0; j < numberOfMeshes; j++) {
			Model::Mesh &currentMesh = currentGlyph.model.meshes[j];
			glBindVertexArray(currentMesh.vertexArrayObject);

			glUniformMatrix4fv(glGetUniformLocation(Engine_Pointer->shaderRegister[indexOfTextShader]->program, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(Engine_Pointer->mainCamera->viewMatrix));
			glUniformMatrix4fv(glGetUniformLocation(Engine_Pointer->shaderRegister[indexOfTextShader]->program, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(Engine_Pointer->mainCamera->projectionMatrix));
			glUniformMatrix4fv(glGetUniformLocation(Engine_Pointer->shaderRegister[indexOfTextShader]->program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(currentMesh.GetModelMatrix()));

			bool useTextures = (currentGlyph.texture.id != -1 && currentMesh.isSetupForTextures);
			glUniform1i(glGetUniformLocation(Engine_Pointer->shaderRegister[indexOfTextShader]->program, "hasTexture"), useTextures);
			if (useTextures) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, currentGlyph.texture.id);
			}
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currentMesh.indicesBufferObject);
			glDrawElements(GL_TRIANGLES, (GLsizei)currentMesh.indices.size(), GL_UNSIGNED_INT, (void*)0);
			if (useTextures) {
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			glUseProgram(0);
		}
	}

	
	glDisable(GL_BLEND);
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
