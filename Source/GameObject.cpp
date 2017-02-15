#include "GameObject.h"
#include "Engine.h"

GameObject::GameObject(Engine &engine, Model model, glm::vec3 position, char * texturePath, glm::vec2 sourceFrameSize) {
	// Initialise the GameObject.
	this->engine = &engine;
	this->model = model;
	this->position = position;
	this->drawPosition = position;
	rotation = 0.0f;
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	velocity = glm::vec3(0.0f);
	this->model.translationMatrix = glm::translate(this->model.translationMatrix, position);
	this->model.rotationMatrix = glm::mat4(1.0f);
	this->model.scaleMatrix = glm::mat4(1.0f);
	this->model.UpdateViewMatrix();
	this->sourceFrameSize = sourceFrameSize;
	this->currentTextureIndex = 0;

	LoadTexture(texturePath);
}
GameObject::~GameObject() {

}

void GameObject::Update(float deltaTime) {
	Translate(drawPosition);
	Rotate(rotation, glm::vec3(0.0f, 1.0f, 0.0f));
	Scale(scale);
	this->model.UpdateViewMatrix();
}
void GameObject::Draw(void) {
	// Loop through each mesh of the model
	for (int i = 0; i < model.meshes.size(); i++) {
		Model::Mesh &currentMesh = model.meshes[i];

		// Bind the VAO for the mesh, this tells the OpenGL program what data to use for the draw call.
		glBindVertexArray(currentMesh.vertexArrayObject);

		// Passes the model matrix to the Vertex Shader.
		glUniformMatrix4fv(currentMesh.vertexShaderComponents.modelMatrixUniform, 1, GL_FALSE, glm::value_ptr(model.modelMatrix));

		bool useTextures = ((textureRegister.size() > 0) && currentMesh.isSetupForTextures);		// Decides whether the current mesh has been correctly setup for texturing.
		if (useTextures) {

			glUniform1i(currentMesh.fragementShaderComponents.hasTextureUniform, useTextures);		// Tells the Fragment shader that it is to try and use textures and not the colour buffer data.

			// Texturing setup for the current draw call.
			glActiveTexture(GL_TEXTURE0);															// Select the active texture.
			glBindTexture(GL_TEXTURE_2D, textureRegister[currentTextureIndex].textureID);			// Bind the current texture from the register as the active texture chosen above.
			glUniform1i(currentMesh.fragementShaderComponents.textureSamplerUniform, 0);			// Passes active texture 0 to the Fragement shader.
		}
		else {
			glUniform1i(currentMesh.fragementShaderComponents.hasTextureUniform, false);			// Tells the Fragment shader that it is to try and use the colour buffer data and not textures.
		}

		// Bind the indices buffer as the one to be used in the draw call, this is how OpenGL decides how to draw each triangle.
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
}
void GameObject::Translate(glm::vec3 translation) {
	this->model.translationMatrix = glm::translate(glm::mat4(1.0f), translation);
}
void GameObject::Rotate(float rotationAngle, glm::vec3 rotationAxis) {
	this->model.rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle), rotationAxis);
}
void GameObject::Scale(glm::vec3 scale) {
	this->model.scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
}
void GameObject::LoadTexture(char * texturePath) {
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

		std::cout << "Spritesheet Split has begun, this may take a while..." << std::endl;
		Texture tempTexture = Texture(texturePath);
		glGenTextures(1, &tempTexture.textureID);				// Generate a texture ID and store it
		glBindTexture(GL_TEXTURE_2D, tempTexture.textureID);

		// Set the texturing variables for this texture.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		// Setup the texture.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		textureRegister.push_back(tempTexture);
		SDL_FreeSurface(image);
	}
}
void GameObject::LoadAndSplitTexture(char * texturePath) {
	// This function loads a texture into memory and divides it into seperate textures of a specified size, this takes more computation but does not require a source rectangle.
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
		std::cout << "Spritesheet Split has begun, this may take a while..." << std::endl;
		// Loop splitting the SpriteSheet into seperate textures for use.
		for (int y = 0; y < image->h / sourceFrameSize.y; y++) {
			for (int x = 0; x < image->w / sourceFrameSize.x; x++) {
				Texture tempTexture = Texture(texturePath);

				// Grab the part of the texture to be used currently.
				glPixelStorei(GL_UNPACK_ROW_LENGTH, image->w);
				glPixelStorei(GL_UNPACK_SKIP_PIXELS, x * sourceFrameSize.x);
				glPixelStorei(GL_UNPACK_SKIP_ROWS, y * sourceFrameSize.y);

				glGenTextures(1, &tempTexture.textureID);	// Generate a texture ID and store it
				glBindTexture(GL_TEXTURE_2D, tempTexture.textureID);
				// Set the texturing variables for this texture.
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

				// Setup the texture.
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sourceFrameSize.x, sourceFrameSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
				glGenerateMipmap(GL_TEXTURE_2D);

				glBindTexture(GL_TEXTURE_2D, 0);

				textureRegister.push_back(tempTexture);
			}
		}
		std::cout << "Spritesheet Split finished, The Spritesheet was Split into " << textureRegister.size() << " Sprites.\n" << std::endl;
		SDL_FreeSurface(image);
	}
}

