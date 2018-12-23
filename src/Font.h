#ifndef ARKENGINE_FONT_H_
#define ARKENGINE_FONT_H_

#include <iostream>
#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H

class Font
{
public:
	bool isLoaded = false;
	std::string name = "";
	std::string style = "";
	int numberOfGlyphs = 0;

	Font(const FT_Library& library, const std::string& fontName);
	~Font();

private:
	FT_Face freeTypeFace;

};

#endif