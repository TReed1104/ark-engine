#include "Font.h"
Font::Font(const FT_Library& library, const std::string& fontName) {
	if (!FT_New_Face(library, fontName.c_str(), 0, &freeTypeFace)) {
		// Face has been created, now lets split it into our glyphs

		name = freeTypeFace->family_name;
		style = freeTypeFace->style_name;
		fontSize = 48;

		FT_Set_Pixel_Sizes(freeTypeFace, 0, fontSize);
		
		// Standard character glyphs fall within 32 and 127 on the ASCII Character table
		for (size_t i = 32; i < 127; i++) {
			char charToLoad = char(i);
			int index = FT_Get_Char_Index(freeTypeFace, charToLoad);	// If the index is 0, the glyph is missing

			// If the character is present, lets create a glyph object
			if (index != 0) {
				if (!FT_Load_Char(freeTypeFace, charToLoad, FT_LOAD_RENDER)) {
					std::cout << ">>>> Character: " << charToLoad << " was loaded into the Glyphslot for the face" << std::endl;
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
		isLoaded = true; // We've loaded the font, feedback for the engine to continue loading
	}
	else {
		// Face failed to load, feedback to the engine for it to close
		isLoaded = false;
	}
}

Font::~Font() {
}