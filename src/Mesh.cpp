#include "Model.h"
#include "Engine.h"

// Mesh definitions
Model::Mesh::Mesh() {
	isSetupForTextures = false;
}
Model::Mesh::~Mesh() {

}
void Model::Mesh::SetParent(const Model & parentModel) {
	this->parent = &parentModel;
}
void Model::Mesh::GenerateVertexBuffers() {
	if (vertexPositions.size() > 0) {
		glGenBuffers(1, &vertexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, vertexPositions.size() * sizeof(glm::vec3), &vertexPositions[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}
	if (indices.size() > 0) {
		glGenBuffers(1, &indicesBufferObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	if (colourData.size() > 0) {
		glGenBuffers(1, &colourBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, colourBufferObject);
		glBufferData(GL_ARRAY_BUFFER, colourData.size() * sizeof(glm::vec3), &colourData[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	if (uvs.size() > 0) {
		glGenBuffers(1, &uvBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, uvBufferObject);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	std::cout << ">> Buffers OK! VBO GLuint: " << vertexBufferObject << ", IBO GLuint: " << indicesBufferObject << ", UBO GLuint: " << uvBufferObject << std::endl;
}
void Model::Mesh::GenerateVertexArrayObject() {
	// Generate and bind a Vertex Array Object.
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Bind the Buffers to the Vertex Array Object.
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, colourBufferObject);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);

	if (isSetupForTextures) {
		glBindBuffer(GL_ARRAY_BUFFER, uvBufferObject);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid*)0);
	}

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	if (isSetupForTextures) {
		glEnableVertexAttribArray(2);
	}

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	if (isSetupForTextures) {
		glDisableVertexAttribArray(2);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind array buffer.
}
void Model::Mesh::SetTranslation(bool isModelUpdate, const glm::vec3& translation) {
	if (!isModelUpdate) {
		translationMatrix = glm::translate(parent->modelTranslationMatrix, translation);
		currentTranslation = translation;
	}
	else {
		translationMatrix = glm::translate(parent->modelTranslationMatrix, currentTranslation);
	}
}
void Model::Mesh::SetRotation(bool isModelUpdate, const float& rotationAngle, const glm::vec3& rotationAxis) {
	if (!isModelUpdate) {
		rotationMatrix = glm::rotate(parent->modelRotationMatrix, glm::radians(rotationAngle), rotationAxis);
		currentRotationAngle = rotationAngle;
		currentRotationAxis = rotationAxis;
	}
	else {
		rotationMatrix = glm::rotate(parent->modelRotationMatrix, glm::radians(currentRotationAngle), currentRotationAxis);
	}
}
void Model::Mesh::SetScale(bool isModelUpdate, const glm::vec3& scale) {
	if (!isModelUpdate) {
		scaleMatrix = glm::scale(parent->modelScaleMatrix, scale);
		currentScale = scale;
	}
	else {
		scaleMatrix = glm::scale(parent->modelScaleMatrix, currentScale);
	}
}
glm::mat4 Model::Mesh::GetModelMatrix() {
	return (translationMatrix * rotationMatrix * scaleMatrix);
}