#ifndef ARKENGINE_LEVEL_H_
#define ARKENGINE_LEVEL_H_
#include <string>
#include <glm/glm.hpp>
#include "LuaScript.h"
#include "Tile.h"

class Engine;

class Level
{
public:
	glm::vec2 tileGridSize;
	glm::vec2 playerStartPosition;
	Tile* tileMap;
	Tile* test;

	Level(const Engine& engine, const std::string& filePath);
	~Level();

	void Update(float deltaTime);
	void Draw();

private:
	const Engine* engine;
	LuaScript* script;

	void Load(const std::string& filePath);
};
#endif