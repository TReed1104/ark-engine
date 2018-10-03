#include "Shader.h"

Shader::Shader(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
	this->name = name;
	this->vertexFilePath = vertexShaderPath;
	this->fragmentFilePath = fragmentShaderPath;
}
Shader::~Shader() {

}

bool Shader::ReadFile(std::string& rawString, const std::string& strShaderFile) {
	// Read the Shader file
	rawString = "";
	std::ifstream fileReader(strShaderFile);
	if (!fileReader.is_open()) {
		std::cout << ">>>> ERROR!!!! - Shader File not found - " << strShaderFile << std::endl;
		return false;
	}
	std::string line = "";
	while (getline(fileReader, line)) {
		rawString += line + "\n";
	}
	fileReader.close();

	std::cout << ">>>> Shader File Loaded! - " << strShaderFile << std::endl;
	return true;
}
bool Shader::ImportShaderFiles() {
	if (ReadFile(rawVertexString, vertexFilePath)) {
		if (ReadFile(rawFragmentString, fragmentFilePath)) {
			// As both the shader files were parsed correctly into memory, return true.
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
bool Shader::CreateShader(GLuint& shaderID, const GLenum& eShaderType, const std::string& strShaderFile) {
	shaderID = glCreateShader(eShaderType);
	// Error check
	const char *strFileData = strShaderFile.c_str();
	glShaderSource(shaderID, 1, &strFileData, NULL);
	glCompileShader(shaderID);

	GLint status;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
	if (!status) {
		GLint infoLogLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shaderID, infoLogLength, NULL, strInfoLog);

		const char *strShaderType = NULL;
		switch (eShaderType) {
		case GL_VERTEX_SHADER:
			strShaderType = "vertex";
			break;
		case GL_FRAGMENT_SHADER:
			strShaderType = "fragment";
			break;
		}
		fprintf(stderr, ">>>> ERROR!!!! - Compile Failure in %s shader:\n%s\n", strShaderType, strInfoLog);
		delete[] strInfoLog;
		return false;
	}
	return true;
}
bool Shader::CompileShader() {
	GLuint vertexID;
	GLuint fragmentD;

	// Compile and create our shader
	if (CreateShader(vertexID, GL_VERTEX_SHADER, rawVertexString)) {
		//std::cout << ">>>> Vertex Compiled - " << name << std::endl;
		if (CreateShader(fragmentD, GL_FRAGMENT_SHADER, rawFragmentString)) {
			//std::cout << ">>>> Fragment Compiled - " << name << std::endl;

			this->program = glCreateProgram();
			glAttachShader(this->program, vertexID);
			glAttachShader(this->program, fragmentD);
			glLinkProgram(this->program);

			GLint status;
			glGetProgramiv(this->program, GL_LINK_STATUS, &status);

			if (!status) {
				// If the program or shaders fail, print the errors.
				GLint infoLogLength;
				glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
				GLchar *strInfoLog = new GLchar[infoLogLength + 1];
				glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
				fprintf(stderr, ">>>> ERROR!!!! - Linker failure: %s\n", strInfoLog);
				delete[] strInfoLog;
				glDeleteShader(vertexID);
				glDeleteShader(fragmentD);
				return false;
			}
			glDeleteShader(vertexID);
			glDeleteShader(fragmentD);
			std::cout << ">>>> Shader Compiled! - " << name << std::endl;
			return true;
		}
		else {
			std::cout << ">>>> ERROR!!!! - Fragment Shader Failure - " << name << std::endl;
			return false;
		}
	}
	else {
		std::cout << ">>>> ERROR!!!! - Fragment Vertex Failure - " << name << std::endl;
		return false;
	}
}
bool Shader::Load(void) {
	// Import the shader files
	if (ImportShaderFiles()) {
		// Compile the Shaders and attach them to the program
		if (CompileShader()) {
			// The Shader has been successfully created, return true
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
void Shader::Activate(void) {
	// Tells the engine to use this shader for the Current Draw.
	glUseProgram(program);
}
