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
	// Constructors
	Shader(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	~Shader();

	// Public Functions
	void Activate(void);
	const std::string Name(void);
	const GLuint* GetShader(void);
	const bool IsLoaded(void);

private:
	// Private Variables
	std::string name;
	GLuint program;
	bool isLoaded;
	std::string vertexFilePath;			// Path of Vertex Shader file
	std::string fragmentFilePath;		// Path of Fragment Shader file
	std::string rawVertexString;
	std::string rawFragmentString;

	// Private Functions
	bool Load(void);
	bool ReadFile(std::string& rawString, const std::string& strShaderFile);
	bool ImportShaderFiles();
	bool CreateShader(GLuint& shaderID, const GLenum& eShaderType, const std::string& strShaderFile);
	bool CompileShader();
};

#endif
