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

		// Get the index of theTileset's texture.
		indexOfTileSetTexture = Engine_Pointer->GetIndexOfTexture(tilesetTextureName);

		// Load each of the Tiles from the current Tileset script.
		for (int i = 0; i < numberOfTiles; i++) {
			int tileType = tileConfigScript.Get<int>("tileset.tile_" + std::to_string(i) + ".type");
			bool isSlope = tileConfigScript.Get<bool>("tileset.tile_" + std::to_string(i) + ".isSlope");
			int sourceFrameX = tileConfigScript.Get<int>("tileset.tile_" + std::to_string(i) + ".source_frame_position.x");
			int sourceFrameY = tileConfigScript.Get<int>("tileset.tile_" + std::to_string(i) + ".source_frame_position.y");
			glm::ivec2 sourceFramePosition = glm::ivec2(sourceFrameX, sourceFrameY);

			glm::ivec2 aabbOffset = glm::ivec2(tileConfigScript.Get<int>("tileset.tile_" + std::to_string(i) + ".bounding_box.x_offset"), tileConfigScript.Get<int>("tileset.tile_" + std::to_string(i) + ".bounding_box.y_offset"));
			glm::ivec2 aabbDimensions = glm::ivec2(tileConfigScript.Get<int>("tileset.tile_" + std::to_string(i) + ".bounding_box.width"), tileConfigScript.Get<int>("tileset.tile_" + std::to_string(i) + ".bounding_box.height"));

			if (indexOfTileSetTexture != -1) {
				tileList.push_back(Tile(Engine_Pointer->textureRegister[indexOfTileSetTexture], (Tile::Type)tileType, isSlope, sourceFramePosition, glm::vec3(0.0f), BoundingBox(glm::ivec2(0), aabbDimensions), aabbOffset));
			}
			else {
				tileList.push_back(Tile(Engine_Pointer->textureRegister[indexOfTileSetTexture], (Tile::Type)tileType, isSlope, sourceFramePosition, glm::vec3(0.0f), BoundingBox(glm::ivec2(0), aabbDimensions), aabbOffset));
			}
		}
	}
}
