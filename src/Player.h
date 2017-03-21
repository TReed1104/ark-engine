#ifndef ARKENGINE_PLAYER_H_
#define ARKENGINE_PLAYER_H_
#include "Entity.h"
#include "Keyboard.h"

class Player : public Entity
{
public:
	Keyboard::Keys keybindMovementUp;
	Keyboard::Keys keybindMovementDown;
	Keyboard::Keys keybindMovementLeft;
	Keyboard::Keys keybindMovementRight;

	Player(const Model& model, const Texture& texture, const glm::vec3& position = glm::vec3(0.0f), const glm::vec2& sourceFrameSize = glm::vec2(16, 16));
	~Player();
	
	void HandleMovement(void);
	void HandleInput_WithController(void);
	void HandleInput_WithoutController(void);

private:

};

#endif