#include "Shader.h"

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
	vertexFilePath = vertexShaderPath;
	fragmentFilePath = fragmentShaderPath;
}

Shader::~Shader() {

}

void Shader::Activate(void) {
	// Tells the engine to use this shader for the Current Draw.
	glUseProgram(program);
}

bool Shader::Load(void)
{
	if (ReadShaderFiles(vertexFilePath, fragmentFilePath)) {
		if (CompileShader()) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

bool Shader::ReadShaderFiles(const std::string & vertexShaderPath, const std::string & fragmentShaderPath)
{
	rawVertexString = "";
	rawFragmentString = "";

	// Read the Vertex shader file
	std::ifstream vertexShaderReader(vertexShaderPath);
	if (!vertexShaderReader.is_open()) {
		std::cout << "Vertex Shader file not found - " << vertexShaderPath << std::endl;
		return false;
	}
	std::string line = "";
	while (getline(vertexShaderReader, line)) {
		rawVertexString += line + "\n";
	}
	vertexShaderReader.close();

	// Read the Fragment shader file
	std::ifstream fragmentShaderReader(fragmentShaderPath);
	if (!fragmentShaderReader.is_open()) {
		std::cout << "Fragment Shader file not found - " << fragmentShaderPath << std::endl;
		return false;
	}
	line = "";
	while (getline(fragmentShaderReader, line)) {
		rawFragmentString += line + "\n";
	}
	fragmentShaderReader.close();

	// As both the shader files were parsed correctly into memory, return true.
	return true;
}

bool Shader::CompileShader() {
	return false;
}
