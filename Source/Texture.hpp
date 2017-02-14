#ifndef Texture_hpp_included
#define Texture_hpp_included

#include <string>
#include <GL/glew.h>

class Texture {
public:
	std::string name;
	GLuint textureID;

	Texture(std::string name) {
		this->name = name;
		this->textureID = -1;
	}
	~Texture() {

	}
};
#endif
