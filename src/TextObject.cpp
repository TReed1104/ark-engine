#include "TextObject.h"
#include "Engine.h"

Engine* TextObject::Engine_Pointer;

TextObject::TextObject(const std::string& name, const std::string& text, Font* font, const glm::vec3& position, const glm::vec3& colour, const bool& isEnabled, const bool& useCamera) {
	// Informational
	this->name = name;
	this->text = text;
	this->font = font;

	// Transforms
	this->position = position;
	this->rotation = 0.0f;
	this->scale = glm::vec3(1.0f);
	
	// Rendering
	this->colour = colour;
	this->indexOfTextShader = Engine_Pointer->GetIndexOfShader("text renderer");
	this->model = Model(text, false);
	this->model.OverrideLoadState(true);
	this->useCamera = useCamera;
	this->isEnabled = isEnabled;

	LoadText();
}
TextObject::~TextObject() {

}

void TextObject::Update(const float & deltaTime) {
	if (isEnabled) {
		// Apply transformations
		UpdatePosition();
		UpdateRotation();
		UpdateScale();
	}
}
void TextObject::Draw(void) {
	if (isEnabled) {
		// Setup the MVP matrix for the Text.
		glm::vec2 viewPort = (Engine_Pointer->windowGridSize * Engine_Pointer->tileSize);
		glm::mat4* projectionMatrix = &(glm::ortho(0.0f, viewPort.x, viewPort.y, 0.0f, 0.0f, 2.0f));
		glm::mat4* viewMatrix = &(glm::lookAt(glm::vec3(position.x, position.y, 1.0f), glm::vec3(position.x, position.y, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		if (useCamera) {
			viewMatrix = &Engine_Pointer->mainCamera->viewMatrix;
			projectionMatrix = &Engine_Pointer->mainCamera->projectionMatrix;
		}

		glEnable(GL_BLEND);
		const size_t numberOfMeshes = model.meshes.size();
		for (size_t i = 0; i < numberOfMeshes; i++) {
			Engine_Pointer->shaderRegister[indexOfTextShader]->Activate();
			Model::Mesh &currentMesh = model.meshes[i];
			glBindVertexArray(currentMesh.vertexArrayObject);

			const GLuint* shader = Engine_Pointer->shaderRegister[indexOfTextShader]->GetShader();
			glUniformMatrix4fv(glGetUniformLocation(*shader, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(*viewMatrix));
			glUniformMatrix4fv(glGetUniformLocation(*shader, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(*projectionMatrix));
			glUniformMatrix4fv(glGetUniformLocation(*shader, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(currentMesh.GetModelMatrix()));

			bool useTextures = (glyphs[i].texture.textureID != -1 && currentMesh.isSetupForTextures);
			glUniform1i(glGetUniformLocation(*shader, "hasTexture"), useTextures);
			if (useTextures) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, glyphs[i].texture.textureID);
				glUniform1i(glGetUniformLocation(*shader, "textureSampler"), 0);
				glUniform3fv(glGetUniformLocation(*shader, "textColour"), 1, glm::value_ptr(colour));
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
		glDisable(GL_BLEND);
	}
}
void TextObject::UpdateText(const std::string & newText) {
	this->text = newText;
	this->LoadText();
}
void TextObject::UpdateFont(Font* font) {
	this->font = font;
	LoadText();
}
const std::string TextObject::GetName(void) {
	return this->name;
}
void TextObject::IsEnabled(const bool& enableState) {
	this->isEnabled = enableState;
}
void TextObject::LoadText() {
	glyphs.clear();
	model.meshes.clear();
	const size_t lengthOfText = text.size();
	for (size_t i = 0; i < lengthOfText; i++) {
		Glyph& currentGlyph = font->GetGlyph(text[i]);
		glyphs.push_back(currentGlyph);
		model.meshes.push_back(currentGlyph.mesh);
	}

	// Setup the Model for handling its new Meshes.
	model.SetMeshParents();
	model.Translate(position);
	model.Rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	model.Scale(scale);

	// Go through and move the glyphs to their respective positions in the text.
	glm::vec3 cursorPosition = position;	// The cursor for where the Glyph is to be rendered
	for (size_t i = 0; i < lengthOfText; i++) {
		Glyph& currentGlyph = glyphs[i];
		Model::Mesh& currentMesh = model.meshes[i];

		currentMesh.SetTranslation(false, cursorPosition);
		cursorPosition.x += currentGlyph.advance.x;
	}
}
void TextObject::UpdatePosition() {
	model.Translate(position);
}
void TextObject::UpdateRotation() {
	model.Rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f));
}
void TextObject::UpdateScale() {
	model.Scale(scale);
}
