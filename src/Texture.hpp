#ifndef ARKENGINE_TEXTURE_HPP_
#define ARKENGINE_TEXTURE_HPP_

#include <string>
#include <GL/glew.h>

class Texture {
public:
	std::string name;
	GLuint id;

	Texture(const std::string& name) {
		this->name = name;
		this->id = -1;
	}
	~Texture() {

	}
};
#endif
