#ifndef ARKENGINE_PLAYER_H_
#define ARKENGINE_PLAYER_H_
#include "Entity.h"
#include "Keyboard.h"

class Engine;

class Player : public Entity
{
public:
	
	Player(const std::string& scriptPath);
	~Player(void);
	
	void EntityController(void);
	void HandleInputKeyboard(void);
	void HandleInputGameController(void);

private:

};

#endif