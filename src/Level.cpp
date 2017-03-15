#include "Level.h"
#include "Engine.h"

Level::Level(const Engine& engine, const std::string & filePath) {
	this->engine = &engine;
	Load(filePath);
}

Level::~Level() {
	delete script;
}

void Level::Load(const std::string & filePath) {
	script = new LuaScript(filePath);
	if (script->isScriptLoaded) {
		tileGridSize = glm::vec2(script->Get<int>("map.tile_grid_size.x"), script->Get<int>("map.tile_grid_size.y"));
		playerStartPosition = glm::vec2(script->Get<int>("map.player_start_grid_position.x"), script->Get<int>("map.player_start_grid_position.y"));
		std::vector<int> rawMapData = script->GetVector<int>("map.map_data");
	}
}