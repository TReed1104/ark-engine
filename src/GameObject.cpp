#include "GameObject.h"
#include "Engine.h"

Engine* GameObject::Engine_Pointer;

GameObject::GameObject(const std::string & scriptPath) {
	// Load the script if given
	if (scriptPath != "NO SCRIPT") {
		this->script = new LuaScript(scriptPath);
	}
	else {
		this->script = nullptr;
	}

	// Texture Setup
	this->texture = nullptr;
	this->sourceFrameSize = glm::vec2(Engine_Pointer->tileSize);
	this->sourceFramePosition = glm::vec2(0.0f);
	// Position Setup
	this->position = glm::vec3(0.0f);
	this->gridPosition = Engine_Pointer->ConvertToGridPosition(glm::vec2(this->position.x, this->position.y));
	this->drawOffset = glm::vec2(0.0f);
	this->drawPosition = this->position + glm::vec3(this->drawOffset, 0);
	this->rotation = 0.0f;
	this->scale = glm::vec3(1.0f);
	
	// Physics Setup
	this->velocity = glm::vec2(0.0f);
	this->velocityForTileSnap = glm::vec2(0.0f);
	this->movementSpeed = 0.0f;
	this->boundingBoxOffset = glm::vec2(0.0f);
	this->boundingBox = BoundingBox(glm::vec2(this->position.x, this->position.y) + boundingBoxOffset);

	model.Translate(drawPosition);
	model.Rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	model.Scale(scale);
}
GameObject::~GameObject(void) {
	if (script != nullptr) {
		delete script;
	}
}

// Game Run Time.
void GameObject::Update(const float& deltaTime) {
	// If the velocity of the object has been changed, update the translation matrix of the object.
	if (velocity != glm::vec2(0.0f, 0.0f) || velocityForTileSnap != glm::vec2(0.0f, 0.0f)) {
		model.Translate(drawPosition);
	}

	velocity = glm::vec2(0, 0);
	velocityForTileSnap = glm::vec2(0.0f, 0.0f);
	this->gridPosition = Engine_Pointer->ConvertToGridPosition(glm::vec2(this->position.x, this->position.y));
}
void GameObject::Draw(void) {
	glEnable(GL_BLEND);
	// Loop through each mesh of the model
	for (int i = 0; i < model.meshes.size(); i++) {
		Model::Mesh &currentMesh = model.meshes[i];	// Ref to the current mesh for easier access.

		// Bind the VAO to be used in this draw.
		glBindVertexArray(currentMesh.vertexArrayObject);

		// Passes the Model Matrix of the Object to the shader.
		glUniformMatrix4fv(Engine_Pointer->shaderPointers.modelMatrixUniform, 1, GL_FALSE, glm::value_ptr(currentMesh.GetModelMatrix()));

		bool useTextures = (texture->id != -1 && currentMesh.isSetupForTextures);
		if (useTextures) {
			// Textures are setup correctly, tell the shader to use the texture and setup the source frame.
			glUniform1i(Engine_Pointer->shaderPointers.hasTextureUniform, useTextures);
			glUniform2fv(Engine_Pointer->shaderPointers.textureDimensionsUniform, 1, glm::value_ptr(texture->dimensions));
			glUniform2fv(Engine_Pointer->shaderPointers.sourceFrameSizeUniform, 1, glm::value_ptr(sourceFrameSize));
			glUniform2fv(Engine_Pointer->shaderPointers.sourceFramePositionUniform, 1, glm::value_ptr(sourceFramePosition));

			// Activate the correct texture.
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture->id);
			glUniform1i(Engine_Pointer->shaderPointers.textureSamplerUniform, 0);
		}
		else {
			// Textures are not setup, use the colour buffer.
			glUniform1i(Engine_Pointer->shaderPointers.hasTextureUniform, false);
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

// Position control functions
void GameObject::UpdatePositions(const glm::vec2& newPosition) {
	position = glm::vec3(newPosition.x, newPosition.y, position.z);
	gridPosition = Engine_Pointer->ConvertToGridPosition(glm::vec2(position.x, position.y));
	drawPosition = (position + glm::vec3(drawOffset, 0.0f));
	boundingBox.UpdatePosition(glm::vec2(position.x, position.y) + boundingBoxOffset);
}
