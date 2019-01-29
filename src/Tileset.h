#ifndef ARKENGINE_TILESET_H_
#define ARKENGINE_TILESET_H_
#include <vector>
#include "Tile.h"
class Engine;

class Tileset {
public:
	static Engine* Engine_Pointer;

	Tileset(const std::string& tilesetConfig);
	~Tileset();

	const std::string GetName(void);
	const bool IsLoaded(void);
	const std::vector<Tile>* GetTiles(void);
	std::vector<Tile> tileList;


private:
	JsonFile* configFile = nullptr;
	std::string name;
	bool isLoaded;

	bool Load(void);
};

#endif