#ifndef ARKENGINE_TILESET_H_
#define ARKENGINE_TILESET_H_
#include <vector>
#include "Tile.h"
class Engine;

class Tileset
{
public:
	static Engine* Engine_Pointer;
	std::string name;
	std::vector<Tile> tileList;

	Tileset(const std::string& tilesetScript);
	~Tileset();


private:
	void Load(const std::string& tilesetScript);
};

#endif