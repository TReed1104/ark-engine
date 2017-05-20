#ifndef ARKENGINE_SHADER_H_
#define ARKENGINE_SHADER_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <GL/glew.h>

class Shader
{
public:
	std::string name;
	GLuint program;

	Shader(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	~Shader();

	void Activate(void);
	bool Load(void);

private:
	// Path of Shader files
	std::string vertexFilePath;
	std::string fragmentFilePath;
	// Raw Shader text
	std::string rawVertexString;
	std::string rawFragmentString;

	bool ReadFile(std::string& rawString, const std::string& strShaderFile);
	bool ImportShaderFiles();
	bool CreateShader(GLuint& shaderID, const GLenum& eShaderType, const std::string& strShaderFile);
	bool CompileShader();
};

#endif
