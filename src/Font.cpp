#include "Font.h"
Font::Font(const FT_Library& library, const std::string& fontName) {
	if (FT_New_Face(library, fontName.c_str(), 0, &freeTypeFace)) {
		// Face failed to load, feedback to the engine for it to close
		isLoaded = false;
	}
	else {
		// Face has been created, now lets split it into our glyphs
		
		name = freeTypeFace->family_name;
		style = freeTypeFace->style_name;
		numberOfGlyphs = freeTypeFace->num_glyphs;


		isLoaded = true; // We've loaded the font, feedback for the engine to continue loading
	}
}

Font::~Font() {

}