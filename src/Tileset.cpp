#include "Tileset.h"
#include "Engine.h"

Engine* Tileset::Engine_Pointer;

Tileset::Tileset(const std::string& tilesetConfig) {
	configFile = new JsonFile(tilesetConfig);
	isLoaded = Load();
}
Tileset::~Tileset() {
	if (configFile != nullptr) {
		delete configFile;
	}
}

const std::string Tileset::GetName(void) {
	return name;
}
const bool Tileset::IsLoaded(void) {
	return isLoaded;
}
const std::vector<Tile>* Tileset::GetTiles(void) {
	return &tileList;
}
bool Tileset::Load() {
	if (configFile->IsLoaded()) {
		name = configFile->Get<std::string>("tileset.id");
		int indexOfTileSetTexture = -1;
		std::string textureName = configFile->Get<std::string>("tileset.texture");
		indexOfTileSetTexture = Engine_Pointer->GetIndexOfTexture(textureName);
		if (indexOfTileSetTexture != -1) {
			const size_t numberOfTiles = configFile->SizeOfObjectArray("tileset.tiles");
			for (size_t i = 0; i < numberOfTiles; i++) {
				int tileType = configFile->Get<int>("tileset.tiles." + std::to_string(i) + ".tile.type");
				bool isSlope = configFile->Get<bool>("tileset.tiles." + std::to_string(i) + ".tile.slope.is slope");
				glm::ivec2 slopeOffset = glm::ivec2(configFile->Get<int>("tileset.tiles." + std::to_string(i) + ".tile.slope.slope offset.left"), configFile->Get<int>("tileset.tiles." + std::to_string(i) + ".tile.slope.slope offset.right"));
				glm::ivec2 aabbOffset = glm::ivec2(configFile->Get<int>("tileset.tiles." + std::to_string(i) + ".tile.bounding box.offset.x"), configFile->Get<int>("tileset.tiles." + std::to_string(i) + ".tile.bounding box.offset.y"));
				glm::ivec2 aabbDimensions = glm::ivec2(configFile->Get<int>("tileset.tiles." + std::to_string(i) + ".tile.bounding box.dimensions.width"), configFile->Get<int>("tileset.tiles." + std::to_string(i) + ".tile.bounding box.dimensions.height"));
				glm::ivec2 sourceFramePosition = glm::ivec2(configFile->Get<int>("tileset.tiles." + std::to_string(i) + ".tile.source frame.position.x"), configFile->Get<int>("tileset.tiles." + std::to_string(i) + ".tile.source frame.position.y"));
				if (indexOfTileSetTexture != -1) {
					tileList.push_back(Tile(Engine_Pointer->textureRegister[indexOfTileSetTexture], (Tile::Type)tileType, sourceFramePosition, glm::vec3(0.0f), BoundingBox(glm::ivec2(0), aabbDimensions), aabbOffset, isSlope, slopeOffset));
				}
				else {
					tileList.push_back(Tile(Engine_Pointer->textureRegister[indexOfTileSetTexture], (Tile::Type)tileType, sourceFramePosition, glm::vec3(0.0f), BoundingBox(glm::ivec2(0), aabbDimensions), aabbOffset, isSlope, slopeOffset));
				}
			}
			return true;
		}
		else {
			Engine_Pointer->engineDebugger.WriteLine(">>>> ERROR!!!! - Tileset texture was not found in the texture register");
			return false;
		}
	}
	else {
		Engine_Pointer->engineDebugger.WriteLine(">>>> ERROR!!!! - Tileset config file wasn't loaded");
		return false;
	}
}
