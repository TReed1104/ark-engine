// Include Guard, helps prevent recursive inclusion.
#ifndef ShaderComponents_h_included
#define ShaderComponents_h_included

// Header file Includes.
#include <GL/glew.h>

class ShaderComponents
{
public:
	// Public Member Variables.
	GLint vertexPositionAttrib;
	GLint colourAttrib;
	GLint uvAttrib;
	GLint hasTextureUniform;
	GLint textureSamplerUniform;
	GLint modelMatrixUniform;
	GLint viewMatrixUniform;
	GLint projectionMatrixUniform;

	// Constructors and Deconstructors
	ShaderComponents() {}
	~ShaderComponents() {}
};
#endif