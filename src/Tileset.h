#ifndef ARKENGINE_TILESET_H_
#define ARKENGINE_TILESET_H_
#include <vector>
#include "Tile.h"
class Engine;

class Tileset
{
public:
	static Engine* engine;
	std::string name;
	std::vector<Tile> tileList;

	Tileset();
	~Tileset();

	void Load(const std::string& tilesetScript);

private:
};

#endif