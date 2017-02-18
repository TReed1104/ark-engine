#ifndef ARKENGINE_MODEL_H_
#define ARKENGINE_MODEL_H_

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <glm.hpp>
#include "ShaderPointers.hpp"

class Engine;

class Model
{
public:
	class Mesh
	{
	public:
		// Buffers where the data about the mesh are stored for use by OpenGL
		GLuint vertexBufferObject;
		GLuint indicesBufferObject;
		GLuint colourBufferObject;
		GLuint uvBufferObject;
		GLuint vertexArrayObject;

		// The attributes of the mesh itself.
		bool isSetupForTextures;
		std::vector<glm::vec3> vertexPositions;
		std::vector<unsigned int> indices;
		std::vector<glm::vec3> colourData;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> surfaceNormals;

		Mesh();
		~Mesh();

		void GenerateVertexBuffers();
		void GenerateVertexArrayObject(const ShaderPointers& shaderPointers);
	private:

	};

	std::string name;
	std::vector<Mesh> meshes;

	Model() {}
	Model(const Engine &engine, const std::string& name = "");
	~Model();

	void SetVertexObjects();

private:
	const Engine* engine;

};
#endif