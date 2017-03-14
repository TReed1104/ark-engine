#ifndef ARKENGINE_SHADER_H_
#define ARKENGINE_SHADER_H_

#include <GL/glew.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>

class Shader {
public:
	GLuint shaderID;
	std::string strShader;

	Shader(const std::string& shaderLocation, const GLenum& eShaderType);
	~Shader();

	void Load(const std::string& ShaderLocation);
	void Create(const GLenum& eShaderType, const std::string& strShaderFile);
};
#endif