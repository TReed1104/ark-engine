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
	const size_t numberOfLightSources = lightSourcesRegister.size();
	for (size_t i = 0; i < numberOfLightSources; i++) {
		delete lightSourcesRegister[i];
	}

	// Delete the background effect
	if (levelBackground != nullptr) {
		delete levelBackground;
	}

	// Delete the tilemap
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

	// Update the level background
	if (levelBackground != nullptr) {
		levelBackground->Update(deltaTime);
	}
}
void Level::Draw(void) {
	// Render the level background
	if (levelBackground != nullptr) {
		levelBackground->Draw();
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
const bool Level::IsLoaded(void) {
	return isLoaded;
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

		// Sound effects
		std::string soundEffectId = configFile->Get<std::string>("level.sounds.background");
		int indexOfSoundEffect = Engine_Pointer->GetIndexOfSoundEffect(soundEffectId);
		if (indexOfSoundEffect != -1) {
			backgroundSoundEffect = Engine_Pointer->soundEffectRegister[indexOfSoundEffect];
		}
		else {
			Engine_Pointer->engineDebugger.WriteLine(">>>> Could not find background sound effect: " + soundEffectId);
			backgroundSoundEffect = nullptr;
		}

		// Backgrounds
		std::string backgroundID = configFile->Get<std::string>("level.background.id");
		std::string backgroundTexturePath = Engine_Pointer->contentDirectory + "textures\\" + configFile->Get<std::string>("level.background.texture");
		std::string backgroundShaderID = configFile->Get<std::string>("level.background.shader");
		levelBackground = new Background(backgroundID, backgroundTexturePath, backgroundShaderID);
		if (!levelBackground->IsLoaded()) {
			delete levelBackground;
			levelBackground = nullptr;
		}
		
		// Find the index of tileset to use for this level in the Engines tileset register.
		indexOfTileset = -1;
		for (size_t i = 0; i < Engine_Pointer->tilesetRegister.size(); i++) {
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
		for (int i = 0; i < tileMap.size(); i++) {
			delete tileMap[i];
		}
		tileMap.clear();

		// Stop the background music if its been set and is playing
		if (backgroundSoundEffect != nullptr) {
			backgroundSoundEffect->Stop();
		}
	}
	isLoaded = Load();
	
	// Start the background track playing again, now that we've reloaded the file
	if (backgroundSoundEffect != nullptr) {
		backgroundSoundEffect->Play();
	}
}
