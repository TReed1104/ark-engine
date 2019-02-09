#include "Font.h"
#include "Engine.h"
#include <math.h>

Engine* Font::Engine_Pointer;

Font::Font(const std::string& fontName, const std::string& fontPath, const int& fontSize) {
	this->name = fontName;
	this->path = fontPath;
	this->fontSize = fontSize;
	this->isLoaded = false;

	if (name != "NOT LOADED") {
		// Try and load the font face
		if (!FT_New_Face(Engine_Pointer->freeTypeLibrary, path.c_str(), 0, &fontFace)) {
			FT_Set_Pixel_Sizes(fontFace, 0, this->fontSize);	// Set the size of the font face
			isLoaded = LoadGlyphs();							// Split the loaded font face into its glyphs for rendering
		}
		else {
			isLoaded = false;
		}
	}
}
Font::~Font() {
	if (fontFace != NULL) {
		FT_Done_Face(fontFace);
	}
}

bool Font::LoadGlyphs(void) {
	// Go through each ASCII character between 32 and 126 in the standardised ASCII Table and try and load the character
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for (size_t i = 32; i <= 126; i++) {
		char charToLoad = char(i);	// Convert the ASCII Decima value to its character format

		// If FT_Get_Char_Index returns anything other than 0, the character is present
		if (FT_Get_Char_Index(fontFace, charToLoad) != 0) {
			// Try and load the Character into the FontFace's glyph slot for use
			if (!FT_Load_Char(fontFace, charToLoad, FT_LOAD_RENDER)) {

				// Get Glyph Metrics
				Glyph newGlyph = Glyph(charToLoad);

				// Glyph Metrics provided by the FontDriver are stored in values of 1/64 of a pixel, so we bitshift by 6 to get the actual values
				newGlyph.size = glm::vec2(fontFace->glyph->metrics.width >> 6, fontFace->glyph->metrics.height >> 6) / Engine_Pointer->windowScaler;	// Adjust the size of the font to make up for the window scalar
				newGlyph.advance = glm::vec2(fontFace->glyph->metrics.horiAdvance >> 6, fontFace->glyph->metrics.vertAdvance >> 6) / Engine_Pointer->windowScaler;
				newGlyph.horizontalBearing = glm::vec2(fontFace->glyph->metrics.horiBearingX >> 6, fontFace->glyph->metrics.horiBearingY >> 6) / Engine_Pointer->windowScaler;
				newGlyph.verticalBearing = glm::vec2(fontFace->glyph->metrics.vertBearingX >> 6, fontFace->glyph->metrics.vertBearingY >> 6) / Engine_Pointer->windowScaler;

				// Create Mesh object for the Glyph
				newGlyph.mesh = Model::Mesh();
				newGlyph.mesh.vertexPositions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
				newGlyph.mesh.vertexPositions.push_back(glm::vec3(0.0f + newGlyph.size.x, 0.0f, 0.0f));
				newGlyph.mesh.vertexPositions.push_back(glm::vec3(0.0f + newGlyph.size.x, 0.0f + newGlyph.size.y, 0.0f));
				newGlyph.mesh.vertexPositions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
				newGlyph.mesh.vertexPositions.push_back(glm::vec3(0.0f + newGlyph.size.x, 0.0f + newGlyph.size.y, 0.0f));
				newGlyph.mesh.vertexPositions.push_back(glm::vec3(0.0f, 0.0f + newGlyph.size.y, 0.0f));
				newGlyph.mesh.uvs.push_back(glm::vec2(0.0f, 0.0f));
				newGlyph.mesh.uvs.push_back(glm::vec2(1.0f, 0.0f));
				newGlyph.mesh.uvs.push_back(glm::vec2(1.0f, 1.0f));
				newGlyph.mesh.uvs.push_back(glm::vec2(0.0f, 0.0f));
				newGlyph.mesh.uvs.push_back(glm::vec2(1.0f, 1.0f));
				newGlyph.mesh.uvs.push_back(glm::vec2(0.0f, 1.0f));
				newGlyph.mesh.isSetupForTextures = true;
				const size_t numberOfVertices = newGlyph.mesh.vertexPositions.size();
				for (size_t i = 0; i < numberOfVertices; i++) {
					newGlyph.mesh.colourData.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
					newGlyph.mesh.surfaceNormals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
				}
				newGlyph.mesh.indices.push_back(0);
				newGlyph.mesh.indices.push_back(1);
				newGlyph.mesh.indices.push_back(2);
				newGlyph.mesh.indices.push_back(3);
				newGlyph.mesh.indices.push_back(4);
				newGlyph.mesh.indices.push_back(5);
				newGlyph.mesh.BindBuffers();

				// Get the Texture of the Glyph for Rendering
				glGenTextures(1, &newGlyph.texture.textureID);
				glBindTexture(GL_TEXTURE_2D, newGlyph.texture.textureID);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, fontFace->glyph->bitmap.width, fontFace->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, fontFace->glyph->bitmap.buffer);
				// Wrapping settings
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				// Filtering settings
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

				newGlyph.texture.dimensionsInPixels = glm::ivec2(fontFace->glyph->bitmap.width, fontFace->glyph->bitmap.rows);
				newGlyph.texture.dimensionsInFrames = glm::ivec2(1, 1);
				newGlyph.texture.frameSize = glm::ivec2(fontFace->glyph->bitmap.width, fontFace->glyph->bitmap.rows);
				newGlyph.texture.frameSizeWithBorder = glm::ivec2(0, 0);
				newGlyph.texture.numberOfFrames = 1;

				// Clear up
				glBindTexture(GL_TEXTURE_2D, 0);

				glyphs[charToLoad] = newGlyph;
			}
			else {
				Engine_Pointer->engineDebugger.WriteLine(">>>> Character: " + std::to_string(charToLoad) + " was not loaded successfully");
				return false;
			}
		}
		else {
			// Output Error - Character missing from TTF file
			Engine_Pointer->engineDebugger.WriteLine(">>>> Character: " + std::to_string(charToLoad) + " was not found in the Font Face file supplied");
		}
	}
	Engine_Pointer->engineDebugger.WriteLine(">>>> Font Loaded! - " + name + " (Size - " + std::to_string(fontSize) + "}");
	return true;
}
Glyph Font::GetGlyph(const char& character) {
	if (glyphs.count(character) == 1) {
		return glyphs[character];
	}
	else {
		return glyphs['X'];
	}
}
