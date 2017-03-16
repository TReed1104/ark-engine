#ifndef ARKENGINE_TILESET_H_
#define ARKENGINE_TILESET_H_
#include <vector>
#include "Tile.h"
class Engine;

class Tileset
{
public:
	std::string name;
	std::vector<Tile*> tileList;

	Tileset(const Engine& engine, const std::string& name);
	~Tileset();

	void Load(const std::string& tilesetScript);

private:
	const Engine* engine;
};

#endif