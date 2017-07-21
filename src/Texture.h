#ifndef ARKENGINE_TEXTURE_H_
#define ARKENGINE_TEXTURE_H_

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Engine;

class Texture {
public:
	static Engine* Engine_Pointer;

	std::string name;
	bool isLoaded;
	GLuint id;
	glm::ivec2 dimensionsInPixels;
	glm::ivec2 dimensionsInFrames;
	glm::ivec2 frameSize;
	glm::ivec2 frameSizeBordered;
	int numberOfFrames;

	Texture(const std::string& path = "", const bool& load = true, const bool& isTextureArray = false);
	~Texture();

private:
	bool ImportTexture();
	bool ImportTextureArray();
};
#endif
