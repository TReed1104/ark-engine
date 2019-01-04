#include "Level.h"
#include "Engine.h"
#include "BoundingBox.h"

Engine* Level::Engine_Pointer;

Level::Level(const std::string & filePath) {
	this->filePath = filePath;
	Load();
}
Level::~Level(void) {
	delete script;
	for (int i = 0; i < tileMap.size(); i++) {
		delete tileMap[i];
	}
}

void Level::Update(const float& deltaTime) {
	// Use the cameras position (top left of its viewport) to calculate where to update
	glm::vec2 topLeftGridPosition = Engine_Pointer->ConvertToGridPosition(glm::vec2(Engine_Pointer->mainCamera->position.x, Engine_Pointer->mainCamera->position.y));
	glm::vec2 bottomRightGridPosition = topLeftGridPosition + (Engine_Pointer->windowGridSize + glm::vec2(1, 1));	// the +(1,1) here is to update one extra line of tiles on each axis, preventing odd behaviour when things are only partially visible.

	// Clamp the bottomRight of the update area to the bounds of the world, the cameras already had its position clamped so we don't need to do it again.
	int xAxis = (int)glm::clamp(bottomRightGridPosition.x, topLeftGridPosition.x, tileGridSize.x);
	int yAxis = (int)glm::clamp(bottomRightGridPosition.y, topLeftGridPosition.y, tileGridSize.y);

	for (int y = (int)topLeftGridPosition.y; y < yAxis; y++) {
		for (int x = (int)topLeftGridPosition.x; x < xAxis;  x++) {
			int index = y * (int)tileGridSize.x + x;
			tileMap[index]->Update(deltaTime);
		}
	}

}
void Level::Draw(void) {
	// Use the cameras position (top left of its viewport) to calculate where to update
	glm::vec2 topLeftGridPosition = Engine_Pointer->ConvertToGridPosition(glm::vec2(Engine_Pointer->mainCamera->position.x, Engine_Pointer->mainCamera->position.y));
	glm::vec2 bottomRightGridPosition = topLeftGridPosition + (Engine_Pointer->windowGridSize + glm::vec2(1, 1));	// the +(1,1) here is to render one extra line of tiles on each axis, preventing terrain popping in and out of existence.

	// Clamp the bottomRight of the render area to the bounds of the world, the cameras already had its position clamped so we don't need to do it again.
	int xAxis = (int)glm::clamp(bottomRightGridPosition.x, topLeftGridPosition.x, tileGridSize.x);
	int yAxis = (int)glm::clamp(bottomRightGridPosition.y, topLeftGridPosition.y, tileGridSize.y);

	for (int y = (int)topLeftGridPosition.y; y < yAxis; y++) {
		for (int x = (int)topLeftGridPosition.x; x < xAxis; x++) {
			int index = y * (int)tileGridSize.x + x;
			tileMap[index]->Draw();
		}
	}
}
bool Level::IsTileSolid(const glm::vec2 & gridPosition) {
	int index = (int)gridPosition.y * (int)tileGridSize.x + (int)gridPosition.x;
	if ((int)gridPosition.x < 0) return true;
	if ((int)gridPosition.x >= tileGridSize.x) return true;
	if ((int)gridPosition.y < 0) return true;
	if ((int)gridPosition.y >= tileGridSize.y) return true;
	return tileMap[index]->type == Tile::Type::Solid;
}
BoundingBox* Level::GetTileBoundingBox(const glm::vec2 & gridPosition) {
	if ((int)gridPosition.x < 0) return nullptr;
	if ((int)gridPosition.x >= tileGridSize.x) return nullptr;
	if ((int)gridPosition.y < 0) return nullptr;
	if ((int)gridPosition.y >= tileGridSize.y) return nullptr;

	int index = (int)gridPosition.y * (int)tileGridSize.x + (int)gridPosition.x;
	size_t sizeOftileMap = tileMap.size();
	if (index < sizeOftileMap) {
		return &tileMap[index]->boundingBox;
	}
	else {
		return nullptr;
	}
}
void Level::Reload(void) {
	std::cout << ">> RELOADING MAP FILE" << std::endl;
	if (script->isScriptLoaded) {
		delete script;
		for (int i = 0; i < tileMap.size(); i++) {
			delete tileMap[i];
		}
		tileMap.clear();
	}
	Load();
}
void Level::Load(void) {
	// Load the information from the script
	script = new LuaScript(filePath);
	if (script->isScriptLoaded) {
		// Grab the data from the script.
		name = script->Get<std::string>("map.level_name");
		nameOfTilest = script->Get<std::string>("map.tileset_name");
		tileGridSize = glm::vec2(script->Get<int>("map.tile_grid_size.x"), script->Get<int>("map.tile_grid_size.y"));
		pixelGridSize = tileGridSize * Engine_Pointer->tileSize;
		playerStartPosition = glm::vec2(script->Get<int>("map.player_start_grid_position.x"), script->Get<int>("map.player_start_grid_position.y"));
		std::vector<int> rawMapData = script->GetVector<int>("map.map_data");
		
		// Find the index of tileset to use for this level in the Engines tileset register.
		indexOfTileset = -1;
		for (size_t i = 0; i < Engine_Pointer->tilesetRegister.size(); i++) {
			if (Engine_Pointer->tilesetRegister[i].name == nameOfTilest) {
				indexOfTileset = (int)i;
			}
		}
		// If the wanted one wasn't found, use the default.
		if (indexOfTileset == -1) {
			indexOfTileset = 0;
		}

		// Populate the tilemap.
		for (int y = 0; y < (int)tileGridSize.y; y++) {
			for (int x = 0; x < (int)tileGridSize.x; x++) {
				int index = y * (int)tileGridSize.x + x;

				const Texture* texture = Engine_Pointer->tilesetRegister[indexOfTileset].tileList[rawMapData[index]].texture;
				Tile::Type type = Engine_Pointer->tilesetRegister[indexOfTileset].tileList[rawMapData[index]].type;
				glm::ivec2 sourceFramePosition = Engine_Pointer->tilesetRegister[indexOfTileset].tileList[rawMapData[index]].sourceFramePosition;
				glm::vec3 position = glm::vec3(x * Engine_Pointer->tileSize.x, y * Engine_Pointer->tileSize.y, -0.01f);
				tileMap.push_back(new Tile(*texture, type, sourceFramePosition, position));
			}
		}

	}
}