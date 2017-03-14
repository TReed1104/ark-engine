#ifndef ARKENGINE_TEXTURE_HPP_
#define ARKENGINE_TEXTURE_HPP_

#include <string>
#include <GL/glew.h>

class Texture {
public:
	std::string name;
	GLuint textureID;

	Texture(const std::string& name) {
		this->name = name;
		this->textureID = -1;
	}
	~Texture() {

	}
};
#endif
