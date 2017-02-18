#ifndef ARKENGINE_SHADERPOINTERS_HPP_
#define ARKENGINE_SHADERPOINTERS_HPP_

// Header file Includes.
#include <GL/glew.h>
class ShaderPointers {
public:
	// Vertex Shader Pointers
	GLint vertexPositionAttrib;
	GLint colourAttrib;
	GLint uvAttrib;
	GLint modelMatrixUniform;
	GLint viewMatrixUniform;
	GLint projectionMatrixUniform;
	// Fragment Shader Pointers
	GLint hasTextureUniform;
	GLint textureSamplerUniform;
	GLint textureDimensionsUniform;
	GLint sourceFrameSizeUniform;
	GLint sourceFramePositionUniform;

	ShaderPointers() {};
	~ShaderPointers() {};
};
#endif