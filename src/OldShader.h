#ifndef ARKENGINE_OLDSHADER_H_
#define ARKENGINE_OLDSHADER_H_

#include <GL/glew.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>

class OldShader {
public:
	GLuint shaderID;
	std::string strShader;

	OldShader(const std::string& shaderLocation, const GLenum& eShaderType);
	~OldShader();

	void Load(const std::string& ShaderLocation);
	void Create(const GLenum& eShaderType, const std::string& strShaderFile);
};
#endif