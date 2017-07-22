#include "Font.h"
#include "Engine.h"

Engine* Font::Engine_Pointer;

Font::Font(const std::string & scriptPath) {
	this->script = nullptr;
	path = "NOT LOADED";
	size = 0;
	font = nullptr;
	name = "NOT LOADED";

	// Load the script config for the font and load it into memory.
	if (scriptPath != "NO SCRIPT") {
		this->script = new LuaScript(scriptPath);
		if (script->isScriptLoaded) {
			path = script->Get<std::string>("font.file_name");
			size = script->Get<int>("font.size");
			font = TTF_OpenFont((Engine_Pointer->contentDirectory + "fonts/" + path).c_str(), size);
			name = script->Get<std::string>("font.name");

			// Go through and load each of the glyphs.
			if (font) {
				std::cout << "Font " << name << " was opened successfully." << std::endl;
				LoadGlyphs();
			}
			else {
				std::cout << "Font " << name << " was not opened successfully." << std::endl;
			}
		}
	}
}
Font::~Font() {
	// Clean up the font
	if (script->isScriptLoaded) {
		delete script;
	}
	if (font) {
		TTF_CloseFont(font);
	}
}
void Font::LoadGlyphs(void) {
	// Indexes of usable glyphs
	int startIndexOfGlyphs = 32;
	int lastIndexOfGlyphs = 126;

	for (int i = startIndexOfGlyphs; i <= lastIndexOfGlyphs; i++) {
		int indexOfGlyph = TTF_GlyphIsProvided(font, i);
		if (indexOfGlyph) {
			//std::cout << "Glyph " << i << " was found in the font " << name << " at index " << indexOfGlyph << std::endl;

			char currentChar = char(i);
			Glyph newGlyph = Glyph(currentChar);
			if (TTF_GlyphMetrics(font, newGlyph.character, &newGlyph.minX, &newGlyph.maxX, &newGlyph.minY, &newGlyph.maxY, &newGlyph.advance) != -1) {
				// Glyph mesh setup
				newGlyph.mesh = Model::Mesh();
				// Calculate the vertex locations of the renderable surface for the glyph
				newGlyph.width = newGlyph.maxX - newGlyph.minX;
				newGlyph.height = newGlyph.maxY - newGlyph.minY;
				// Calculate the four vertex positions using the size of the glyph
				newGlyph.mesh.vertexPositions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
				newGlyph.mesh.vertexPositions.push_back(glm::vec3(0.0f + newGlyph.width, 0.0f, 0.0f));
				newGlyph.mesh.vertexPositions.push_back(glm::vec3(0.0f + newGlyph.width, 0.0f + newGlyph.height, 0.0f));
				newGlyph.mesh.vertexPositions.push_back(glm::vec3(0.0f, 0.0f + newGlyph.height, 0.0f));

				newGlyph.mesh.uvs.push_back(glm::vec2(0.0f, 0.0f));
				newGlyph.mesh.uvs.push_back(glm::vec2(1.0f, 0.0f));
				newGlyph.mesh.uvs.push_back(glm::vec2(1.0f, 1.0f));
				newGlyph.mesh.uvs.push_back(glm::vec2(0.0f, 1.0f));
				newGlyph.mesh.isSetupForTextures = true;

				// Add raw colour data to each vertex incase none is passed to the shader
				const size_t numberOfVertices = newGlyph.mesh.vertexPositions.size();
				for (size_t i = 0; i < numberOfVertices; i++) {
					newGlyph.mesh.colourData.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
				}

				// Triangle 0 of the surface
				newGlyph.mesh.indices.push_back(1);
				newGlyph.mesh.indices.push_back(2);
				newGlyph.mesh.indices.push_back(3);
				// Triangle 1 of the surface
				newGlyph.mesh.indices.push_back(1);
				newGlyph.mesh.indices.push_back(3);
				newGlyph.mesh.indices.push_back(4);

				// Generate the surface
				newGlyph.mesh.BindBuffers();

				// Texture Generation
				// Setup the defaults of the Glyph Texture
				SDL_Color colour = { 255, 255, 255, 255 };
				SDL_Surface* glyphSurface = TTF_RenderUTF8_Blended(font, &currentChar, colour);
				newGlyph.texture.dimensionsInPixels = glm::ivec2(glyphSurface->w, glyphSurface->h);
				newGlyph.texture.dimensionsInFrames = glm::ivec2(1, 1);
				newGlyph.texture.frameSize = glm::ivec2(glyphSurface->w, glyphSurface->h);
				newGlyph.texture.frameSizeBordered = glm::ivec2(0, 0);
				newGlyph.texture.numberOfFrames = 1;

				// OpenGL side of texture setup
				glGenTextures(1, &newGlyph.texture.id);
				glBindTexture(GL_TEXTURE_2D, newGlyph.texture.id);
				
				int colours = glyphSurface->format->BytesPerPixel;
				GLenum textureFormat = GL_RGBA;
				if (colours == 4) {
					textureFormat = GL_RGBA;
				}
				else {
					textureFormat = GL_RGB;
				}
				glTexImage2D(GL_TEXTURE_2D, 0, colours, glyphSurface->w, glyphSurface->h, 0, textureFormat, GL_UNSIGNED_BYTE, glyphSurface->pixels);
				
				// Wrapping settings
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

				// Filtering settings
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

				glBindTexture(GL_TEXTURE_2D, 0);
				SDL_FreeSurface(glyphSurface);

				// Add the newly created glyph to the glyph dictionary
				glyphs[currentChar] = newGlyph;
			}
			else {
				std::cout << "TTF Error: " << TTF_GetError() << std::endl;
			}
		}
		else {
			std::cout << "Glyph " << i << " was not found in the font " << name << std::endl;
		}
	}
}
Glyph Font::GetGlyph(const char & character) {
	return glyphs[character];
}
