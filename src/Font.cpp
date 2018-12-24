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
			// Get the fonts settings from the script
			path = script->Get<std::string>("font.file_name");
			fontSize = script->Get<int>("font.size");

			// Try and load the font face
			if (!FT_New_Face(Engine_Pointer->freeTypeLibrary, (Engine_Pointer->contentDirectory + "fonts/source/" + path).c_str(), 0, &freeTypeFace)) {
				name = freeTypeFace->family_name;				// The friendly name for the font
				FT_Set_Pixel_Sizes(freeTypeFace, 0, fontSize);	// Set the size of the font face
				isLoaded = LoadGlyphs();						// Split the loaded font face into its glyphs for rendering
			}
			else {
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

bool Font::LoadGlyphs(void) {
	// Go through each ASCII character between 32 and 126 in the standardised ASCII Table and try and load the character
	for (size_t i = 32; i <= 126; i++) {
		char charToLoad = char(i);	// Convert the ASCII Decima value to its character format
		
		// If FT_Get_Char_Index returns anything other than 0, the character is present
		if (FT_Get_Char_Index(freeTypeFace, charToLoad) != 0) {
			// Try and load the Character into the FontFace's glyph slot for use
			if (!FT_Load_Char(freeTypeFace, charToLoad, FT_LOAD_RENDER)) {
				std::cout << ">>>> Character: " << charToLoad << " was loaded into the Glyphslot for the face" << std::endl;

				// Get Glyph Metrics


				// Create Mesh object for the Glyph


				// Get the Texture of the Glyph for Rendering
				
				
			}
			else {
				std::cout << ">>>> Character: " << charToLoad << " was not loaded successfully" << std::endl;
				return false;
			}
		}
		else {
			// Output Error - Character missing from TTF file
			std::cout << i << " - " << charToLoad << ": was not found in the Font Face file supplied" << std::endl;
		}
	}
	return true;
}

Glyph Font::GetGlyph(const char & character){
	return glyphs[character];
}
