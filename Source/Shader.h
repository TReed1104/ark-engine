// Include Guard, helps prevent recursive inclusion.
#ifndef Shader_h_included
#define Shader_h_included

// Header file Includes.
#include <GL/glew.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>

class Shader {
public:
	// Public Member Variables.
	GLuint shaderID;
	std::string strShader;

	Shader(const std::string shaderLocation, GLenum eShaderType);		// Constructor.
	~Shader();															// Deconstructor.

																		// Public Methods.
	void LoadShader(std::string ShaderLocation);
	void CreateShader(GLenum eShaderType, const std::string &strShaderFile);
};
#endif