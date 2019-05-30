#ifndef ARKENGINE_LEVELMANAGER_H_
#define ARKENGINE_LEVELMANAGER_H_

#include <vector>
#include "Level.h"

class LevelManager {
public:
	~LevelManager();

	static LevelManager* GetInstance();

private:
	static LevelManager* instance;
	LevelManager();

};

#endif