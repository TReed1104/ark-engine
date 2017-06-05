#include "Tileset.h"
#include "Engine.h"
#include "LuaScript.h"

Engine* Tileset::Engine_Pointer;

Tileset::Tileset(const std::string& tilesetScript) {
	Load(tilesetScript);
}

Tileset::~Tileset() {
}

void Tileset::Load(const std::string& tilesetScript) {
	LuaScript tileConfigScript = LuaScript(tilesetScript);
	if (tileConfigScript.isScriptLoaded) {
		// Get the config script values
		name = tileConfigScript.Get<std::string>("tileset.name");
		int numberOfTiles = tileConfigScript.Get<int>("tileset.number_of_tiles");

		// Find the Texture for this Tileset.
		int indexOfTileSetTexture = -1;
		std::string tilesetTextureName = tileConfigScript.Get<std::string>("tileset.texture");

		for (size_t i = 0; i < Engine_Pointer->textureRegister.size(); i++) {
			if (Engine_Pointer->textureRegister[i].name.find(tilesetTextureName) != std::string::npos) {
				indexOfTileSetTexture = i;
			}
		}

		// Load each of the Tiles from the current Tileset script.
		for (int i = 0; i < numberOfTiles; i++) {
			int tileType = tileConfigScript.Get<int>("tileset.tile_" + std::to_string(i) + ".type");
			int sourceFrameX = tileConfigScript.Get<int>("tileset.tile_" + std::to_string(i) + ".source_frame_position.x");
			int sourceFrameY = tileConfigScript.Get<int>("tileset.tile_" + std::to_string(i) + ".source_frame_position.y");
			glm::ivec2 sourceFramePosition = glm::ivec2(sourceFrameX, sourceFrameY);

			if (indexOfTileSetTexture != -1) {
				tileList.push_back(Tile(Engine_Pointer->textureRegister[indexOfTileSetTexture], (Tile::Type)tileType, sourceFramePosition));
			}
			else {
				tileList.push_back(Tile(Engine_Pointer->textureRegister[Engine_Pointer->indexOfDefaultTexture], (Tile::Type)tileType, sourceFramePosition));
			}
		}
	}
}
