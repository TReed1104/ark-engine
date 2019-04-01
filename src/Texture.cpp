#include "Texture.h"
#include "Engine.h"
#include "Texture.h"

Engine* Texture::Engine_Pointer;

Texture::Texture(const std::string& name, const std::string& filePath, const bool& load, const bool & isTextureArray) {
	this->name = name;
	this->isLoaded = false;
	this->textureID = -1;
	this->dimensionsInPixels = glm::ivec2(0, 0);
	this->dimensionsInFrames = glm::ivec2(0, 0);
	this->frameSize = glm::ivec2(0, 0);
	this->frameSizeWithBorder = glm::ivec2(0, 0);
	this->numberOfFrames = 0;
	if (load) {
		if (!isTextureArray) {
			isLoaded = ImportTexture(filePath);
		}
		else {
			isLoaded = ImportTextureArray(filePath);
		}
	}
}
Texture::~Texture() {
}

const std::string Texture::GetName(void) {
	return name;
}
const bool Texture::IsLoaded(void) {
	return isLoaded;
}
bool Texture::ImportTexture(const std::string& filepath) {
	SDL_Surface* image = IMG_Load(filepath.c_str());	// Produces an error, need to look into it ICCP - sRGB profile?

	if (image != NULL) {
		Engine_Pointer->engineDebugger.WriteLine(">>>> Texture Loaded! - " + name);
	}
	else {
		// If the texture was not loaded correctly, quit the program and show a error message on the console.
		Engine_Pointer->engineDebugger.WriteLine(">>>> ERROR!!!! - Failed to load: " + name);
		return false;
	}

	// Set up the texture metadata, as its not an array we are just using one texture
	dimensionsInPixels = glm::ivec2(image->w, image->h);
	dimensionsInFrames = glm::ivec2(1, 1);
	frameSize = glm::ivec2(image->w, image->h);
	frameSizeWithBorder = glm::ivec2(image->w, image->h);
	numberOfFrames = 1;

	// Generate the texture buffer
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Setup the colour format to use
	Uint8 colours = image->format->BytesPerPixel;
	GLenum textureFormat = GL_RGBA;
	if (colours == 4) {
		textureFormat = GL_RGBA;
	}
	else {
		textureFormat = GL_RGB;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, image->w, image->h, 0, textureFormat, GL_UNSIGNED_BYTE, image->pixels);

	// Wrapping Settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Filtering settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);			// Unbind the texture
	SDL_FreeSurface(image);							// Clear up the memory used by SDL's image loader.
	return true;
}
bool Texture::ImportTextureArray(const std::string& filepath) {
	SDL_Surface* image = IMG_Load(filepath.c_str());	// Produces an error, need to look into it ICCP - sRGB profile?

	if (image == NULL) {
		// If the texture was not loaded correctly, quit the program and show a error message on the console.
		Engine_Pointer->engineDebugger.WriteLine(">>>> ERROR!!!! - Failed to load: " + name);
		return false;
	}
	else {
		Engine_Pointer->engineDebugger.WriteLine(">>>> Texture Loaded! - " + name);
	}
	// Create the texture
	dimensionsInPixels = glm::ivec2(image->w, image->h);

	// Base framesize to be used for the texture
	frameSize = Engine_Pointer->tileTextureFrameSize;

	// Work out what frame size to use using the texture naming conventions.
	if (name.find("entity") != std::string::npos) {
		frameSize = Engine_Pointer->entityTextureFrameSize;
	}

	// Size of each frame including its border for differentiation
	frameSizeWithBorder = frameSize + (Engine_Pointer->textureBorderSize * 2);
	dimensionsInFrames = glm::ivec2(image->w / frameSizeWithBorder.x, image->h / frameSizeWithBorder.y);
	numberOfFrames = (dimensionsInFrames.x * dimensionsInFrames.y);

	// Initialise the texture buffer
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D_ARRAY, textureID);

	// Setup the colour format to use
	Uint8 colours = image->format->BytesPerPixel;
	GLenum textureFormat = GL_RGBA8;
	if (colours == 4) {
		textureFormat = GL_RGBA8;
	}
	else {
		textureFormat = GL_RGB8;
	}

	// Initialise the size of the 3D texture array
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, textureFormat, frameSize.x, frameSize.y, numberOfFrames);

	int textureArrayLayerIndexer = 0;	// Stores the current layer level we are putting the new texture into
	for (int y = 0; y < dimensionsInFrames.y; y++) {
		for (int x = 0; x < dimensionsInFrames.y; x++) {
			// Works out how to unpack and grab the correct part of the texture for the frame
			glPixelStorei(GL_UNPACK_ROW_LENGTH, image->w);
			glPixelStorei(GL_UNPACK_SKIP_PIXELS, (frameSizeWithBorder.x * x) + Engine_Pointer->textureBorderSize.x);
			glPixelStorei(GL_UNPACK_SKIP_ROWS, (frameSizeWithBorder.y * y) + Engine_Pointer->textureBorderSize.y);
			// Store the part of the texture into the array
			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, textureArrayLayerIndexer, frameSize.x, frameSize.y, 1, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
			textureArrayLayerIndexer++;		// Increment the indexer
		}
	}

	// Wrapping settings
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Filtering settings
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);			// Unbind the texture
	SDL_FreeSurface(image);							// Clear up the memory used by SDL's image loader.
	return true;
}
