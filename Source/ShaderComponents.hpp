#ifndef ShaderComponents_h_included
#define ShaderComponents_h_included

// Header file Includes.
#include <GL/glew.h>

class VertexShaderComponents {
public:
	GLint vertexPositionAttrib;
	GLint colourAttrib;
	GLint uvAttrib;

	VertexShaderComponents() {}
	~VertexShaderComponents() {}
};

class FragementShaderComponents
{
public:
	GLint hasTextureUniform;
	GLint textureSamplerUniform;
	GLint modelMatrixUniform;
	GLint viewMatrixUniform;
	GLint projectionMatrixUniform;

	// Constructors and Deconstructors
	FragementShaderComponents() {}
	~FragementShaderComponents() {}
};
#endif