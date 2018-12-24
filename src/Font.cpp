#include "Font.h"
#include "Engine.h"
#include <math.h>

Engine* Font::Engine_Pointer;

Font::Font(const std::string& scriptPath) {
	this->script = nullptr;
	path = "NOT LOADED";
	fontSize = 12;
	name = "NOT LOADED";
	isLoaded = false;

	if (scriptPath != "NO SCRIPT") {
		this->script = new LuaScript(scriptPath);
		if (script->isScriptLoaded) {
			path = script->Get<std::string>("font.file_name");
			fontSize = script->Get<int>("font.size");

			if (!FT_New_Face(Engine_Pointer->freeTypeLibrary, (Engine_Pointer->contentDirectory + "fonts/source/" + path).c_str(), 0, &freeTypeFace)) {
				// Font face has been successfully loaded

				name = freeTypeFace->family_name;
				FT_Set_Pixel_Sizes(freeTypeFace, 0, fontSize);	// Set the size of the font face

				LoadGlyphs();	// Load the Glyphs

				isLoaded = true;
			}
			else {
				// Face failed to load, tell the engine to exit()
				isLoaded = false;
			}
		}
	}
}

Font::~Font() {
	if (script->isScriptLoaded) {
		delete script;
	}
}

void Font::LoadGlyphs(void) {
	// Split the loaded font face into its glyphs for rendering
	
	for (size_t i = 32; i <= 126; i++) {		// Standard character glyphs fall within 32 and 126 on the ASCII Character table
		char charToLoad = char(i);
		int index = FT_Get_Char_Index(freeTypeFace, charToLoad);	// If the index is 0, the glyph is missing

		// If the character is present, lets create a glyph object
		if (index != 0) {
			if (!FT_Load_Char(freeTypeFace, charToLoad, FT_LOAD_RENDER)) {
				// Load the character into the faces glyph slot to be used for creating our glyph object of the character
				std::cout << ">>>> Character: " << charToLoad << " was loaded into the Glyphslot for the face" << std::endl;

				// Get Glyph Metrics


				// Create Mesh object for the Glyph


				// Get the Texture of the Glyph for Rendering


			}
			else {
				std::cout << ">>>> Character: " << charToLoad << " failed to load" << std::endl;
			}
		}
		else {
			// Output Error - Character missing from TTF file
			std::cout << i << " - " << charToLoad << ": was not found" << std::endl;
		}
	}
}

Glyph Font::GetGlyph(const char & character){
	return glyphs[character];
}
