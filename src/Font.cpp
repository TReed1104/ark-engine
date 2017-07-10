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
				// Glyph model setup
				newGlyph.model = Model(std::to_string(currentChar));
				Model::Mesh mesh = Model::Mesh();
				// Calculate the vertex locations of the renderable surface for the glyph
				newGlyph.width = newGlyph.maxX - newGlyph.minX;
				newGlyph.height = newGlyph.maxY - newGlyph.minY;
				// Calculate the four vertex positions using the size of the glyph
				mesh.vertexPositions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
				mesh.vertexPositions.push_back(glm::vec3(0.0f + newGlyph.width, 0.0f, 0.0f));
				mesh.vertexPositions.push_back(glm::vec3(0.0f + newGlyph.width, 0.0f + newGlyph.height, 0.0f));
				mesh.vertexPositions.push_back(glm::vec3(0.0f, 0.0f + newGlyph.height, 0.0f));

				mesh.uvs.push_back(glm::vec2(0.0f, 0.0f));
				mesh.uvs.push_back(glm::vec2(1.0f, 0.0f));
				mesh.uvs.push_back(glm::vec2(1.0f, 1.0f));
				mesh.uvs.push_back(glm::vec2(0.0f, 1.0f));

				// Add raw colour data to each vertex incase none is passed to the shader
				const size_t numberOfVertices = mesh.vertexPositions.size();
				for (size_t i = 0; i < numberOfVertices; i++) {
					mesh.colourData.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
				}

				// Triangle 0 of the surface
				mesh.indices.push_back(1);
				mesh.indices.push_back(2);
				mesh.indices.push_back(3);
				// Triangle 1 of the surface
				mesh.indices.push_back(1);
				mesh.indices.push_back(3);
				mesh.indices.push_back(4);

				// Generate the surface
				mesh.BindBuffers();
				newGlyph.model.meshes.push_back(mesh);
				newGlyph.model.SetMeshParents();
				
				// Set the default model transformations
				newGlyph.model.Translate(glm::vec3(0.0f));
				newGlyph.model.Rotate(0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
				newGlyph.model.Scale(glm::vec3(1.0f));

				// Texture Generation
				// Setup the defaults of the Glyph Texture
				SDL_Color colour = { 255, 255, 255, 255 };
				SDL_Surface* glyphImage = TTF_RenderText_Blended(font, &currentChar, colour);
				newGlyph.texture.dimensionsInPixels = glm::ivec2(glyphImage->w, glyphImage->h);
				newGlyph.texture.dimensionsInFrames = glm::ivec2(1, 1);
				newGlyph.texture.frameSize = glm::ivec2(glyphImage->w, glyphImage->h);
				newGlyph.texture.frameSizeBordered = glm::ivec2(0, 0);
				newGlyph.texture.numberOfFrames = 1;

				// OpenGL side of texture setup
				glGenTextures(1, &newGlyph.texture.id);
				glBindTexture(GL_TEXTURE_2D, newGlyph.texture.id);
				GLenum textureFormat = GL_RGBA8;
				GLint internalFormat = GL_RGBA;
				if (glyphImage->format->BytesPerPixel == 4) {
					textureFormat = GL_RGBA8;
					internalFormat = GL_RGBA;
				}
				else {
					textureFormat = GL_RGB8;
					internalFormat = GL_RGB;
				}
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, glyphImage->w, glyphImage->h, 0, textureFormat, GL_UNSIGNED_BYTE, glyphImage->pixels);
				
				// Wrapping settings
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

				// Filtering settings
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

				glBindTexture(GL_TEXTURE_2D, 0);
				SDL_FreeSurface(glyphImage);


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

Font::Glyph Font::GetGlyph(const char & character) {
	return glyphs[character];
}

Font::Glyph::Glyph(const char& character) {
	this->texture = Texture(std::to_string(character));
	this->character = character;
	this->minX = -1;
	this->maxX = -1;
	this->minY = -1;
	this->maxY = -1;
	this->advance = -1;
	this->width = -1;
	this->height = -1;
}
Font::Glyph::~Glyph() {

}
