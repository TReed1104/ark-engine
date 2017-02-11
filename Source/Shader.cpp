#include "Shader.h"

Shader::Shader(const std::string shaderLocation, GLenum eShaderType) {
	LoadShader(shaderLocation);
	CreateShader(eShaderType, strShader);
}
Shader::~Shader() {

}

void Shader::LoadShader(std::string ShaderLocation) {
	bool complete = false;
	std::string line = "";
	std::string text = "";
	std::ifstream shaderReader(ShaderLocation);

	if (!shaderReader.is_open()) {
		throw std::runtime_error("Shader file not found" + ShaderLocation);
	}

	while (getline(shaderReader, line)) {
		text += line + "\n";
	}
	shaderReader.close();
	strShader = text;
	complete = true;
}
void Shader::CreateShader(GLenum eShaderType, const std::string &strShaderFile){
	shaderID = glCreateShader(eShaderType);
	// Error check
	const char *strFileData = strShaderFile.c_str();
	glShaderSource(shaderID, 1, &strFileData, NULL);

	glCompileShader(shaderID);

	GLint status;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		GLint infoLogLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shaderID, infoLogLength, NULL, strInfoLog);

		const char *strShaderType = NULL;
		switch (eShaderType) {
		case GL_VERTEX_SHADER:
			strShaderType = "vertex";
			break;
		case GL_GEOMETRY_SHADER:
			strShaderType = "geometry";
			break;
		case GL_FRAGMENT_SHADER:
			strShaderType = "fragment";
			break;
		}

		fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
		delete[] strInfoLog;
	}
}
