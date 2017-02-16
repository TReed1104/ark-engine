#ifndef ShaderComponents_h_included
#define ShaderComponents_h_included

// Header file Includes.
#include <GL/glew.h>
class ShaderPointers {
public:
	GLint vertexPositionAttrib;
	GLint colourAttrib;
	GLint uvAttrib;
	GLint modelMatrixUniform;
	GLint viewMatrixUniform;
	GLint projectionMatrixUniform;
	GLint hasTextureUniform;
	GLint textureSamplerUniform;

	ShaderPointers() {};
	~ShaderPointers() {};
};
#endif