#ifndef ARKENGINE_SHADER_H_
#define ARKENGINE_SHADER_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <GL/glew.h>

class Engine;

class Shader
{
public:
	static Engine* Engine_Pointer;

	Shader(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	~Shader();

	void Activate(void);
	const std::string GetName(void);
	const GLuint* GetShader(void);
	const bool IsLoaded(void);

private:
	std::string name;
	GLuint program;
	bool isLoaded;

	GLuint CreateShaderObject(const std::string& strShaderFile, const GLenum& typeOfShader);
	bool CreateShaderProgram(const std::string& vertexSourcePath, const std::string& fragmentSourcePath);
	std::string ReadSourceFile(const std::string& filePath);
	bool Load(const std::string& vertexSourcePath = "", const std::string& fragmentSourcePath = "");
};

#endif
