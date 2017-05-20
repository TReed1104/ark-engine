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
	std::string ID;
	GLuint program;

	Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
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

	bool ReadShaderFiles(const std::string & vertexShaderPath, const std::string & fragmentShaderPath);
	bool CompileShader();
};

#endif
