#include "Level.h"
#include "Engine.h"

Engine* Level::Engine_Pointer;

Level::Level(const std::string& filePath) {
	this->filePath = filePath;
	isLoaded = Load();
}
Level::~Level(void) {
	// Delete the config file from memory
	if (configFile != nullptr) {
		delete configFile;
	}

	// Delete the lightsources
	for (LightSource* light : lightSourcesRegister) {
		delete light;
		light = nullptr;
	}

	// Delete the background effect
	if (levelBackgroundSurface != nullptr) {
		delete levelBackgroundSurface;
	}

	// Delete the tilemap
	for (Tile* tile : tileMap) {
		delete tile;
		tile = nullptr;
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

	// Update the level lights
	for (LightSource* light : lightSourcesRegister) {
		light->Update(deltaTime);
	}

	// Update the level background
	if (levelBackgroundSurface != nullptr) {
		levelBackgroundSurface->Update(deltaTime);
	}
}
void Level::Draw(void) {
	// Render the level background
	if (levelBackgroundSurface != nullptr) {
		levelBackgroundSurface->Draw();
	}
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
const std::string Level::GetName(void) {
	return this->name;
}
const std::string Level::GetFilePath(void) {
	return this->filePath;
}
const std::string Level::GetTileSetName(void) {
	return this->nameOfTilest;
}
const glm::vec2 Level::GetPlayerStartPosition(void) {
	return this->playerStartPosition;
}
const glm::vec2 Level::GetPixelGridSize(void) {
	return this->pixelGridSize;
}
const bool Level::IsLoaded(void) {
	return isLoaded;
}
void Level::PlayBackgroundMusic(void) {
	// Check the level has had its background sound set
	if (levelMusicTrack != nullptr) {
		// Play the background music of the level
		levelMusicTrack->Play();
	}
}
void Level::PauseBackgroundMusic(void) {
	// Check the level has had its background sound set
	if (levelMusicTrack != nullptr) {
		// Play the background music of the level
		levelMusicTrack->Pause();
	}
}
void Level::StopBackgroundMusic(void) {
	// Check the level has had its background sound set
	if (levelMusicTrack != nullptr) {
		// Play the background music of the level
		levelMusicTrack->Stop();
	}
}
bool Level::IsTileValid(const glm::vec2& gridPosition) {
	if ((int)gridPosition.x < 0) return false;
	if ((int)gridPosition.x >= tileGridSize.x) return false;
	if ((int)gridPosition.y < 0) return false;
	if ((int)gridPosition.y >= tileGridSize.y) return false;
	return true;
}
bool Level::IsTileSolid(const glm::vec2& gridPosition) {
	if (IsTileValid(gridPosition)) {
		int index = (int)gridPosition.y * (int)tileGridSize.x + (int)gridPosition.x;
		return tileMap[index]->type == Tile::Type::Solid;
	}
	else {
		return true;
	}
}
BoundingBox* Level::GetTileBoundingBox(const glm::vec2& gridPosition) {
	if (IsTileValid(gridPosition)) {
		int index = (int)gridPosition.y * (int)tileGridSize.x + (int)gridPosition.x;
		return &tileMap[index]->boundingBox;
	}
	else {
		return nullptr;
	}
}
bool Level::Load(void) {
	configFile = new JsonFile(filePath);
	if (configFile->IsLoaded()) {
		name = configFile->Get<std::string>("level.id");
		nameOfTilest = configFile->Get<std::string>("level.tileset");
		tileGridSize = glm::vec2(configFile->Get<int>("level.dimensions.width"), configFile->Get<int>("level.dimensions.height"));
		pixelGridSize = tileGridSize * Engine_Pointer->tileSize;
		playerStartPosition = glm::vec2(configFile->Get<int>("level.player start position.x"), configFile->Get<int>("level.player start position.y"));
		std::vector<int> rawMapData = configFile->GetVector<int>("level.tile grid");

		// Light Sources
		const size_t numberOfLightSources = configFile->SizeOfObjectArray("level.lightsources");
		for (size_t i = 0; i < numberOfLightSources; i++) {
			std::string lightID = configFile->Get<std::string>("level.lightsources." + std::to_string(i) + ".light.id");
			glm::vec3 lightPosition = glm::vec3(configFile->Get<int>("level.lightsources." + std::to_string(i) + ".light.position.x"), configFile->Get<int>("level.lightsources." + std::to_string(i) + ".light.position.y"), configFile->Get<int>("level.lightsources." + std::to_string(i) + ".light.position.z"));
			glm::vec3 lightColour = glm::vec3(configFile->Get<float>("level.lightsources." + std::to_string(i) + ".light.colour.red"), configFile->Get<float>("level.lightsources." + std::to_string(i) + ".light.colour.green"), configFile->Get<float>("level.lightsources." + std::to_string(i) + ".light.colour.blue"));
			float lightRadius = configFile->Get<float>("level.lightsources." + std::to_string(i) + ".light.radius");
			lightSourcesRegister.push_back(new LightSource(lightID, lightPosition, lightColour, lightRadius));
		}

		// Sound effects
		std::string soundEffectId = configFile->Get<std::string>("level.sounds.background");
		int indexOfSoundEffect = Engine_Pointer->GetIndexOfSoundEffect(soundEffectId);
		if (indexOfSoundEffect != -1) {
			levelMusicTrack = Engine_Pointer->soundEffectRegister[indexOfSoundEffect];
		}
		else {
			Engine_Pointer->engineDebugger.WriteLine(">>>> Could not find background sound effect: " + soundEffectId);
			levelMusicTrack = nullptr;
		}

		// Backgrounds
		std::string backgroundID = configFile->Get<std::string>("level.background.id");
		std::string backgroundTexturePath = Engine_Pointer->contentDirectory + "textures\\" + configFile->Get<std::string>("level.background.texture");
		std::string backgroundShaderID = configFile->Get<std::string>("level.background.shader");
		levelBackgroundSurface = new Background(backgroundID, backgroundTexturePath, backgroundShaderID);
		if (!levelBackgroundSurface->IsLoaded()) {
			delete levelBackgroundSurface;
			levelBackgroundSurface = nullptr;
		}
		
		// Find the index of tileset to use for this level in the Engines tileset register.
		indexOfTileset = -1;
		const size_t numberOfTiles = Engine_Pointer->tilesetRegister.size();
		for (size_t i = 0; i < numberOfTiles; i++) {
			if (Engine_Pointer->tilesetRegister[i]->GetName() == nameOfTilest) {
				indexOfTileset = (int)i;
			}
		}
		// If the wanted one wasn't found, use the default.
		if (indexOfTileset == -1) {
			indexOfTileset = 0;
		}

		// Populate the tilemap.
		const std::vector<Tile>* tileSet = Engine_Pointer->tilesetRegister[indexOfTileset]->GetTiles();
		for (int y = 0; y < (int)tileGridSize.y; y++) {
			for (int x = 0; x < (int)tileGridSize.x; x++) {
				// Get the tile from the tileset which we want to copy
				Tile currentTile = tileSet->at(rawMapData[(y * (int)tileGridSize.x + x)]);
				// Create the new tile, copying its state from the default tile
				Tile* newTile = new Tile(currentTile);
				// The copied tile will be at a default position, so reposition it
				newTile->Reposition(glm::vec3(x * Engine_Pointer->tileSize.x, y * Engine_Pointer->tileSize.y, -0.01f));

				// push our new tile to the tilemap
				tileMap.push_back(newTile);
			}
		}

		return true;
	}
	else {
		Engine_Pointer->engineDebugger.WriteLine(">>>> ERROR: Could not load level: " + name);
		return false;
	}
}
void Level::Reload(void) {
	Engine_Pointer->engineDebugger.WriteLine(">>>> Reloading Level: " + name);
	if (configFile->IsLoaded()) {
		delete configFile;
		for (Tile* tile : tileMap) {
			delete tile;
			tile = nullptr;
		}
		tileMap.clear();

		// Stop the background music if its been set and is playing
		if (levelMusicTrack != nullptr) {
			levelMusicTrack->Stop();
		}
	}
	isLoaded = Load();
	
	// Start the background track playing again, now that we've reloaded the file
	if (levelMusicTrack != nullptr) {
		levelMusicTrack->Play();
	}
}
std::map<std::string, std::string> Level::ExportDataForBinding(void) {
	std::map<std::string, std::string> exportData;

	// Format the data into the strings which the UIs will be able to render
	exportData["level_name"] = name;
	exportData["level_file_path"] = filePath;
	exportData["level_tile_set"] = nameOfTilest;
	exportData["level_background"] = levelBackgroundSurface->GetName();
	exportData["level_music"] = levelMusicTrack->GetName();
	exportData["level_size_in_pixels"] = std::to_string(pixelGridSize.x) + ", " + std::to_string(pixelGridSize.y);
	exportData["level_size_in_tiles"] = std::to_string(tileGridSize.x) + ", " + std::to_string(tileGridSize.y);
	exportData["level_player_start_position"] = std::to_string(playerStartPosition.x) + ", " + std::to_string(playerStartPosition.y);

	return exportData;
}
