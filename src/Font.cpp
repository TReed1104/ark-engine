#include "Font.h"

Font::Font(const std::string & scriptPath) {
	// Load the script config for the font and load it into memory.
	if (scriptPath != "NO SCRIPT") {
		this->script = new LuaScript(scriptPath);
		if (script->isScriptLoaded) {
			path = script->Get<std::string>("font.file_name");
			size = script->Get<int>("font.size");
			font = TTF_OpenFont(path.c_str(), size);
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
		else {
			this->script = nullptr;
			path = "NOT LOADED";
			size = 0;
			font = nullptr;
			name = "NOT LOADED";
		}
	}
	else {
		this->script = nullptr;
		path = "NOT LOADED";
		size = 0;
		font = nullptr;
		name = "NOT LOADED";
	}
}
Font::~Font() {
	// Clean up the font
	if (script != nullptr) {
		delete script;
	}
	if (font) {
		TTF_CloseFont(font);
	}
}
void Font::LoadGlyphs(void) {
	int numberOfCharacters = 128;	// Set to 128 as thats the default number of characters in ASCII without extension
	for (int i = 0; i < numberOfCharacters; i++) {
		int indexOfGlyph = TTF_GlyphIsProvided(font, i);
		if (indexOfGlyph) {
			std::cout << "Glyph " << i << " was found in the font " << name << " at index" << indexOfGlyph << std::endl;
		}
		else {
			std::cout << "Glyph " << i << " was not found in the font " << name << std::endl;
		}
	}
}