#include "GameObject.h"
#include "Engine.h"

GameObject::GameObject(const Engine& engine, const Model &model, const Texture& texture, const glm::vec3& position, const glm::vec2& sourceFrameSize) {
	this->engine = &engine;
	this->model = model;
	this->model.SetMeshParents();
	this->position = position;
	this->drawPosition = position;
	rotation = 0.0f;
	scale = glm::vec3(1.0f);
	velocity = glm::vec3(0.0f);
	this->model.Translate(drawPosition);
	this->model.Rotate();
	this->model.Scale();

	// Textures
	this->texture = &texture;
	this->sourceFrameSize = sourceFrameSize;
	this->sourceFramePosition = glm::vec2(0, 0);
}
GameObject::~GameObject() {

}

// Game Run Time.
void GameObject::Update(float deltaTime) {
	// Derived logic will be called here.


	// Set the transforms for the overall model.
	model.Translate(drawPosition);
	model.Rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	model.Scale(scale);
}
void GameObject::Draw() {
	glEnable(GL_BLEND);
	// Loop through each mesh of the model
	for (int i = 0; i < model.meshes.size(); i++) {
		Model::Mesh &currentMesh = model.meshes[i];	// Ref to the current mesh for easier access.

		// Bind the VAO to be used in this draw.
		glBindVertexArray(currentMesh.vertexArrayObject);

		// Passes the Model Matrix of the Object to the shader.
		glUniformMatrix4fv(engine->shaderPointers.modelMatrixUniform, 1, GL_FALSE, glm::value_ptr(currentMesh.GetModelMatrix()));

		bool useTextures = (texture->id != -1 && currentMesh.isSetupForTextures);
		if (useTextures) {
			// Textures are setup correctly, tell the shader to use the texture and setup the source frame.
			glUniform1i(engine->shaderPointers.hasTextureUniform, useTextures);
			glUniform2fv(engine->shaderPointers.textureDimensionsUniform, 1, glm::value_ptr(texture->dimensions));
			glUniform2fv(engine->shaderPointers.sourceFrameSizeUniform, 1, glm::value_ptr(sourceFrameSize));
			glUniform2fv(engine->shaderPointers.sourceFramePositionUniform, 1, glm::value_ptr(sourceFramePosition));

			// Activate the correct texture.
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture->id);
			glUniform1i(engine->shaderPointers.textureSamplerUniform, 0);
		}
		else {
			// Textures are not setup, use the colour buffer.
			glUniform1i(engine->shaderPointers.hasTextureUniform, false);
		}

		// Tell the shader how to draw between each point.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currentMesh.indicesBufferObject);

		// Executes the draw call. Draws triangles, drawing between the points in the order specified. Using the indicies buffer currently bound.
		glDrawElements(GL_TRIANGLES, currentMesh.indices.size(), GL_UNSIGNED_INT, (void*)0);

		// If the mesh was setup for texturing, this unbinds the textures used, clearing up ready for next time.
		if (useTextures) {
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		// Unbinds the indices buffer.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// Unbind the VAO, ready for the next object.
		glBindVertexArray(0);
	}
	glDisable(GL_BLEND);
}

// Transformation
void GameObject::Translate(const int& indexOfMesh, const glm::vec3& translation) {
	model.SetMeshTranslation(indexOfMesh, translation);
}
void GameObject::Rotate(const int& indexOfMesh, const float& rotationAngle, const glm::vec3& rotationAxis) {
	model.SetMeshRotation(indexOfMesh, rotationAngle, rotationAxis);
}
void GameObject::Scale(const int& indexOfMesh, const glm::vec3& scale) {
	model.SetMeshScale(indexOfMesh, scale);
}

