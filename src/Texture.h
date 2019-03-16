#ifndef ARKENGINE_TEXTURE_H_
#define ARKENGINE_TEXTURE_H_

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Engine;

class Texture {
public:
	static Engine* Engine_Pointer;
	GLuint textureID;
	glm::ivec2 dimensionsInPixels;
	glm::ivec2 dimensionsInFrames;
	glm::ivec2 frameSize;
	glm::ivec2 frameSizeWithBorder;
	int numberOfFrames;

	Texture(const std::string& name = "", const std::string& filePath = "", const bool& load = false, const bool& isTextureArray = false);
	~Texture();

	const std::string GetName(void);
	const bool IsLoaded(void);


private:
	std::string name;
	bool isLoaded;

	bool ImportTexture(const std::string& filepath);
	bool ImportTextureArray(const std::string& filepath);
};
#endif
