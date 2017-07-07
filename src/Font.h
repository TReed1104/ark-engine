#ifndef ARKENGINE_FONT_H_
#define ARKENGINE_FONT_H_

#include <string>
#include <SDL_ttf.h>
#include "LuaScript.h"


class Font {
public:
	LuaScript* script;
	std::string path;
	int size;
	std::string name;
	TTF_Font* font;

	Font(const std::string& scriptPath = "NO SCRIPT");
	~Font();
	
private:

};

Font::Font(const std::string & scriptPath) {
	// Load the script config for the font and load it into memory.
	if (scriptPath != "NO SCRIPT") {
		this->script = new LuaScript(scriptPath);
		if (script->isScriptLoaded) {
			path = script->Get<std::string>("font.path");
			size = script->Get<int>("font.size");
			font = TTF_OpenFont(path.c_str(), size);
			name = script->Get<std::string>("font.name");
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
	TTF_CloseFont(font);
}

#endif
