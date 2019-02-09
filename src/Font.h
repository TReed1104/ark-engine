#ifndef ARKENGINE_FONT_H_
#define ARKENGINE_FONT_H_

#include <string>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Texture.h"
#include "Model.h"
#include "Glyph.h"

class Font {
public:
	static Engine* Engine_Pointer;

	std::string path;
	int fontSize;
	std::string name;
	bool isLoaded;

	Font(const std::string& fontName = "NOT LOADED", const std::string& fontPath = "NOT LOADED", const int& fontSize = 12);
	~Font();

	bool LoadGlyphs(void);
	Glyph GetGlyph(const char& character);

private:
	FT_Face fontFace;
	std::map<char, Glyph> glyphs;
};

#endif