#include "GameObject.h"
#include "Engine.h"

GameObject::GameObject(Engine &engine, Model model, glm::vec3 position, char * texturePath) {
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
	this->currentTextureIndex = 0;
	LoadTextures(texturePath);
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
	for (int i = 0; i < model.meshes.size(); i++)
	{
		Model::Mesh &currentMesh = model.meshes[i];
		// OpenGL Rendering fun.
		glBindVertexArray(currentMesh.vertexArrayObject);
		glUniformMatrix4fv(currentMesh.shaderLocations.modelMatrixUniform, 1, GL_FALSE, glm::value_ptr(model.modelMatrix));
		glUniform1i(currentMesh.shaderLocations.hasTextureUniform, currentMesh.hasTextures);
		// Does the Model support texturing?
		if (currentMesh.hasTextures && textureRegister.size() > 0)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureRegister[currentTextureIndex].textureID);
			glUniform1i(currentMesh.shaderLocations.textureSamplerUniform, 0);
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currentMesh.indicesBufferObject);
		glDrawElements(GL_TRIANGLES, currentMesh.indices.size(), GL_UNSIGNED_INT, (void*)0);
		glBindVertexArray(0);
		// Does the Model support texturing?
		if (currentMesh.hasTextures && textureRegister.size() > 0)
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
void GameObject::LoadTextures(char * texturePath) {
	if (texturePath != "")
	{
		SDL_Surface* image = IMG_Load(texturePath);
		Texture tempTexture = Texture(texturePath);
		if (image == NULL)
		{
			std::cout << "Image loading (for texture) failed." << std::endl;
			SDL_Quit();
			exit(1);
		}

		glGenTextures(1, &tempTexture.textureID); //generate a texture ID and store it
		glBindTexture(GL_TEXTURE_2D, tempTexture.textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

		glBindTexture(GL_TEXTURE_2D, 0);
		SDL_FreeSurface(image);
		textureRegister.push_back(tempTexture);
		std::cout << "Texture loaded: " << texturePath << std::endl;
	}
}
