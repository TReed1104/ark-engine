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
				Model newModel = Model();
				Model::Mesh newMesh = Model::Mesh();
				// Calculate the vertex locations of the renderable surface for the glyph
				newGlyph.width = newGlyph.maxX - newGlyph.minX;
				newGlyph.height = newGlyph.maxY - newGlyph.minY;
				// Calculate the four vertex positions using the size of the glyph
				newMesh.vertexPositions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
				newMesh.vertexPositions.push_back(glm::vec3(0.0f + newGlyph.width, 0.0f, 0.0f));
				newMesh.vertexPositions.push_back(glm::vec3(0.0f + newGlyph.width, 0.0f + newGlyph.height, 0.0f));
				newMesh.vertexPositions.push_back(glm::vec3(0.0f, 0.0f + newGlyph.height, 0.0f));

				// Generate the surface
				newMesh.BindBuffers();
				newModel.meshes.push_back(newMesh);
				newModel.SetMeshParents();

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
