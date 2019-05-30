#include "LevelManager.h"

LevelManager* LevelManager::instance = nullptr;

LevelManager::LevelManager() {
}

LevelManager::~LevelManager() {
	if (instance != nullptr) {
		delete instance;
	}
}

LevelManager * LevelManager::GetInstance() {
	if (instance == nullptr) {
		instance = new LevelManager();
	}
	return instance;
}
