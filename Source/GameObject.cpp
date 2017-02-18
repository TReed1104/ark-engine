#include "GameObject.h"
#include "Engine.h"

GameObject::GameObject(const Engine &engine, const Model &model, const glm::vec3& position, const char* texturePath, const glm::vec2& sourceFrameSize) {
	this->engine = &engine;
	this->model = model;
	this->position = position;
	this->drawPosition = position;
	rotation = 0.0f;
	scale = glm::vec3(1.0f);
	velocity = glm::vec3(0.0f);
	this->translationMatrix = glm::translate(this->translationMatrix, position);
	this->rotationMatrix = glm::mat4(1.0f);
	this->scaleMatrix = glm::mat4(1.0f);

	// Textures
	this->currentTextureIndex = 0;
	this->sourceFrameSize = sourceFrameSize;
	this->sourceFramePosition = glm::vec2(0, 0);
	this->textureDimensions = glm::vec2(0, 0);
	LoadTexture(texturePath);
}
GameObject::~GameObject() {

}

// Game Run Time.
void GameObject::Update(float deltaTime) {
	// Derived logic will be called here.


	// Set the transforms for the object.
	Translate(drawPosition);
	Rotate(rotation, glm::vec3(0.0f, 1.0f, 0.0f));
	Scale(scale);
}
void GameObject::Draw() {
	glEnable(GL_BLEND);
	// Loop through each mesh of the model
	for (int i = 0; i < model.meshes.size(); i++) {
		Model::Mesh &currentMesh = model.meshes[i];	// Ref to the current mesh for easier access.

		// Bind the VAO to be used in this draw.
		glBindVertexArray(currentMesh.vertexArrayObject);

		// Passes the Model Matrix of the Object to the shader.
		glUniformMatrix4fv(engine->shaderPointers.modelMatrixUniform, 1, GL_FALSE, glm::value_ptr(GetModelMatrix()));

		bool useTextures = ((textureRegister.size() > 0) && currentMesh.isSetupForTextures);
		if (useTextures) {
			// Textures are setup correctly, tell the shader to use the texture and setup the source frame.
			glUniform1i(engine->shaderPointers.hasTextureUniform, useTextures);
			glUniform2fv(engine->shaderPointers.textureDimensionsUniform, 1, glm::value_ptr(textureDimensions));
			glUniform2fv(engine->shaderPointers.sourceFrameSizeUniform, 1, glm::value_ptr(sourceFrameSize));
			glUniform2fv(engine->shaderPointers.sourceFramePositionUniform, 1, glm::value_ptr(sourceFramePosition));

			// Activate the correct texture.
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureRegister[currentTextureIndex].textureID);
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
void GameObject::Translate(const glm::vec3& translation) {
	translationMatrix = glm::translate(glm::mat4(1.0f), translation);
}
void GameObject::Rotate(const float& rotationAngle, const glm::vec3& rotationAxis) {
	rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle), rotationAxis);
}
void GameObject::Scale(const glm::vec3& scale) {
	scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
}
glm::mat4 GameObject::GetModelMatrix() {
	return (translationMatrix * rotationMatrix * scaleMatrix);
}

// Texturing
void GameObject::LoadTexture(const char* texturePath) {
	// This function loads a texture into memory to be used with a source rectangle to depict what part of it to render.
	if (texturePath != "") {
		//int init = IMG_Init(IMG_INIT_PNG);
		SDL_Surface* image = IMG_Load(texturePath);	// Try and load the texture.
		if (image == NULL) {
			// If the texture was not loaded correctly, quit the program and show a error message on the console.
			std::cout << "The loading of Spritesheet: " << texturePath << " failed." << std::endl;
			SDL_Quit();
			exit(1);
		}
		else {
			std::cout << "The loading of Spritesheet: " << texturePath << " was successful." << std::endl;
		}

		Texture tempTexture = Texture(texturePath);
		glGenTextures(1, &tempTexture.textureID);				// Generate a texture ID and store it
		glBindTexture(GL_TEXTURE_2D, tempTexture.textureID);

		// Set the texturing variables for this texture.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

		textureDimensions = glm::vec2(image->w, image->h);
		// Setup the texture.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		textureRegister.push_back(tempTexture);
		SDL_FreeSurface(image);
	}
}

