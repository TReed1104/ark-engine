#ifndef ARKENGINE_TEXTURE_HPP_
#define ARKENGINE_TEXTURE_HPP_

#include <string>
#include <GL/glew.h>

class Texture {
public:
	std::string name;
	GLuint id;
	glm::ivec2 dimensionsInPixels;
	glm::ivec2 dimensionsInTiles;
	glm::ivec2 frameSize;
	glm::ivec2 frameSizeBordered;
	int numberOfFrames;

	Texture(const std::string& name) {
		this->name = name;
		this->id = -1;
		this->dimensionsInPixels = glm::ivec2(0, 0);
		this->dimensionsInTiles = glm::ivec2(0, 0);
		this->frameSize = glm::ivec2(0, 0);
		this->frameSizeBordered = glm::ivec2(0, 0);
		this->numberOfFrames = 0;
	}
	~Texture() {

	}
};
#endif
