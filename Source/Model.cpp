#include "Model.h"
#include "Engine.h"

Model::Mesh::Mesh() {
	isSetupForTextures = false;
}
Model::Mesh::~Mesh() {

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
	}

	if (uvs.size() > 0) {
		glGenBuffers(1, &uvBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, uvBufferObject);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	std::cout << "Buffers OK! VBO GLuint: " << vertexBufferObject << ", IBO GLuint: " << indicesBufferObject << ", UBO GLuint: " << uvBufferObject << std::endl;
}
void Model::Mesh::GenerateVertexArrayObject(const ShaderPointers &shaderPointers) {
	// Generate and bind a Vertex Array Object.
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Bind the Buffers to the Vertex Array Object.
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glVertexAttribPointer(shaderPointers.vertexPositionAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, colourBufferObject);
	glVertexAttribPointer(shaderPointers.colourAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

	if (isSetupForTextures) {
		glBindBuffer(GL_ARRAY_BUFFER, uvBufferObject);
		glVertexAttribPointer(shaderPointers.uvAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

	glEnableVertexAttribArray(shaderPointers.vertexPositionAttrib);
	glEnableVertexAttribArray(shaderPointers.colourAttrib);
	if (isSetupForTextures) { 
		glEnableVertexAttribArray(shaderPointers.uvAttrib); 
	}

	glBindVertexArray(0);
	glDisableVertexAttribArray(shaderPointers.vertexPositionAttrib);
	glDisableVertexAttribArray(shaderPointers.colourAttrib);
	if (isSetupForTextures) { 
		(shaderPointers.uvAttrib); 
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind array buffer.
}

Model::Model(const Engine &engine, const std::string& name) {
	this->engine = &engine;
	this->name = name;
}
Model::~Model() {

}

void Model::SetVertexObjects() {
	for (int i = 0; i < meshes.size(); i++) {
		meshes[i].GenerateVertexBuffers();
		meshes[i].GenerateVertexArrayObject(engine->shaderPointers);
	}
}
