#include "Model.h"

void Model::Mesh::SetVertexBuffers(const ShaderPointers &shaderPointers) {
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
void Model::Mesh::SetVertexArray(const ShaderPointers &shaderPointers) {
	glGenVertexArrays(1, &vertexArrayObject);			// Create a Vertex Array Object
	std::cout << "Vertex Array OK! GLuint: " << vertexArrayObject << std::endl;
	glBindVertexArray(vertexArrayObject);				// Make the just created VAO the active one.

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);	// Bind positionBufferObject.
	glVertexAttribPointer(shaderPointers.vertexPositionAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);	// Specify that position data contains four floats per vertex, and goes into attribute index positionLocation

	glBindBuffer(GL_ARRAY_BUFFER, colourBufferObject);
	glVertexAttribPointer(shaderPointers.colourAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

	if (isSetupForTextures) {
		glBindBuffer(GL_ARRAY_BUFFER, uvBufferObject);
		glVertexAttribPointer(shaderPointers.uvAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

	glEnableVertexAttribArray(shaderPointers.vertexPositionAttrib);		// Enable attribute at index positionLocation.
	glEnableVertexAttribArray(shaderPointers.colourAttrib);
	if (isSetupForTextures) {
		glEnableVertexAttribArray(shaderPointers.uvAttrib);
	}

	glBindVertexArray(0);	// Unbind the VAO so we can't change it.

							// Clean up.
	glDisableVertexAttribArray(shaderPointers.vertexPositionAttrib);	//disable vertex attribute at index positionLocation.
	glDisableVertexAttribArray(shaderPointers.colourAttrib);
	if (isSetupForTextures) {
		glDisableVertexAttribArray(shaderPointers.uvAttrib);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind array buffer.
}

void Model::SetVertexData(const ShaderPointers &shaderPointers) {
	for (int i = 0; i < meshes.size(); i++) {
		meshes[i].SetVertexBuffers(shaderPointers);
		meshes[i].SetVertexArray(shaderPointers);
	}
}
