#include "Tileset.h"
#include "Engine.h"
#include "LuaScript.h"

Engine* Tileset::Engine_Pointer;

Tileset::Tileset() {

}

Tileset::~Tileset() {
}

void Tileset::Load(const std::string & tilesetScript) {
	LuaScript tileConfigScript = LuaScript(tilesetScript);
	if (tileConfigScript.isScriptLoaded) {
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
			std::string tileType = tileConfigScript.Get<std::string>("tileset.tile_" + std::to_string(i) + ".type");
			int sourceFrameX = tileConfigScript.Get<int>("tileset.tile_" + std::to_string(i) + ".source_frame_position.x");
			int sourceFrameY = tileConfigScript.Get<int>("tileset.tile_" + std::to_string(i) + ".source_frame_position.y");
			glm::vec2 sourceFramePosition = glm::vec2(sourceFrameX, sourceFrameY);

			if (indexOfTileSetTexture != -1) {
				tileList.push_back(Tile(Engine_Pointer->modelRegister[0], Engine_Pointer->textureRegister[indexOfTileSetTexture], "", sourceFramePosition));
			}
			else {
				tileList.push_back(Tile(Engine_Pointer->modelRegister[0], Engine_Pointer->textureRegister[Engine_Pointer->indexOfDefaultTexture], "", sourceFramePosition));
			}
		}
	}
}
