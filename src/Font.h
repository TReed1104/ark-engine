#ifndef ARKENGINE_FONT_H_
#define ARKENGINE_FONT_H_

#include <iostream>
#include <string>
#include <SDL_ttf.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "LuaScript.h"


class Font {
public:
	class Glyph {
	public:
		// OpenGL rendering
		GLuint vertexBufferObject;
		GLuint indicesBufferObject;
		GLuint vertexArrayObject;

		// Glyph metrics
		int minX;
		int maxX;
		int minY;
		int maxY;
		int advance;

		Glyph() {}
		~Glyph() {}

	private:

	};

	LuaScript* script;
	std::string path;
	int size;
	std::string name;
	TTF_Font* font;

	Font(const std::string& scriptPath = "NO SCRIPT");
	~Font();

	void LoadGlyphs(void);
	
private:

};
#endif
