#ifndef ARKENGINE_TEXTURE_HPP_
#define ARKENGINE_TEXTURE_HPP_

#include <string>
#include <GL/glew.h>

class Texture {
public:
	std::string name;
	GLuint id;
	glm::ivec2 dimensions;

	Texture(const std::string& name) {
		this->name = name;
		this->id = -1;
		this->dimensions = glm::ivec2(0, 0);
	}
	~Texture() {

	}
};
#endif
