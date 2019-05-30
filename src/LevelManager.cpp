#include "LevelManager.h"

LevelManager& LevelManager::GetInstance() {
	static LevelManager instance;
	return instance;
}

void LevelManager::RegisterLevel(Level& newLevel) {
	levelRegister.push_back(&newLevel);
}
const int LevelManager::GetIndexOfLevel(const std::string & levelName) {
	int indexOfLevel = -1;
	for (size_t i = 0; i < levelRegister.size(); i++) {
		if (levelRegister[i]->name == levelName) {
			indexOfLevel = (int)i;
		}
	}
	return indexOfLevel;
}
Level* LevelManager::GetLevel(const std::string& levelName) {
	int indexOfLevel = -1;
	for (size_t i = 0; i < levelRegister.size(); i++) {
		if (levelRegister[i]->name == levelName) {
			indexOfLevel = (int)i;;
		}
	}
	if (indexOfLevel == -1) {
		return nullptr;
	}
	return levelRegister[indexOfLevel];
}
Level* LevelManager::GetCurrentLevel(void) {
	if (indexOfCurrentLevel == -1 || indexOfCurrentLevel > levelRegister.size()) {
		return nullptr;
	}
	else {
		return levelRegister[indexOfCurrentLevel];
	}
}
bool LevelManager::SetCurrentLevel(const std::string& levelName) {
	int indexOfNewLevel = GetIndexOfLevel(levelName);
	if (indexOfNewLevel != -1) {
		indexOfCurrentLevel = indexOfNewLevel;
		return true;
	}
	else {
		return false;
	}
}
