#ifndef ARKENGINE_LEVEL_H_
#define ARKENGINE_LEVEL_H_
#include <string>
#include <glm/glm.hpp>
#include "LuaScript.h"
#include "Tile.h"

class Engine;
class BoundingBox;

class Level
{
public:
	static Engine* Engine_Pointer;
	std::string filePath;
	std::string nameOfTilest;
	int indexOfTileset;
	glm::vec2 tileGridSize;
	glm::vec2 pixelGridSize;
	glm::vec2 playerStartPosition;
	std::vector<Tile*> tileMap;

	Level(const std::string& filePath);
	~Level();

	void Update(const float& deltaTime);
	void Draw();
	bool IsTileSolid(const glm::vec2& gridPosition);
	BoundingBox GetTileBoundingBox(const glm::vec2& gridPosition);

private:
	LuaScript* script;

	void Load();
};
#endif