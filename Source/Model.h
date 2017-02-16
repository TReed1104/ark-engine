#ifndef Mesh_h_included
#define Mesh_h_included

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <glm.hpp>
#include "ShaderPointers.hpp"

class Model
{
public:
	class Mesh
	{
	public:
		GLuint vertexBufferObject;
		GLuint indicesBufferObject;
		GLuint colourBufferObject;
		GLuint uvBufferObject;
		GLuint vertexArrayObject;
		bool isSetupForTextures = false;
		std::vector<glm::vec3> vertexPositions;
		std::vector<unsigned int> indices;
		std::vector<glm::vec3> colourData;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> surfaceNormals;

		Mesh() {}
		~Mesh() {}

		void SetVertexBuffers(const ShaderPointers &shaderPointers);
		void SetVertexArray(const ShaderPointers &shaderPointers);
	};

	std::string name;
	std::vector<Mesh> meshes;

	Model() {}
	~Model() {}

	void SetVertexData(const ShaderPointers &shaderPointers);
};
#endif