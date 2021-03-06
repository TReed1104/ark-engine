#ifndef ARKENGINE_LEVEL_MANAGER_H_
#define ARKENGINE_LEVEL_MANAGER_H_

#include <vector>
#include "Level.h"

class LevelManager {
public:
	static LevelManager* GetInstance();
	LevelManager(LevelManager const&) = delete;
	void operator=(LevelManager const&) = delete;

	void RegisterLevel(Level* newLevel);
	void Close(void);
	const int GetIndexOfLevel(const std::string& levelName);
	Level* GetLevel(const std::string& levelName);
	Level* GetCurrentLevel(void);
	bool SetCurrentLevel(const std::string& levelName);

	bool Update(const float& deltaTime);
	bool Render(void);

private:
	std::vector<Level*> levelRegister;
	int indexOfCurrentLevel = -1;

	LevelManager() {}

};

#endif