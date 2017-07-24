#include "Texture.h"
#include "Engine.h"
#include "Texture.h"

Engine* Texture::Engine_Pointer;

Texture::Texture(const std::string & path, const bool &load, const bool & isTextureArray) {
	this->name = path;
	bool isLoaded = false;
	this->id = -1;
	this->dimensionsInPixels = glm::ivec2(0, 0);
	this->dimensionsInFrames = glm::ivec2(0, 0);
	this->frameSize = glm::ivec2(0, 0);
	this->frameSizeBordered = glm::ivec2(0, 0);
	this->numberOfFrames = 0;
	if (load) {
		if (!isTextureArray) {
			ImportTexture();
		}
		else {
			ImportTextureArray();
		}
	}
}
Texture::~Texture() {
}

bool Texture::ImportTexture() {
	isLoaded = false;
	return false;
}
bool Texture::ImportTextureArray() {
	SDL_Surface* image = IMG_Load(name.c_str());	// Try and load the texture.
	if (image == NULL) {
		// If the texture was not loaded correctly, quit the program and show a error message on the console.
		std::cout << ">> The loading of Spritesheet: " << name << " failed." << std::endl;
		isLoaded = false;
		return false;
	}
	else {
		std::cout << ">> The loading of Spritesheet: " << name << " was successful." << std::endl;
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
	frameSizeBordered = frameSize + (Engine_Pointer->textureBorderSize * 2);
	dimensionsInFrames = glm::ivec2(image->w / frameSizeBordered.x, image->h / frameSizeBordered.y);
	numberOfFrames = (dimensionsInFrames.x * dimensionsInFrames.y);

	// Initialise the texture buffer
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D_ARRAY, id);
	// Initialise the size of the 3D texture array

	Uint8 colours = image->format->BytesPerPixel;
	GLenum textureFormat = GL_RGBA8;
	if (colours == 4) {
		textureFormat = GL_RGBA8;
	}
	else {
		textureFormat = GL_RGB8;
	}

	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, textureFormat, frameSize.x, frameSize.y, numberOfFrames);

	int textureArrayLayerIndexer = 0;	// Stores the current layer level we are putting the new texture into
	for (int y = 0; y < dimensionsInFrames.y; y++) {
		for (int x = 0; x < dimensionsInFrames.y; x++) {
			// Works out how to unpack and grab the correct part of the texture for the frame
			glPixelStorei(GL_UNPACK_ROW_LENGTH, image->w);
			glPixelStorei(GL_UNPACK_SKIP_PIXELS, (frameSizeBordered.x * x) + Engine_Pointer->textureBorderSize.x);
			glPixelStorei(GL_UNPACK_SKIP_ROWS, (frameSizeBordered.y * y) + Engine_Pointer->textureBorderSize.y);
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
	isLoaded = true;
	return true;
}
