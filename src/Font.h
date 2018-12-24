#ifndef ARKENGINE_FONT_H_
#define ARKENGINE_FONT_H_

#include <iostream>
#include <string>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "LuaScript.h"
#include "Texture.h"
#include "Model.h"
#include "Glyph.h"

class Font {
public:
	static Engine* Engine_Pointer;

	LuaScript* script;
	std::string path;
	int fontSize;
	std::string name;
	bool isLoaded;

	Font(const std::string& scriptPath = "NO SCRIPT");
	~Font();

	bool LoadGlyphs(void);
	Glyph GetGlyph(const char& character);

private:
	FT_Face freeTypeFace;
	std::map<char, Glyph> glyphs;
};

#endif