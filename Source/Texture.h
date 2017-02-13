#ifndef Texture_h_included
#define Texture_h_included

#include <string>
#include <GL/glew.h>

class Texture {
public:
	std::string name;
	GLuint textureID;

	Texture(std::string name);
	~Texture();
};

#endif
