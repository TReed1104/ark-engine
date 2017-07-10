#ifndef ARKENGINE_FONT_H_
#define ARKENGINE_FONT_H_

#include <iostream>
#include <string>
#include <map>
#include <SDL_ttf.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Glyph.h"
#include "LuaScript.h"
#include "Texture.hpp"
#include "Model.h"

class Engine;

class Font {
public:
	static Engine* Engine_Pointer;

	LuaScript* script;
	std::string path;
	int size;
	std::string name;
	TTF_Font* font;

	Font(const std::string& scriptPath = "NO SCRIPT");
	~Font();

	void LoadGlyphs(void);
	Glyph GetGlyph(const char& character);
	
private:
	std::map<char, Glyph> glyphs;
};
#endif
