#include "Shader.h"
#include "Engine.h"

Engine* Shader::Engine_Pointer;

Shader::Shader(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
	this->name = name;
	isLoaded = Load(vertexShaderPath, fragmentShaderPath);
}
Shader::~Shader() {
}

// Creation and compilation functions
GLuint Shader::CreateShaderObject(const std::string& strShaderFile, const GLenum& typeOfShader) {
	GLuint shaderID = glCreateShader(typeOfShader);

	const char *strFileData = strShaderFile.c_str();
	glShaderSource(shaderID, 1, &strFileData, NULL);
	glCompileShader(shaderID);

	// Check the compilation status of the shader
	GLint shaderCompilationStatus;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderCompilationStatus);
	if (!shaderCompilationStatus) {
		// Failure, get the errors and output them to the log for debugging
		GLint infoLogLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shaderID, infoLogLength, NULL, strInfoLog);

		// Get our shader type in a text format
		const char *strShaderType = NULL;
		switch (typeOfShader) {
		case GL_VERTEX_SHADER:
			strShaderType = "vertex";
			break;
		case GL_FRAGMENT_SHADER:
			strShaderType = "fragment";
			break;
		}

		// Cleanup and output
		Engine_Pointer->engineDebugger.WriteLine(">>>> ERROR!!!! - Compile Failure in " + (std::string)strShaderType + " - compilation error: " + (std::string)strInfoLog);
		glDeleteShader(shaderID);
		delete[] strInfoLog;
		return -1;
	}
	else {
		// Success!
		return shaderID;
	}
}
bool Shader::CompileShaderProgram(const std::string& vertexSourcePath, const std::string& fragmentSourcePath) {
	// Create our shader objects
	GLuint vertexID = CreateShaderObject(vertexSourcePath, GL_VERTEX_SHADER);
	GLuint fragmentID = CreateShaderObject(fragmentSourcePath, GL_FRAGMENT_SHADER);

	// Check the shader objects have been successfully created
	if (vertexID != -1) {
		if (fragmentID != -1) {

			// Compile each of our shader objects into our final shader
			program = glCreateProgram();
			glAttachShader(program, vertexID);
			glAttachShader(program, fragmentID);
			glLinkProgram(program);

			// Get our compilation result, old C style
			GLint programLinkerStatus;
			glGetProgramiv(program, GL_LINK_STATUS, &programLinkerStatus);

			// Check our compilation result
			if (programLinkerStatus) {
				// If successful, clean up and return true
				Engine_Pointer->engineDebugger.WriteLine(">>>> Shader Compiled! - " + name);
				glDeleteShader(vertexID);
				glDeleteShader(fragmentID);
				return true;
			}
			else {
				// If the program fails to compile, print the errors and clean up
				GLint infoLogLength;
				glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
				GLchar *strInfoLog = new GLchar[infoLogLength + 1];
				glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
				Engine_Pointer->engineDebugger.WriteLine(">>>> ERROR!!!! - Linker failure:" + (std::string)strInfoLog);
				glDeleteShader(vertexID);
				glDeleteShader(fragmentID);
				delete[] strInfoLog;
				return false;
			}
		}
		else {
			Engine_Pointer->engineDebugger.WriteLine(">>>> ERROR!!!! - Fragement Shader Failure" + name);
			return false;
		}
	}
	else {
		Engine_Pointer->engineDebugger.WriteLine(">>>> ERROR!!!! - Vertex Shader Failure" + name);
		return false;
	}
}
std::string Shader::ReadSourceFile(const std::string& filePath) {
	// Read the Shader file
	std::string rawFileString = "";
	std::ifstream fileReader(filePath);
	if (!fileReader.is_open()) {
		Engine_Pointer->engineDebugger.WriteLine(">>>> ERROR!!!! - Shader File not found - " + filePath);
		return "";
	}
	std::string line = "";
	while (getline(fileReader, line)) {
		rawFileString += line + "\n";
	}
	fileReader.close();

	Engine_Pointer->engineDebugger.WriteLine(">>>> Shader File Loaded! - " + filePath);
	return rawFileString;
}
bool Shader::Load(const std::string& vertexSourcePath, const std::string& fragmentSourcePath) {
	// Variables to store the raw source for the parts of the shader
	std::string vertexSource = "";
	std::string fragmentSource = "";

	// Check we've been given a pathway for the vertex shader source code
	if (vertexSourcePath != "") {
		vertexSource = ReadSourceFile(vertexSourcePath);
	}
	else {
		Engine_Pointer->engineDebugger.WriteLine(">>>> ERROR!!!! - Vertex Shader File Path not supplied");
		return false;
	}

	// Check we've been given a pathway for the fragment shader source code
	if (fragmentSourcePath != "") {
		fragmentSource = ReadSourceFile(fragmentSourcePath);
	}
	else {
		Engine_Pointer->engineDebugger.WriteLine(">>>> ERROR!!!! - Fragment Shader File Path not supplied");
		return false;
	}

	// Compile our shader program
	return CompileShaderProgram(vertexSource, fragmentSource);
}

// Access and Activation functions
const GLuint* Shader::GetShader(void) {
	return &this->program;
}
const bool Shader::IsLoaded(void) {
	return isLoaded;
}
const std::string Shader::GetName(void) {
	return this->name;
}
void Shader::Activate(void) {
	// Tells the engine to use this shader for the Current Draw.
	glUseProgram(program);
}
