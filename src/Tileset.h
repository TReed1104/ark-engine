#ifndef ARKENGINE_TILESET_H_
#define ARKENGINE_TILESET_H_
#include <vector>
#include "Tile.h"

class Tileset
{
public:
	std::string name;
	std::vector<Tile> tileList;

	Tileset(const std::string& name);
	~Tileset();

private:

};

#endif