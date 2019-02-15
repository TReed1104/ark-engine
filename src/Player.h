#ifndef ARKENGINE_PLAYER_H_
#define ARKENGINE_PLAYER_H_
#include "Entity.h"
#include "Keyboard.h"

class Player : public Entity
{
public:
	Player(const std::string& filePath);
	~Player(void);

private:
	void HandleInputGameController(void);
	void HandleInputKeyboard(void);
	void EntityController(void);

};
#endif