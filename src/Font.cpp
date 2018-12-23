#include "Font.h"
Font::Font(const FT_Library& library, const std::string& fontName) {
	if (FT_New_Face(library, fontName.c_str(), 0, &freeTypeFace)) {
		isLoaded = false;
	}
	else {
		isLoaded = true;	
	}
}

Font::~Font() {

}