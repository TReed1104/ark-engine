#ifndef Mesh_h_included
#define Mesh_h_included

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <glm.hpp>
#include "ShaderComponents.hpp"



class Model
{
public:
	class Mesh
	{
	public:
		// Public Member Variables.
		VertexShaderComponents vertexShaderComponents;
		FragementShaderComponents fragementShaderComponents;
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

		// Constructors and Deconstructors
		Mesh() {}
		~Mesh() {}

		// Public Member Function Declarations
		void InitialiseShaderObjects(VertexShaderComponents vertexShaderComponents, FragementShaderComponents fragShaderComponents);

	private:
		// Private Member Function Declarations
		void InitialiseBuffers(void);
		void InitialiseVertexArray(void);
	};

	// Public Member Variables.
	std::string name;
	std::vector<Mesh> meshes;
	glm::mat4 translationMatrix;
	glm::mat4 rotationMatrix;
	glm::mat4 scaleMatrix;
	glm::mat4 modelMatrix;

	// Constructors and Deconstructors
	Model() {}
	~Model() {}

	// Public Member Function Declarations
	void UpdateViewMatrix();
	void InitialiseMeshShaderObject(VertexShaderComponents vertexShaderComponents, FragementShaderComponents fragShaderComponents);
};
#endif