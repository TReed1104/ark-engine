#ifndef ARKENGINE_PLAYER_H_
#define ARKENGINE_PLAYER_H_
#include "Entity.h"
#include "Keyboard.h"

class Engine;

class Player : public Entity
{
public:
	
	Player(const std::string& scriptPath);
	~Player();
	
	void HandleMovement(void);
	void HandleInput_WithController(void);
	void HandleInput_WithoutController(void);

private:

};

#endif